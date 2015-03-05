<?php
function qlmonhoc_import($form, &$form_state, $importform) {
    global $user, $base_url;
    if(!user_access('Quản lý môn học') ) {
        //if($user->uid == 0) drupal_goto('user/login', array('query' => drupal_get_destination()));
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>IMPORT FILES - DANH MỤC</h1></center>';
    $form['markup'] = array('#markup' => $html);
    $options = array(
            '' => 'Chọn mẫu import',
            'monhoc' => 'Môn học (xls, xlsx)',
            'monhoctd' => 'Môn học tương đương (xls, xlsx)',
            );
    
    $form['importform'] = array(
            '#title'=>t('Chọn mẫu import:'),
            '#type'=>'select',
            '#options'=>$options,
            '#default_value'=> (isset($importform) ? $importform:NULL),
//        '#attributes' => array('onchange' => 'location.href=\''.$base_url.'/admin/qlmonhoc/import/\'+document.getElementById(\'edit-importform\').value;'),
            '#attributes' => array('onchange' => 'location.href=\''.$base_url.'/?q=admin/qlmonhoc/import/\'+document.getElementById(\'edit-importform\').value;'),
        ); 
    $form['browser'] = array(
      '#type' => 'fieldset',
      '#title' => t('Import file'),
      '#collapsible' => TRUE,
      '#description' => t("Upload file").qlmonhoc_import_template($importform),
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

function qlmonhoc_import_template($importform) {
    $html = '';
    switch ($importform) {
        case 'monhoc':
            $html= l('  File mẫu',drupal_get_path('module','qlmonhoc').'/templates/Import_MonHoc.xlsx');
            break;
        case 'monhoctd':
            $html= l('  File mẫu',drupal_get_path('module','qlmonhoc').'/templates/Import_MonHocTD.xlsx');
            break;
        
        default:
            break;
    }
    
    return $html;
}

function qlmonhoc_import_validate($form, &$form_state) {
    // attempt to save the uploaded file
    $importform = $form_state['values']['importform'];
    if($importform == '') {
        form_set_error('importform', t('Phải chọn mẫu import.'));
        return;
    }
    $file = file_save_upload('file_upload', array('file_validate_extensions'=>array('xls xlsx')), 'public://uploads', FILE_EXISTS_REPLACE);
    // check file uploaded OK
    if (!$file) { 
      form_set_error('file_upload', t('Upload bị lỗi.'));
    }
    $form_state['values']['file_upload'] = $file;  
}

function qlmonhoc_import_submit($form, &$form_state) {
    $file = $form_state['values']['file_upload'];
    $importform = $form_state['values']['importform'];
    $filepath = 'sites/default/files/uploads';
    if(!file_exists($filepath)) mkdir($filepath);
    $filepath .= '/'.$file->filename;
    switch ($importform) {
        case 'monhoc':
            importmonhoc($filepath);
            break;
        case 'monhoctd':
            importmonhoctd($filepath);
            break;
        
        default:
            break;
    }
    
    return ;
}


function importmonhoc($filepath) {
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $objPHPExcel = PHPExcel_IOFactory::load($filepath);
    $ActiveSheet = $objPHPExcel->setActiveSheetIndex('0');
    $succ = 0;
    $err = 0;
    for($r = 2; $r <= $ActiveSheet->getHighestRow(); $r++) {
        $data = array();
        $data['mamh'] = trim($ActiveSheet->getCell('A'.$r)->getValue());
        $data['tenmh'] = trim($ActiveSheet->getCell('B'.$r)->getValue());
        if($data['mamh'] == '' || $data['tenmh'] == '') continue;
        $data['tenta'] = trim($ActiveSheet->getCell('C'.$r)->getValue());
        if($data['tenta'] == '') $data['tenta'] = $data['tenmh'];
        $data['loaimh'] = trim($ActiveSheet->getCell('D'.$r)->getValue());
        $data['sotc'] = (int)$ActiveSheet->getCell('E'.$r)->getValue();
        $data['sotc_lt'] = (int)$ActiveSheet->getCell('F'.$r)->getValue();
        $data['sotc_th'] = (int)$ActiveSheet->getCell('G'.$r)->getValue();
        $data['hedt'] = trim($ActiveSheet->getCell('H'.$r)->getValue());
        $data['khoaql'] = trim($ActiveSheet->getCell('I'.$r)->getValue());
//        $data['hinhthucth'] = (int)$ActiveSheet->getCell('J'.$r)->getValue();
        $data['mamhcu'] = trim($ActiveSheet->getCell('K'.$r)->getValue());
        $data['ghichu'] = trim($ActiveSheet->getCell('L'.$r)->getValue());
        if($data['sotc'] < 0 || $data['sotc_lt'] < 0 || $data['sotc_th'] < 0 || $data['sotc'] != $data['sotc_lt']+$data['sotc_th']) {
            drupal_set_message('Dòng '.$r.' bị lỗi (số tín chỉ không hợp lệ)');
            $err++;
            continue;
        }
        $rid = db_select('uit_monhoc', 't')->fields('t', array('id'))->condition('mamh', $data['mamh'])->execute()->fetchField();
//        dvm($data);
        try {
            if($rid) { //Update
                db_update('uit_monhoc')->fields($data)->condition('id', $rid)->execute();
            } else { //Insert
                db_insert('uit_monhoc')->fields($data)->execute();
            }
            $succ++;
        } catch (Exception $exc) {
            drupal_set_message('Dòng '.$r.' bị lỗi ('.$exc->getMessage().')');
            $err++;
        }
    }
    if($succ > 0) {
        drupal_set_message('Có '.$succ.' dòng import thành công');
        ghilogs('Import môn học ('.$succ.' dòng thành công)', basename($filepath), 'qldanhmuc');
    }
    if($err > 0 && $succ == 0) {
        drupal_set_message('Import không thành công', 'error');
        return;
    }
    if($err > 0) {
        drupal_set_message('Có '.$err.' dòng import không thành công', 'error');
    }
    drupal_goto('admin/qlmonhoc/import/monhoc');
}

function importmonhoctd($filepath) {
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
//    require_once '/var/www/html/qldt/sites/all/libraries/PHPExcel/IOFactory.php';
    $objPHPExcel = PHPExcel_IOFactory::load($filepath);
    $ActiveSheet = $objPHPExcel->setActiveSheetIndex('0');
    $succ = 0;
    $err = 0;
    for($r = 2; $r <= $ActiveSheet->getHighestRow(); $r++) {
        $data = array();
        $data['mamh1'] = trim($ActiveSheet->getCell('A'.$r)->getValue());
        $data['mamh2'] = trim($ActiveSheet->getCell('B'.$r)->getValue());
        if($data['mamh1'] == '' || $data['mamh2'] == '') continue;
        $data['hedt'] = trim($ActiveSheet->getCell('C'.$r)->getValue());
        $data['ghichu'] = trim($ActiveSheet->getCell('D'.$r)->getValue());
        
        $rid = db_select('uit_monhoctd', 't')->fields('t', array('id'))
                ->condition('mamh1', $data['mamh1'])->condition('mamh2', $data['mamh2'])->condition('hedt', $data['hedt'])
                ->execute()->fetchField();
        if($rid) {
            drupal_set_message('Dòng '.$r.' (mã cũ: '.$data['mamh1'].'; mã mới: '.$data['mamh2'].') đã tồn tại.', 'error');
            continue;
        }
        try {
            db_insert('uit_monhoctd')->fields($data)->execute();
            $succ++;
        } catch (Exception $exc) {
            drupal_set_message('Dòng '.$r.' bị lỗi ('.$exc->getMessage().')');
            $err++;
        }
    }
    if($succ > 0) {
        drupal_set_message('Có '.$succ.' dòng import thành công');
        ghilogs('Import môn học tương dương ('.$succ.' dòng thành công)', basename($filepath), 'qldanhmuc');
    }
    if($err > 0 && $succ == 0) {
        drupal_set_message('Import không thành công', 'error');
        return;
    }
    if($err > 0) {
        drupal_set_message('Có '.$err.' dòng import không thành công', 'error');
    }
    drupal_goto('admin/qlmonhoc/import/monhoctd');
}
