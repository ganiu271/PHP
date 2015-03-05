<?php
function qldanhmuc_import($form, &$form_state, $importform) {
    global $base_url;
    if(!user_access('Quản lý danh mục') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>IMPORT FILES - DANH MỤC</h1></center>';
    $form['markup'] = array('#markup' => $html);
    $options = array(
            '' => 'Chọn mẫu import',
            'coso' => 'Cơ sở (xls, xlsx)',
            'monhoc' => 'Môn học (xls, xlsx)',
            'ctdt' => 'Chương trình đào tạo (xls, xlsx)',
            );
    
    $form['importform'] = array(
            '#title'=>t('Chọn mẫu import:'),
            '#type'=>'select',
            '#options'=>$options,
            '#default_value'=> (isset($importform) ? $importform:NULL),
            '#attributes' => array('onchange' => 'location.href=\''.$base_url.'/admin/qldanhmuc/import/\'+document.getElementById(\'edit-importform\').value;'),
        ); 
    $form['browser'] = array(
      '#type' => 'fieldset',
      '#title' => t('Import file'),
      '#collapsible' => TRUE,
      '#description' => t("Upload file").qldanhmuc_import_template($importform),
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

function qldanhmuc_import_template($importform) {
    $html = '';
    switch ($importform) {
        case 'coso':
            $html = l(' (File mẫu)',drupal_get_path('module', 'qldanhmuc').'/templates/Import_CoSo.xlsx');
            break;
        case 'monhoc':
            $html = l(' (File mẫu)',drupal_get_path('module', 'qlmonhoc').'/templates/Import_MonHoc.xlsx');
            break;
        case 'ctdt':
            $html = l(' (File mẫu)',drupal_get_path('module', 'qldanhmuc').'/templates/Import_DM_CTDT.xlsx');
            break;
        default:
            break;
    }
    
    return $html;
}

function qldanhmuc_import_validate($form, &$form_state) {
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

function qldanhmuc_import_submit($form, &$form_state) {
    $file = $form_state['values']['file_upload'];
    $importform = $form_state['values']['importform'];
    $filepath = 'sites/default/files/uploads';
    if(!file_exists($filepath)) mkdir($filepath);
    $filepath .= '/'.$file->filename;
    switch ($importform) {
        case 'coso':
            importcoso($filepath);
            break;
        case 'monhoc':
            importmonhoc($filepath);
            break;
        case 'ctdt':
            importdmctdt($filepath);
            break;
        
        default:
            break;
    }
    
    return ;
}

function importcoso($filepath) {
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $objPHPExcel = PHPExcel_IOFactory::load($filepath);
    $ActiveSheet = $objPHPExcel->setActiveSheetIndex('0');
    
    $arrFieldsDB = array('macs','tencs','tenvt','chuquan','diachi','daidien','dienthoai','ghichu');
    $succ = 0;
    $err = 0;
    $arrFields = array();
    $col_key = NULL;
    for($c='A';$c <= $ActiveSheet->getHighestColumn() || strlen($c) < strlen($ActiveSheet->getHighestColumn());$c++) {
        $fieldname = $ActiveSheet->getCell($c.'1')->getValue();
        if(in_array($fieldname, $arrFieldsDB)) {
            $arrFields[$fieldname] = $c;
            if($fieldname == 'macs') $col_key = $c;
        }
    }

    for($r = 2; $r <= $ActiveSheet->getHighestRow(); $r++) {
        $madm = trim($ActiveSheet->getCell($col_key.$r)->getValue());
        if(!isset($madm) || $madm == '') continue;
        $fields = array();
        foreach ($arrFields as $key => $col) {
            //if($key == 'magv') continue;
            $val = $ActiveSheet->getCell($col.$r)->getValue();
            if(is_bool($val)) {
                if($val) $val = 1;  else $val = 0;
            } else {
                $val = trim($val);
            }
            $fields[$key] = $val;
        }
        $idcheck = db_select('uit_coso', 't')->fields('t', array('id'))
                ->condition('macs', $fields['macs'])->execute()->fetchField();
        try {
            if(!$idcheck) { //Insert
                db_insert('uit_coso')->fields($fields)->execute();
            } else { //Update
                db_update('uit_coso')->fields($fields)->condition('id', $idcheck)->execute();
            }
        } catch (Exception $exc) {
            drupal_set_message('Dòng '.$r.' bị lỗi ('.$exc->getMessage().')', 'error');
            $err++;
            continue;
        }
        $succ++;
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
    drupal_goto('admin/qldanhmuc/import/coso');
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
    drupal_goto('admin/qldanhmuc/import/monhoc');
}

function importdmctdt($filepath) {
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $objPHPExcel = PHPExcel_IOFactory::load($filepath);
    $ActiveSheet = $objPHPExcel->setActiveSheetIndex('0');
    
    $arrFieldsDB = array('ma_ctdaotao','ten_ctdaotao','hedt','khoa_daotao','giaidoan','thoigian_daotao','thoigian_batdau','ghichu');
    $succ = 0;
    $err = 0;
    $arrFields = array();
    $col_key = NULL;
    for($c='A';$c <= $ActiveSheet->getHighestColumn() || strlen($c) < strlen($ActiveSheet->getHighestColumn());$c++) {
        $fieldname = $ActiveSheet->getCell($c.'1')->getValue();
        if(in_array($fieldname, $arrFieldsDB)) {
            $arrFields[$fieldname] = $c;
            if($fieldname == 'ma_ctdaotao') $col_key = $c;
        }
    }
//
    for($r = 2; $r <= $ActiveSheet->getHighestRow(); $r++) {
        $madm = trim($ActiveSheet->getCell($col_key.$r)->getValue());
        if(!isset($madm) || $madm == '') continue;
        $fields = array();
        foreach ($arrFields as $key => $col) {
            $val = $ActiveSheet->getCell($col.$r)->getValue();
            if(is_bool($val)) {
                if($val) $val = 1;  else $val = 0;
            } else {
                $val = trim($val);
            }
            if($key=='thoigian_batdau'){
               $val=PHPExcel_Style_NumberFormat::toFormattedString($val, "yyyy-mm-dd");
            }
            $fields[$key] = $val;
        }
        $idcheck = db_select('uit_quanly_ctdt', 't')->fields('t', array('id'))
                ->condition('ma_ctdaotao', $fields['ma_ctdaotao'])->execute()->fetchField();
        foreach($fields as $key=>$value){   // xóa các dòng trống
            if($value==''){
                unset($fields[$key]);
            }
        }
        try {
            if(!$idcheck) { //Insert
                db_insert('uit_quanly_ctdt')->fields($fields)->execute();
            } else { //Update
                db_update('uit_quanly_ctdt')->fields($fields)->condition('id', $idcheck)->execute();
            }
        } catch (Exception $exc) {
            drupal_set_message('Dòng '.$r.' bị lỗi ('.$exc->getMessage().')', 'error');
            $err++;
            continue;
        }
        $succ++;
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
    drupal_goto('admin/qldanhmuc/import/ctdt');
}
