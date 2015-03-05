<?php
function qlgiangvien_form_import($form, &$form_state, $importform) {
    global $base_url;
    if(!user_access('UIT qlgiangvien') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>IMPORT THÔNG TIN GIẢNG VIÊN</h1></center>';
    $form['markup'] = array('#markup' => $html);
    $options = array(
            '' => 'Chọn mẫu import',
            'giangvien' => 'Thêm / cập nhật giảng viên (xls, xlsx)',
            );
    
    $form['importform'] = array(
            '#title'=>t('Chọn mẫu import:'),
            '#type'=>'select',
            '#options'=>$options,
            '#default_value'=> (isset($importform) ? $importform:NULL),
            '#attributes' => array('onchange' => 'location.href=\''.$base_url.'/admin/qlgiangvien/import/\'+document.getElementById(\'edit-importform\').value;'),
    );
    if($importform == 'giangvien') {
        $form['chocapnhat'] = array(
            '#title'=>t('Cho phép cập nhật nếu trùng mã GV ?'),
            '#type'=>'checkbox',
            '#default_value'=> 1,
        );
    }
    
    $form['browser'] = array(
      '#type' => 'fieldset',
      '#title' => t('Import file'),
      '#collapsible' => TRUE,
      '#description' => t("Upload file.").qlgiangvien_import_template($importform),
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

function qlgiangvien_import_template($importform) {
    $html = '';
    switch ($importform) {
        case 'giangvien':
            $html = l(' (File mẫu)','sites/default/files/templates/qlgiangvien/Mau_import_GV.xlsx');
            break;
        
        default:
            break;
    }
    
    return $html;
}

function qlgiangvien_form_import_validate($form, &$form_state) {
    // attempt to save the uploaded file
    $importform = $form_state['values']['importform'];
    if($importform == '') {
        form_set_error('importform', t('Phải chọn mẫu import.'));
        return;
    }
    switch ($importform) {
        case 'hinhanh':
            break;

        default:
            $file = file_save_upload('file_upload', array('file_validate_extensions'=>array('xls xlsx')), 'public://uploads/qlgiangvien', FILE_EXISTS_REPLACE);
            break;
    }
    
    // check file uploaded OK
    if (!$file) { 
      form_set_error('file_upload', t('Upload bị lỗi.'));
    }
    $form_state['values']['file_upload'] = $file;  
}

function qlgiangvien_form_import_submit($form, &$form_state) {
    $file = $form_state['values']['file_upload'];
    $importform = $form_state['values']['importform'];
    $filepath = 'sites/default/files/uploads/qlgiangvien';
    if(!file_exists($filepath)) mkdir($filepath);
    $filepath .= '/'.$file->filename;
    switch ($importform) {
        case 'giangvien':
            $AllowUpdate = $form_state['values']['chocapnhat'];
            import_giangvien($filepath, $AllowUpdate);
            break;
        
        default:
            break;
    }
    
    return ;
}

function import_giangvien($filepath, $AllowUpdate) {
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $objPHPExcel = PHPExcel_IOFactory::load($filepath);
    $ActiveSheet = $objPHPExcel->setActiveSheetIndex('0');
    //$connect_db = Database::getConnectionInfo();
    //$rs = db_query("SELECT COLUMN_NAME as cname FROM INFORMATION_SCHEMA.COLUMNS WHERE table_name = 'uit_thongtingv' AND TABLE_SCHEMA = '" . $connect_db['default']['database'] . "'");
    //$arrFieldsDB = $rs->fetchCol();
    $rows = db_select('uit_formfield','t')->fields('t')->condition('tablename','uit_thongtingv')->execute()->fetchAll();
    $arrFieldsDB = array();
    $arrDateFields = array();
    $arrRequiredFields = array();
    foreach ($rows as $row) {
        $arrFieldsDB[$row->field] = $row->defaultvalue;
        if($row->valuetype == 'date') $arrDateFields[] = $row->field;
        if($row->required == 1) $arrRequiredFields[$row->field] = NULL;
    }
    $arrFields = array();
    $col_magv = NULL;
    for($c='A';$c <= $ActiveSheet->getHighestColumn() || strlen($c) < strlen($ActiveSheet->getHighestColumn());$c++) {
        $fieldname = trim($ActiveSheet->getCell($c.'1')->getValue());
        if(array_key_exists($fieldname, $arrFieldsDB)) {
            $arrFields[$fieldname] = $c;
            if($fieldname == 'magv') $col_magv = $c;
        }
    }
    //$arrDateFields = array('ngaysinh','cmnd_ngaycap','ngayvaodoan','ngayvaodang');
    $suss = 0;
    $err = 0;
    for($r = 2; $r <= $ActiveSheet->getHighestRow(); $r++) {
        $magv = trim($ActiveSheet->getCell($col_magv.$r)->getValue());
        if(!isset($magv) || !is_numeric($magv) || strlen($magv) != 5) continue;
        $isUpdate = FALSE;
        $StudentID = db_select('uit_thongtingv', 't')->fields('t', array('id'))->condition('magv', $magv)->execute()->fetchField();
        if($StudentID) {
            if(!$AllowUpdate) {
                drupal_set_message('MaGV '.$magv.' (dòng '.$r.') đã tồn tại.');
                continue;
            } else {
                $isUpdate = TRUE;
            }
        }
        //$arrRequiredFields = array('hoten'=>NULL,'gioitinh'=>NULL,'khoa'=>NULL,'khoahoc'=>NULL,'chuyennganh'=>NULL);
        $arrRequiredFields = array_fill_keys(array_keys($arrRequiredFields), NULL);
        $fields = array();
        foreach ($arrFields as $key => $col) {
            if($key == 'magv') {
                if(!$isUpdate) $fields[$key] = $magv;
                $arrRequiredFields[$key] = $magv;
                continue;
            }
            $val = $ActiveSheet->getCell($col.$r)->getValue();
            if(is_bool($val)) {
                if($val) $val = 1;  else $val = 0;
            } else {
                $val = trim($val);
            }
            if(in_array($key, $arrDateFields)) {
                $tmp = PHPExcel_Style_NumberFormat::toFormattedString($val, "dd/mm/yyyy");
                $val = ConvertToDate($tmp);
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
        if(!$cont) continue;
        
        try {
            if($isUpdate) {
                db_update('uit_thongtingv')->fields($fields)->condition('magv',$magv)->execute();
            } else {
                db_insert('uit_thongtingv')->fields($fields)->execute();
            }
            $suss++;
        } catch (Exception $exc) {
            drupal_set_message('Dòng '.$r.' cập nhật bị lỗi ('.$exc->getMessage().')');
            $err++;
        }

    }
    
    if($err > 0) {
        drupal_set_message('Có '.$err.' dòng import không thành công', 'error');
    }
    if($suss > 0) {
        drupal_set_message('Có '.$suss.' dòng import thành công');
        ghilogs('Import sinh viên ('.$suss.' dòng thành công)', basename($filepath), 'qlgiangvien');
    }
    if($err > 0 && $suss == 0) {
        drupal_set_message('Import không thành công', 'error');
    }
    
    drupal_goto('admin/qlgiangvien/import/giangvien');
}
