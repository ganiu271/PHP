<?php
function qldiem_form_import($form, &$form_state) {
    if(!user_access('Import điểm') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $form['wraper'] = array(
        '#type' => 'fieldset',
        '#title' => t('Chọn thông tin'),
        '#collapsible' => TRUE,
//        '#description' => t("Chọn các thông tin liên quan."),
    );
    $ctdt = isset($form_state['values']['ctdt']) ? $form_state['values']['ctdt']:NULL;
    if(!isset($ctdt) && isset($_SESSION['importdiem']['ctdt']) && $_SESSION['importdiem']['ctdt'] != ''){
        $ctdt = $_SESSION['importdiem']['ctdt'];
    }
    $arrMonHoc = array('' => t('Chọn CTĐT trước'));
    if($ctdt) {
        $arrMonHoc = array('' => t('Chọn môn học'));
        $q = db_select('uit_ctdaotao','t')->distinct();
        $q->join('uit_monhoc','mh','t.mamh=mh.mamh');
        $q->fields('t')->fields('mh');
        $q->condition ('mactdt', $ctdt);
        $q->orderBy('t.mamh');
        $rows = $q->execute()->fetchAll();
        foreach ($rows as $row) {
            $arrMonHoc[$row->mamh] = $row->mamh.' - '.$row->tenmh;
        }
    }
    $form['wraper']['ctdt'] = array(
        '#type' => 'select',
        '#title' => t('Chương trình đào tạo'),
        '#options' => (array('' => t('Chọn CTĐT')) + db_select('uit_quanly_ctdt', 'd')->orderBy('d.ma_ctdaotao')
                                ->fields('d', array('ma_ctdaotao', 'ten_ctdaotao'))->execute()->fetchAllKeyed()),
        '#default_value' => (isset($_SESSION['importdiem']['ctdt']) ? $_SESSION['importdiem']['ctdt']:''),
        '#ajax' => array(
            'callback' => 'ajax_filter_importdiem_callback',
            'wrapper' => 'wrapper_mamh',
        ),
    );
    $form['wraper']['mamh'] = array(
        '#type' => 'select',
        '#title' => t('Môn học').' thuộc : '.$ctdt,
        '#prefix' => '<div id="wrapper_mamh">',
        '#suffix' => '</div>',
        '#options' => $arrMonHoc,
    );
    $form['wraper']['khoahoc'] = array(
        '#type' => 'select',
        '#title' => t('Khóa'),
        '#options' =>(array('' => t('Chọn khóa')) + db_select('uit_thongtinhv', 'k')->distinct()->orderBy('khoahoc')->fields('k', array('khoahoc','khoahoc'))->execute()->fetchAllKeyed()),
        '#default_value' => '',
    );
    $form['wraper']['lanthi'] = array(
        '#type' => 'select',
        '#title' => t('Lần thi'),
        '#options' => array('1' => 1,'2' => 2,'3' => 3),
        '#default_value' => 1,
    );
    $form['wraper']['cotmahv'] = array(
        '#type' => 'select',
        '#title' => t('Cột chứa mã HV'),
        '#options' => array('A'=>'A','B'=>'B','C'=>'C','D'=>'D'),
        '#default_value' => 'B',
    );
    $form['wraper']['cotbd'] = array(
        '#type' => 'select',
        '#title' => t('Cột điểm bắt đầu'),
        '#options' => array('E'=>'E','F'=>'F','G'=>'G','H'=>'H','I'=>'I','J'=>'J'),
        '#default_value' => 'G',
    );
    $form['wraper']['dongbd'] = array(
        '#type' => 'select',
        '#title' => t('Dòng điểm bắt đầu'),
        '#options' => array(12=>12,13=>13,14=>14,15=>15,16=>16),
        '#default_value' =>12,
    );
    $form['chocapnhat'] = array(
        '#title'=>t('Cho phép cập nhật điểm nếu trùng ?'),
        '#type'=>'checkbox',
        '#default_value'=> (isset($_SESSION['importdiem']['chocapnhat']) ? $_SESSION['importdiem']['chocapnhat']:0),
    );
    $form['browser'] = array(
      '#type' => 'fieldset',
      '#title' => t('Import điểm'),
      '#collapsible' => TRUE,
      '#description' => "Upload file điểm (xls, xlsx). File import có dữ liệu điểm mặc định bắt đầu từ dòng 12. Cột mahv: B; cột điểm: G",
    );
    $form['browser']['file_upload'] = array(
      '#type' => 'file',
      '#title' => l(' File mẫu',drupal_get_path('module', 'qldiem').'/templates/MauBangDiemMonHoc.xls'),
      '#size' => 40,
      '#description' => t('Chọn file excel để import. ') . t('Maximum file size: !size MB.', array('!size' => file_upload_max_size()/1024/1024)),
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Import'),
    );
    $form['cancel'] = array(
        '#type' => 'button',
        '#value' => t('Cancel'),
        '#validate' => FALSE,
        '#executes_submit_callback' => TRUE,
        '#submit' => array('cancel_request'),
    );
    $form['#attributes']['enctype'] = "multipart/form-data";
    return $form;
}

function qldiem_form_import_validate($form, &$form_state) {

    if($form_state['values']['khoahoc'] == '') {
        form_set_error('khoahoc', t('Phải chọn khóa học.'));
        return;
    }
    if($form_state['values']['ctdt'] == '') {
        form_set_error('ctdt', t('Phải chọn CTĐT.'));
        return;
    }
    if($form_state['values']['mamh'] == '') {
        form_set_error('mamh', t('Phải chọn môn học.'));
        return;
    }
    if($form_state['values']['lanthi']==''){
        form_set_error('lanthi', t('Phải chọn lần thi'));
        return;
    }

    // attempt to save the uploaded file
    $filepath = 'public://uploads/qldiem/'.date('Ym');
    if(!file_exists($filepath)) mkdir($filepath);
    $file = file_save_upload('file_upload', array('file_validate_extensions'=>array('xls xlsx')), $filepath, FILE_EXISTS_RENAME);
    if (!$file) { 
      form_set_error('file_upload', t('Upload bị lỗi.'));
    }
    $form_state['values']['file_upload'] = $file;  
}

function qldiem_form_import_submit($form, &$form_state) {
    $file = $form_state['values']['file_upload'];
    $filepath = 'sites/default/files/'.str_replace('public://', '', $file->uri);
    if(!file_exists($filepath)) return;
//    $coso = $form_state['values']['coso'];
//    $_SESSION['importdiem']['coso'] = $coso;
    $khoahoc = $form_state['values']['khoahoc'];
    $ctdt = $form_state['values']['ctdt'];
    $mamh = $form_state['values']['mamh'];
    $hocphan = $form_state['values']['hocphan'];
    $lanthi = $form_state['values']['lanthi'];
    $cotmahv = $form_state['values']['cotmahv'];
    $cotbd = $form_state['values']['cotbd'];
    $dongbd = $form_state['values']['dongbd'];
    $chocapnhat=$form_state['values']['chocapnhat'];
    $_SESSION['importdiem']['ctdt'] = $ctdt;
    $_SESSION['importdiem']['khoahoc'] = $khoahoc;
    $_SESSION['importdiem']['mamh'] = $mamh;
    $_SESSION['importdiem']['lanthi'] = $lanthi;
    
    $sotchk = db_select('uit_ctdaotao','t')->fields('t',array('sotc','hocky'))
        ->condition('mamh',$mamh)->condition('mactdt',$ctdt)->execute()->fetch();
    if(!$sotchk) return;
    $sotc = $sotchk->sotc;
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $objPHPExcel = PHPExcel_IOFactory::load($filepath);
    $ActiveSheet = $objPHPExcel->setActiveSheetIndex('0');
    //Lấy thông tin tên lớp
    $malop = $mamh.".".$lanthi;
    if($khoahoc >0 && $khoahoc<10){
        $malop="CH0".$khoahoc.".".$malop;
    }else{
        $malop="CH".$khoahoc.".".$malop;
    }
    $errNo = 0;
    $numrow = 0;
    for($r = $dongbd; $r <= $ActiveSheet->getHighestRow(); $r++) {
        $mahv = trim($ActiveSheet->getCell($cotmahv.$r)->getValue());
        if($mahv == '' || strlen($mahv) > 9) continue;
        if(strlen($mahv) != 8) {
            drupal_set_message('Dòng '.$r.' có MSHV không hợp lệ','error');
            $errNo++;
            continue;
        }
        $diemhp = $ActiveSheet->getCell($cotbd.$r)->getOldCalculatedValue();
        if(!isset($diemhp) || !is_numeric($diemhp)) {
            $diemhp = (float)$ActiveSheet->getCell($cotbd.$r)->getCalculatedValue();
        }
        if(!isset($diemhp) || !is_numeric($diemhp)) {
            $diemhp = (float)$ActiveSheet->getCell($cotbd.$r)->getValue();
        }
        
        if($diemhp < 0 || $diemhp > 10) {
            drupal_set_message('Dòng '.$r.' điểm không hợp lệ (0=>10)','error');
            $errNo++;
            continue;
        }
        $idcheck = db_select('uit_diem','t')->fields('t',array('id'))->condition('mahv',$mahv)->condition('mamh',$mamh)
            ->condition('malop',$malop)->execute()->fetchField();
        try {
            if($idcheck && $chocapnhat=='1') {
                db_update('uit_diem')->fields(array(
                    'sotc' => $sotc,
//                    'nganh' => $ctdt,
//                    'hocky' => $hocky,
                    'diem' => $diemhp,
                    'thoigiancapnhat' => date('Y-m-d h:i:s'),
                ))->condition('id',$idcheck)->execute();
            } else {
                db_insert('uit_diem')->fields(array(
                    'mahv' => $mahv,
                    'mamh' => $mamh,
                    'malop' => $malop,
                    'sotc' => $sotc,
                    'diem' => $diemhp,
////                    'nganh' => $ctdt,
////                    'hocky' => $hocky,
////                    'lanthi' => $lanthi,
//                    'thoigiancapnhat' => date('Y-m-d h:i:s'),
                ))->execute();
            }
            CapNhatTrangThai($mahv, $mamh);
            $numrow++;
        } catch (Exception $exc) {
            drupal_set_message('Dòng '.$r.' ('.$mahv.') bị lỗi ('.$exc->getMessage().')','error');
            $errNo++;
        }
//        dvm($mahv."     ".$mamh."-".$malop."    ".$diemhp);
    }
    if($errNo > 0) {
        drupal_set_message('Số dòng import không thành công: '.$errNo, 'error');
    }
    if($numrow > 0) {
//        ghilogs('Import điểm: malop='.$malop, drupal_basename($filepath));
        drupal_set_message('Số dòng import thành công: '.$numrow);
    }
    return ;
}

function cancel_request() {
    drupal_goto('admin/qldiem/quanly');
}

function ajax_filter_importdiem_callback($form, &$form_state) {
    $_SESSION['importdiem']['ctdt'] = $form_state['values']['ctdt'];
    return $form['wraper']['mamh'];
}

//function qldiem_form_importth($form, &$form_state) {
//    if(!user_access('Import điểm') ) {
//        drupal_set_message('Bạn không được phép truy cập menu này','error');
//        return;
//    }
//    $form = array();
//    $form['wraper'] = array(
//        '#prefix' => '<div id="info_importdiem">',
//        '#suffix' => '</div>',);
//    $form['wraper']['info'] = array(
//        '#type' => 'fieldset',
//        '#title' => t('Chọn thông tin'),
//        '#collapsible' => TRUE,
//        '#prefix' => '<div id="info_importdiem">',
//        '#suffix' => '</div>',
//      //'#description' => t("Chọn các thông tin liên quan."),
//    );
//    $form['wraper']['info']['coso'] = array(
//        '#type' => 'select',
//        '#title' => t('Cơ sở'),
//        '#options' => (array('' => t('Chọn cơ sở')) + db_select('uit_coso', 'd')->orderBy('d.macs')->fields('d', array('macs', 'tencs'))->execute()->fetchAllKeyed()),
//        '#default_value' => (isset($_SESSION['importdiem']['coso']) ? $_SESSION['importdiem']['coso']:''),
//    );
//    $form['wraper']['info']['ctdt'] = array(
//        '#type' => 'select',
//        '#title' => t('Chương trình đào tạo'),
//        '#options' => (array('' => t('Chọn CTĐT')) + db_select('uit_nganh', 'd')->orderBy('d.nganh')
//                ->fields('d', array('nganh', 'tennganh'))->execute()->fetchAllKeyed()),
//        '#default_value' => (isset($_SESSION['importdiem']['ctdt']) ? $_SESSION['importdiem']['ctdt']:''),
//    );
//    $arrKhoaHoc = array(''=>'Chọn khóa học')+db_select('uit_quanly_ctdt','ctdt')
//            ->orderBy('ctdt.khoa_daotao')->fields('ctdt',array('khoa_daotao','khoa_daotao'))->execute()->fetchAllKeyed();
////    for($i=2001; $i<date('Y'); $i++) $arrKhoaHoc[$i] = $i;
//    $form['wraper']['info']['khoahoc'] = array(
//        '#type' => 'select',
//        '#title' => t('Khóa'),
//        '#options' => $arrKhoaHoc,
//        '#default_value' => (isset($_SESSION['importdiem']['khoahoc']) ? $_SESSION['importdiem']['khoahoc']:''),
//    );
//    $form['wraper']['info']['cotmahv'] = array(
//        '#type' => 'select',
//        '#title' => t('Cột chứa mã hv'),
//        '#options' => array('A'=>'A','B'=>'B','C'=>'C','D'=>'D'),
//        '#default_value' => (isset($_SESSION['importdiem']['cotmahv']) ? $_SESSION['importdiem']['cotmahv']:'B'),
//    );
//    $form['wraper']['info']['cotbd'] = array(
//        '#type' => 'select',
//        '#title' => t('Cột điểm bắt đầu'),
//        '#options' => array('E'=>'E','F'=>'F','G'=>'G','H'=>'H','I'=>'I','J'=>'J'),
//        '#default_value' => (isset($_SESSION['importdiem']['cotbd']) ? $_SESSION['importdiem']['cotbd']:'F'),
//    );
//    $form['wraper']['info']['dongbd'] = array(
//        '#type' => 'select',
//        '#title' => t('Dòng điểm bắt đầu'),
//        '#options' => array(6=>6,7=>7,8=>8,9=>9),
//        '#default_value' => (isset($_SESSION['importdiem']['dongbd']) ? $_SESSION['importdiem']['dongbd']:7),
//    );
//    $form['wraper']['info']['chocapnhat'] = array(
//        '#title'=>t('Cho phép cập nhật điểm nếu trùng ?'),
//        '#type'=>'checkbox',
//        '#default_value'=> (isset($_SESSION['importdiem']['chocapnhat']) ? $_SESSION['importdiem']['chocapnhat']:0),
//    );
//
//    $form['browser'] = array(
//      '#type' => 'fieldset',
//      '#title' => t('Import điểm'),
//      '#collapsible' => TRUE,
//      '#description' => "Upload file điểm tổng hợp (xls, xlsx).",
//    );
//    $file_size = t('Maximum file size: !size MB.', array('!size' => file_upload_max_size()/1024/1024));
//    $form['browser']['file_upload'] = array(
//      '#type' => 'file',
//      '#title' => t('Excel File ('.l('File mẫu',drupal_get_path('module', 'qldiem').'/templates/Import_BangDiemTongHop.xls').')'),
//      '#size' => 40,
//      '#description' => t('Chọn file excel để import. ') . $file_size,
//    );
//
//    $form['submit'] = array(
//        '#type' => 'submit',
//        '#value' => t('Import'),
//    );
//    $form['cancel'] = array(
//        '#type' => 'button',
//        '#value' => t('Cancel'),
//        '#validate' => FALSE,
//        '#executes_submit_callback' => TRUE,
//        '#submit' => array('cancel_request'),
//    );
//
//    // set the form encoding type
//    $form['#attributes']['enctype'] = "multipart/form-data";
//    return $form;
//}
//
//function qldiem_form_importth_validate($form, &$form_state) {
//    if($form_state['values']['coso'] == '') {
//        form_set_error('coso', t('Phải chọn cơ sở.'));
//        return;
//    }
//    if($form_state['values']['ctdt'] == '') {
//        form_set_error('ctdt', t('Phải chọn CTĐT.'));
//        return;
//    }
//    // attempt to save the uploaded file
//    $filepath = 'public://uploads/qldiem/'.date('Ym');
//    if(!file_exists($filepath)) mkdir($filepath);
//    $file = file_save_upload('file_upload', array('file_validate_extensions'=>array('xls xlsx')), $filepath, FILE_EXISTS_REPLACE);
//    // check file uploaded OK
//    if (!$file) {
//      form_set_error('file_upload', t('Upload bị lỗi.'));
//    }
//    $form_state['values']['file_upload'] = $file;
//}
//
//function qldiem_form_importth_submit($form, &$form_state) {
//    $file = $form_state['values']['file_upload'];
//    $filepath = 'sites/default/files/'.str_replace('public://', '', $file->uri);
//    if(!file_exists($filepath)) return;
//
//    $cotmahv = $form_state['values']['cotmahv'];
//    $coso = $form_state['values']['coso'];
//    $ctdt = $form_state['values']['ctdt'];
//    $khoahoc = $form_state['values']['khoahoc'];
//    $cotbd = $form_state['values']['cotbd'];
//    $dongbd = $form_state['values']['dongbd'];
//    $chocapnhat = $form_state['values']['chocapnhat'];
//
//    $_SESSION['importdiem']['coso'] = $coso;
//    $_SESSION['importdiem']['cotmahv'] = $cotmahv;
//    $_SESSION['importdiem']['ctdt'] = $ctdt;
//    $_SESSION['importdiem']['khoahoc'] = $khoahoc;
//    $_SESSION['importdiem']['cotbd'] = $cotbd;
//    $_SESSION['importdiem']['dongbd'] = $dongbd;
//    $_SESSION['importdiem']['chocapnhat'] = $chocapnhat;
//
//    $rows = db_select('uit_ctdaotao','t')->fields('t',array('mamh','sotc','hocky'))->condition('nganh',$ctdt)->execute()->fetchAll();
//    if(!$rows) return;
//    $ctdt_mh = array();
//    foreach ($rows as $row) {
//        $ctdt_mh[$row->mamh] = array('sotc'=>$row->sotc,'hocky'=>$row->hocky);
//    }
//
//    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
//    $objPHPExcel = PHPExcel_IOFactory::load($filepath);
//    $ActiveSheet = $objPHPExcel->setActiveSheetIndex('0');
//
//    $arrMonHoc = array();
//    for($col = $cotbd; $col <= $ActiveSheet->getHighestColumn() || strlen($col) < strlen($ActiveSheet->getHighestColumn()); $col++) {
//        $mamh = trim($ActiveSheet->getCell($col.($dongbd-1))->getValue());
//        if($mamh == '') continue;
//        if(!isset($ctdt_mh[$mamh])) {
//            drupal_set_message('Cột '.$col.' có mã môn học không hợp lệ, vui lòng điều chỉnh và import lại.','error');
//            return;
//        }
//        $arrMonHoc[$mamh] = $col;
//    }
//    if(count($arrMonHoc) == 0) {
//        drupal_set_message('Không có thông tin để import.');
//        return;
//    }
//    $strLogs = '';
//    for($r = $dongbd; $r <= $ActiveSheet->getHighestRow(); $r++) {
//        $mahv = trim($ActiveSheet->getCell($cotmahv.$r)->getValue());
//        if($mahv == '') continue;
//        if(is_numeric($mahv) && strlen($mahv) == 7) $mahv = '0'.$mahv;
//        if(strlen($mahv) != 8) {
//            drupal_set_message('Dòng '.$r.' có MSHV không hợp lệ','error');
//            continue;
//        }
//        $checkhv = db_select('uit_thongtinhv','t')->fields('t',array('id'))->condition('mahv',$mahv)->execute()->fetch();
//        if(!$checkhv) {
//            drupal_set_message('Dòng '.$r.' có MShv chưa có trên hệ thống ('.$mahv.')','error');
//            continue;
//        }
//        //$strLogs .= '<p>Dòng '.$r.' ('.$mahv.') import thành công các môn: ';
//        $strtmp = '';
//        foreach ($arrMonHoc as $mamh => $col) {
//            $diem = $ActiveSheet->getCell($col.$r)->getValue();
//            if($diem == '') continue;
//            $lanthi = 1;
//            $color = $ActiveSheet->getStyle($col.$r)->getFont()->getColor()->getRGB();
//            if($color == 'FF0000') $lanthi = 3;
//            if($color == '0000FF') $lanthi = 2;
//            if($diem < 0 || $diem > 10) {
//                drupal_set_message('Dòng '.$r.', môn '.$mamh.' có điểm không hợp lệ ('.$diem.')','error');
//                continue;
//            }
//            $malop = "$mamh.$ctdt.$coso.$khoahoc";
//            $idcheck = db_select('uit_diem','t')->fields('t',array('id'))->condition('mahv',$mahv)->condition('mamh',$mamh)
//                ->condition('malop',$malop)->execute()->fetchField();
//            try {
//                if($idcheck) {
//                    if(!$chocapnhat) {
//                        drupal_set_message('Điểm môn '.$mamh.' của HV '.$mahv.' đã có trên hệ thống','error');
//                        continue;
//                    } else {
////                        db_update('uit_diem')->fields(array(
////                            'sotc' => $ctdt_mh[$mamh]['sotc'],
////                            'nganh' => $ctdt,
////                            'hocky' => $ctdt_mh[$mamh]['hocky'],
////                            'diem' => $diem,
////                            'thoigiancapnhat' => date('Y-m-d h:i:s'),
////                        ))->condition('id',$idcheck)->execute();
//                        $strtmp .= $mamh.':'.$diem.';';
//                    }
//                } else {
////                    db_insert('uit_diem')->fields(array(
////                        'mahv' => $mahv,
////                        'mamh' => $mamh,
////                        'malop' => $malop,
////                        'sotc' => $ctdt_mh[$mamh]['sotc'],
////                        'nganh' => $ctdt,
////                        'hocky' => $ctdt_mh[$mamh]['hocky'],
////                        'diem' => $diem,
////                        'lanthi' => $lanthi,
////                        'thoigiancapnhat' => date('Y-m-d h:i:s'),
////                    ))->execute();
//                    $strtmp .= $mamh.':'.$diem.';';
//                }
//                dvm($idcheck);
//                dvm($mahv.$mamh.$malop.$ctdt_mh[$mamh]['sotc'].$ctdt.$ctdt_mh[$mamh]['hocky'].$diem.$lanthi);
//            } catch (Exception $exc) {
//                drupal_set_message('Điểm môn '.$mamh.' của HV '.$mahv.' bị lỗi ('.$exc->getMessage().')','error');
//            }
//        }
//        if($strtmp != '') $strLogs .= '<p>Dòng '.$r.' ('.$mahv.') import thành công các môn: '.$strtmp.'</p>';
//    }
//    if($strLogs != '') drupal_set_message ($strLogs);
//    return ;
//}
