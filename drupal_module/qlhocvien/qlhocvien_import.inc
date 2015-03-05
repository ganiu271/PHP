<?php
function qlhocvien_form_import($form, &$form_state, $importform) {
    global $base_url;
    if(!user_access('UIT qlhocvien')) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>IMPORT THÔNG TIN HỌC VIÊN</h1></center>';
    $form['markup'] = array('#markup' => $html);
    $options = array(
            '' => 'Chọn mẫu import',
            'hocvien' => 'Thêm / cập nhật học viên (xls, xlsx)',
            'thongtin' => 'Cập nhật thông tin HV (xls, xlsx)',
            'hinhanh' => 'Upload hình ảnh (jpg, zip)',
            );
    
    $form['importform'] = array(
            '#title'=>t('Chọn mẫu import:'),
            '#type'=>'select',
            '#options'=>$options,
            '#default_value'=> (isset($importform) ? $importform:NULL),
            '#attributes' => array('onchange' => 'location.href=\''.$base_url.'/admin/qlhocvien/import/\'+document.getElementById(\'edit-importform\').value;'),
            //'#attributes' => array('onchange' => 'location.href=\''.$base_url.'/?q=admin/qlhocvien/import/\'+document.getElementById(\'edit-importform\').value;'),
    );
    if($importform == 'hocvien') {
        $form['chocapnhat'] = array(
            '#title'=>t('Cho phép cập nhật nếu trùng mã HV ?'),
            '#type'=>'checkbox',
            '#default_value'=> 1,
        );
    }
    
    $form['browser'] = array(
      '#type' => 'fieldset',
      '#title' => t('Import file'),
      '#collapsible' => TRUE,
      '#description' => t("Upload file.").qlhocvien_import_template($importform),
    );
    $file_size = t('Kích thước tới đa: !size MB.', array('!size' => file_upload_max_size()/1024/1024));
    $form['browser']['file_upload'] = array(
      '#type' => 'file',
      '#title' => t('Tải File'),
      '#size' => 40,
      '#description' => t('Chọn file để import. ') . $file_size,
    );

    $form['submit'] = array(
        '#type' => 'submit', 
        '#value' => t('Import'),
    );
    $form['cancel'] = array(
        '#type' => 'button', 
        '#value' => t('Đóng'),
        '#validate' => FALSE,
        '#executes_submit_callback' => FALSE,
        '#attributes' => array('onclick'=>'window.close();'),
    );
    // set the form encoding type
    $form['#attributes']['enctype'] = "multipart/form-data";    
    
    return $form;
}

function qlhocvien_form_import_validate($form, &$form_state) {
    $importform = $form_state['values']['importform'];
    if($importform == '') {
        form_set_error('importform', t('Phải chọn mẫu import.'));
        return;
    }
    switch ($importform) {
        case 'hinhanh':
            $file = file_save_upload('file_upload', array('file_validate_extensions'=>array('jpg zip')), 'public://uploads/qlhocvien', FILE_EXISTS_REPLACE);
            break;
        default:
            $file = file_save_upload('file_upload', array('file_validate_extensions'=>array('xls xlsx')), 'public://uploads/qlhocvien', FILE_EXISTS_REPLACE);
            break;
    }
    
    // check file uploaded OK
    if (!$file) { 
      form_set_error('file_upload', t('Upload bị lỗi.'));
    }
    $form_state['values']['file_upload'] = $file;  
}

function qlhocvien_form_import_submit($form, &$form_state) {
    $file = $form_state['values']['file_upload'];
    $importform = $form_state['values']['importform'];
    $filepath = 'sites/default/files/uploads/qlhocvien';
    if(!file_exists($filepath)) mkdir($filepath);
    $filepath .= '/'.$file->filename;
    switch ($importform) {
        case 'hocvien':
            $AllowUpdate = $form_state['values']['chocapnhat'];
            import_hocvien($filepath, $AllowUpdate);
            break;
        case 'trangthai':
            import_trangthai($filepath);
            break;
        case 'thongtin':
            import_thongtin($filepath);
            break;
        case 'hinhanh':
            importhinhanh($filepath);
            break;
        default:
            break;
    }
    return $form;
}

function import_thongtin($filepath) {
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $objPHPExcel = PHPExcel_IOFactory::load($filepath);
    $ActiveSheet = $objPHPExcel->setActiveSheetIndex('0');
    
    $arrTinhTrang = db_select('uit_tinhtrang', 't')->fields('t', array('id', 'id'))->condition('doituong', 'SV')->execute()->fetchAllKeyed();
    $arrFieldsVB = array('sohieu','sovaoso','hinhthucdt','ngaycapvb');
    $arrFieldsDB = array('mahv','tinhtrang','namhoc','soqd','ngayqd','ghichu');
    $arrFields = array();
    $arrFields2 = array();
    $col_masv = NULL;
    
    for($c='A';$c <= $ActiveSheet->getHighestColumn() || strlen($c) < strlen($ActiveSheet->getHighestColumn());$c++) {
        $fieldname = $ActiveSheet->getCell($c.'1')->getValue();
        if(in_array($fieldname, $arrFieldsDB)) {
            $arrFields[$fieldname] = $c;
            if($fieldname == 'mahv') $col_masv = $c;
        }
        if(in_array($fieldname, $arrFieldsVB)) {
            $arrFields2[$fieldname] = $c;
        }
    }
    if(!isset($arrFields['mahv']) || !isset($arrFields['soqd']) || !isset($arrFields['ngayqd'])) {
        drupal_set_message('Dữ liệu thiếu các cột cần thiết', 'error');
        return;
    }
    if(count($arrFields2) > 0 && (!isset($arrFields2['sohieu']) || !isset($arrFields2['sovaoso'])) ) {
        drupal_set_message('Để kết hợp cập nhật thông tin văn bằng thì phải có tối thiểu 02 cột: sovaoso và sohieu', 'error');
        return;
    }
    $suss = 0;
    $err = 0;
    $updated = 0;
    for($r = 2; $r <= $ActiveSheet->getHighestRow(); $r++) {
        $masv = trim($ActiveSheet->getCell($col_masv.$r)->getValue());
        $masv = strtolower($masv);
        if(!isset($masv) || $masv == '') continue;
        $sv = db_select('uit_thongtinhv', 't')->fields('t', array('tinhtrang','hoten','ngaysinh','gioitinh','noisinh','hedt','khoahoc'))->condition('mahv', $masv)->execute()->fetch();
        if(!$sv) {
            drupal_set_message('Dòng '.$r.' có MSSV không hợp lệ ('.$masv.')', 'error');
            continue;
        }
        $fields = array('mahv' => $masv,'trangthai' => 1);
        //$fields['namhoc'] = (int)$ActiveSheet->getCell($arrFields['namhoc'].$r)->getValue();
        $fields['soqd'] = trim($ActiveSheet->getCell($arrFields['soqd'].$r)->getValue());
        $tmp = $ActiveSheet->getCell($arrFields['ngayqd'].$r)->getValue();
        if(strlen($tmp) >= 8 || is_numeric($tmp)) {
            $tmp = PHPExcel_Style_NumberFormat::toFormattedString($tmp, "dd/mm/yyyy");
//            $fields['ngayqd'] = ConvertToDate($tmp);
            $fields['ngayqd'] = $tmp;
        } else {
            $fields['ngayqd'] = $tmp;
        }
        $fields['namhoc'] = date('Y', strtotime(ConvertToDate($fields['ngayqd'])));
        if($fields['soqd'] == '' || $fields['ngayqd'] == '') {
            drupal_set_message('Thông tin số hoặc ngày QĐ không hợp lệ (dòng '.$r.')', 'error');
            continue;
        }
        
        //if(isset($arrFields['lydo'])) $fields['lydo'] = trim($ActiveSheet->getCell($arrFields['lydo'].$r)->getValue());
        if(isset($arrFields['ghichu'])) $fields['ghichu'] = trim($ActiveSheet->getCell($arrFields['ghichu'].$r)->getValue());
        
        // Cập nhật thông tin văn bằng
        if(count($arrFields2) > 0) {
            $hoten = TachHoTen($sv->hoten);
            $sohieu = trim($ActiveSheet->getCell($arrFields2['sohieu'].$r)->getValue());
            $fields_vb = array(
                'ho' => $hoten['ho'],
                'ten' => $hoten['ten'],
                'ngaysinh' => strlen($sv->ngaysinh) == 10 ? date('d/m/Y',  strtotime($sv->ngaysinh)):$sv->ngaysinh,
                'noisinh' => $sv->noisinh,
                'gioitinh' => $sv->gioitinh == 1 ? 'Nam':'Nữ',
                'khoahoc' => $sv->khoahoc,
                'namtn' => $fields['namhoc'],
                'nganhdt' => 'KHMT',
                //'xeploai' => isset($arrFields2['xeploai']) ? trim($ActiveSheet->getCell($arrFields2['xeploai'].$r)->getValue()) : '',
                'hinhthucdt' => isset($arrFields2['hinhthucdt']) ? trim($ActiveSheet->getCell($arrFields2['hinhthucdt'].$r)->getValue()) : 'CQ',
                'sohieu' => $sohieu,
                'sovaoso' => isset($arrFields2['sovaoso']) ? trim($ActiveSheet->getCell($arrFields2['sovaoso'].$r)->getValue()) : '',
                'ngaycapvb' => isset($arrFields2['ngaycapvb']) ? trim($ActiveSheet->getCell($arrFields2['ngaycapvb'].$r)->getValue()) : NULL,
                'soquyetdinh' => $fields['soqd'],
                'hedt' => $sv->hedt,
                'masv' => $masv,
                'tgcapnhat' => date('Y-m-d H:i:s'),
            );
            $checkid = db_select('uit_vanbang','t')->fields('t',array('id'))->condition('sohieu',$sohieu)->execute()->fetchField();
            try {
                if($checkid) {
                    db_update('uit_vanbang')->fields($fields_vb)->condition('id',$checkid)->execute();
                } else {
                    db_insert('uit_vanbang')->fields($fields_vb)->execute();
                }
            } catch (Exception $exc) {
                drupal_set_message('Có lỗi phát sinh ('.$exc->getMessage().')','error');
            }
        }
        
        if(isset($arrFields['tinhtrang']) ) {
            $fields['tinhtrang'] = (int)$ActiveSheet->getCell($arrFields['tinhtrang'].$r)->getValue();
            if(!isset($arrTinhTrang[$fields['tinhtrang']])) {
                drupal_set_message('Tình trạng không hợp lệ (dòng '.$r.', tình trang = '.$fields['tinhtrang'].')', 'error');
                continue;
            }
            $fields['giatricu'] = $sv->tinhtrang;
            $fields['giatrimoi'] = $fields['tinhtrang'];
            $check = db_select('uit_lichsu_tinhtrang_hocvien','t')->fields('t',array('id'))
                    ->condition('mahv',$masv)->condition('tinhtrang',$fields['tinhtrang'])->condition('namhoc',$fields['namhoc'])->execute()->fetchField();
            try {
                if($check) {
                    unset($fields['giatricu']);
                    unset($fields['giatrimoi']);
                    db_update('uit_lichsu_tinhtrang_hocvien')->fields($fields)->condition('id',$check)->execute();
                    $updated++;
                } else {
                    db_insert('uit_lichsu_tinhtrang_hocvien')->fields($fields)->execute();
                    db_update('uit_thongtinhv')->fields(array('tinhtrang' => $fields['tinhtrang']))->condition('mahv', $masv)->execute();
                    $suss++;
                }
            } catch (Exception $exc) {
                drupal_set_message('Dòng '.$r.' cập nhật bị lỗi ('.$exc->getMessage().')', 'error');
                $err++;
            }
            continue;
        }
    }
    
    if($err > 0) {
        drupal_set_message('Có '.$err.' dòng import không thành công', 'error');
    }
    if($suss > 0) {
        drupal_set_message('Có '.$suss.' dòng import thành công ('.basename($filepath).')');
        ghilogs('Import trạng thái sinh viên ('.$suss.' dòng thành công)', basename($filepath), 'qlhocvien');
    }
    if($updated > 0) {
        drupal_set_message('Có '.$updated.' dòng cập nhật thành công ('.basename($filepath).')');
    }
    if($err > 0 && $suss == 0) {
        drupal_set_message('Import không thành công', 'error');
    }
    
    drupal_goto('admin/qlhocvien/import/thongtin');
}

function importhinhanh($filepath) {
    $fileinfo = pathinfo($filepath);
    $suss = 0;
    if($fileinfo['extension'] == 'zip') {
        $zip = new ZipArchive;
        if ($zip->open($filepath) === true) {
            for($i = 0; $i < $zip->numFiles; $i++) {
                $filename = $zip->getNameIndex($i);
                if(preg_match('/([0-9]){2}52([0-9]){4}(.jpg)/', $filename) != 1) {
                    drupal_set_message('Tên file không hợp lệ ('.$filename.')', 'error');
                    continue;
                }
                $fileinfo1 = pathinfo($filename);
                $foldername = variable_get('QLSV_THUMUCANHSV').'/20'.substr($filename, 0, 2);
                if(!copy("zip://".drupal_realpath($filepath)."#".$fileinfo1['basename'], $foldername.'/'.$fileinfo1['basename'])) {
                    drupal_set_message('Copy file bị lỗi ('.$fileinfo1['basename'].')', 'error');
                } else {
                    $suss++;
                }
            }
            $zip->close();
        }
    } else {
        $filename = $fileinfo['basename'];
        if(preg_match('/([0-9]){2}52([0-9]){4}(.jpg)/', $filename) != 1) {
            drupal_set_message('Tên file không hợp lệ ('.$filename.')', 'error');
//            continue;
        }
        $foldername = variable_get('QLSV_THUMUCANHSV').'/20'.substr($filename, 0, 2);
        if(!file_exists($foldername)) md($foldername);
        if(!copy($filepath, $foldername.'/'.$filename)) {
            drupal_set_message('Copy file bị lỗi ('.$filename.')', 'error');
        } else {
            $suss++;
        }
    }

    if($suss > 0) {
        drupal_set_message('Upload thành công '.$suss.' hình sinh viên');
    }
    drupal_goto('admin/qlhocvien/import/hinhanh');
}

function import_hocvien($filepath, $AllowUpdate) {
    global $user;
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $objPHPExcel = PHPExcel_IOFactory::load($filepath);
    $ActiveSheet = $objPHPExcel->setActiveSheetIndex('0');
    //$connect_db = Database::getConnectionInfo();
    //$rs = db_query("SELECT COLUMN_NAME as cname FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = 'uit_thongtinsv' AND TABLE_SCHEMA = '" . $connect_db['default']['database'] . "'");
    //$arrFieldsDB = $rs->fetchCol();
    $arrFieldsDB = db_select('uit_formfield','t')->fields('t', array('field', 'defaultvalue'))->condition('tablename','uit_thongtinhv')->execute()->fetchAllKeyed();
    //$arrFieldsDB['matkhau'] = 'matkhau';
    $arrFields = array();
    $col_masv = NULL;
    for($c='A';$c <= $ActiveSheet->getHighestColumn() || strlen($c) < strlen($ActiveSheet->getHighestColumn());$c++) {
        $fieldname = $ActiveSheet->getCell($c.'1')->getValue();
        if(array_key_exists($fieldname, $arrFieldsDB)) {
            $arrFields[$fieldname] = $c;
            if($fieldname == 'mahv') $col_masv = $c;
        }
    }
    if(count($arrFields) <= 1) {
        drupal_set_message('Thông tin file import không hợp lệ.', 'error');
        return;
    }
    $arrDateFields = array('ngaysinh','cmnd_ngaycap','ngayvaodoan','ngayvaodang');
    $arrRequiredFields = array('hoten'=>NULL,'gioitinh'=>NULL,'khoahoc'=>NULL,'chuyennganh'=>NULL,'hedt'=>NULL);
    $suss = 0;
    $err = 0;
    for($r = 2; $r <= $ActiveSheet->getHighestRow(); $r++) {
        $masv = trim($ActiveSheet->getCell($col_masv.$r)->getValue());
        //if(strlen($masv) == 7 && is_numeric($masv) ) $masv = '0'.$masv;
        if(!isset($masv) || strlen($masv) < 6) continue;
        $isUpdate = FALSE;
        $StudentID = db_select('uit_thongtinhv', 't')->fields('t', array('id'))->condition('mahv', $masv)->execute()->fetchField();
        if($StudentID) {
            if(!$AllowUpdate) {
                drupal_set_message('MSHV '.$masv.' (dòng '.$r.') đã tồn tại.');
                continue;
            } else {
                $isUpdate = TRUE;
            }
        }
        $fields = array();
        foreach ($arrFields as $key => $col) {
            if($key == 'mahv') {
                if(!$isUpdate) {
                    $fields[$key] = $masv;
                }
            }
            $val = $ActiveSheet->getCell($col.$r)->getValue();
            if(is_bool($val)) {
                if($val) $val = 1;  else $val = 0;
            } else {
                $val = trim($val);
            }
            if(in_array($key, $arrDateFields)) {
                if($val != ''){
                    $tmp = PHPExcel_Style_NumberFormat::toFormattedString($val, "dd/mm/yyyy");
                    $val = ConvertToDate($tmp);
                }
            }
            if(array_key_exists($key, $arrRequiredFields)) {
                $arrRequiredFields[$key] = $val;
            }
            $fields[$key] = $val;
        }
        $cont = TRUE;
        if(!$isUpdate) {
            foreach ($arrRequiredFields as $key => $value) {
                if(!isset($value)) {
                    drupal_set_message('Dòng '.$r.' không thêm mới được do thiếu cột yêu cầu ('.$key.')','error');
                    $cont = FALSE;
                    break;
                }
            }
        }
        foreach($fields as $key=>$value){   // xóa các dòng trống
            if($value==''){
                unset($fields[$key]);
            }
        }
        if($cont){
            try {
                if($isUpdate) {
                    if(!in_array('administrator', $user->roles)) {
                        if((int)substr($masv, 0, 2) >= 14) {
                            unset($fields['coso']);
                            unset($fields['hedt']);
                            unset($fields['tinhtrang']);
                            unset($fields['chuyennganh']);
                        }
                    }
                    if(count($fields) == 0) continue;
                    db_update('uit_thongtinhv')->fields($fields)->condition('mahv',$masv)->execute();
                } else {
                    db_insert('uit_thongtinhv')->fields($fields)->execute();
                }
//                dvm($isUpdate);
//                dvm($fields);
                $suss++;
            } catch (Exception $exc) {
                drupal_set_message('Dòng '.$r.' cập nhật bị lỗi ('.$exc->getMessage().')','error');
                $err++;
            }
        }
    }
    if($err > 0) {
        drupal_set_message('Có '.$err.' dòng import không thành công', 'error');
    }
    if($suss > 0) {
        drupal_set_message('Có '.$suss.' dòng import thành công ('.basename($filepath).')');
        ghilogs('Import sinh viên ('.$suss.' dòng thành công)', basename($filepath), 'qlhocvien');
    }
    if($err > 0 && $suss == 0) {
        drupal_set_message('Import không thành công', 'error');
    }
    drupal_goto('admin/qlhocvien/import/hocvien');
}

function qlhocvien_import_template($importform) {
    $html = '';
    switch ($importform) {
        case 'hocvien':
            $html = l(' (File mẫu)',  drupal_get_path('module', 'qlhocvien').'/templates/Import_HV.xlsx');
            break;
        case 'thongtin':
            $html = l(' (File mẫu)',drupal_get_path('module', 'qlhocvien').'/templates/CapNhat_ThongTinHV.xlsx');
            break;

        default:
            break;
    }

    return $html;
}