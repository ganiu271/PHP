<?php
function qlctdt_import($form, &$form_state) {
    if(!user_access('Quản lý CTĐT') && !user_access('Administer permissions') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $form['browser'] = array(
      '#type' => 'fieldset',
      '#title' => t('Import CTDT'),
      '#collapsible' => TRUE,
      '#description' => t("Upload file CTDT (xls, xlsx)."),
    );
    $file_size = t('Maximum file size: !size MB.', array('!size' => file_upload_max_size()/1024/1024));
    $form['browser']['file_upload'] = array(
      '#type' => 'file',
      '#title' => t('Excel File '.l('(file mẫu)', drupal_get_path('module', 'qlctdt').'/templates/Import_CTDT.xlsx')),
      '#size' => 40,
      '#description' => t('Chọn file excel để import. ') . $file_size,
    );

    $form['submit'] = array(
        '#type' => 'submit', 
        '#value' => t('Import'),
    );
    // set the form encoding type
    $form['#attributes']['enctype'] = "multipart/form-data";
    return $form;
}

function qlctdt_import_validate($form, &$form_state) {
    // attempt to save the uploaded file
    $file = file_save_upload('file_upload', array('file_validate_extensions'=>array('xls xlsx')), 'public://uploads', FILE_EXISTS_REPLACE);
    // check file uploaded OK
    if (!$file) { 
      form_set_error('file_upload', t('Upload bị lỗi.'));
    }
    $form_state['values']['file_upload'] = $file;  
}

function qlctdt_import_submit($form, &$form_state) {
    $file = $form_state['values']['file_upload'];
    $filepath = 'sites/default/files/uploads';
    if(!file_exists($filepath)) mkdir($filepath);
    $filepath .= '/'.$file->filename;
    if(!import_file($filepath)) {
        drupal_set_message('Upload file không thành công','error');
    }
    unlink($filepath);
    return ;
}

function import_file($filepath) {
    if(!file_exists($filepath)) return FALSE;

    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $objPHPExcel = PHPExcel_IOFactory::load($filepath);
    $arrSheetName = $objPHPExcel->getSheetNames();
    
    $arrFieldsDB = array('mamh','tenmh','sotc','sotc_lt','sotc_th','mactdt','giaidoan','hocky','khoadt','hedt','nganh','loaihinh','trangthai','ghichu');
    $succ = 0;
    $err = 0;
    for($i=0; $i<$objPHPExcel->getSheetCount();$i++) {
        $ActiveSheet = $objPHPExcel->setActiveSheetIndex($i);
        $arrFields = array();
        $col_mamh = NULL;
        for($c='A';$c <= $ActiveSheet->getHighestColumn() || strlen($c) < strlen($ActiveSheet->getHighestColumn());$c++) {
            $fieldname = $ActiveSheet->getCell($c.'1')->getValue();
            if(in_array($fieldname, $arrFieldsDB)) {
                $arrFields[$fieldname] = $c;
                if($fieldname == 'mamh') $col_mamh = $c;
            }
        }
    
        for($r = 2; $r <= $ActiveSheet->getHighestRow(); $r++) {
            $mamh = trim($ActiveSheet->getCell($col_mamh.$r)->getValue());
            if(!isset($mamh) || $mamh == '') continue;
            $fields = array();
            foreach ($arrFields as $key => $col) {
                $val = $ActiveSheet->getCell($col.$r)->getValue();
                if(is_bool($val)) {
                    if($val) $val = 1;  else $val = 0;
                } else {
                    $val = trim($val);
                }
                $fields[$key] = $val;
            }
            
            $data2 = array_merge($fields, array('sheet'=>$arrSheetName[$i]));
            $strcheck = KiemTraCTDT($data2);
            if($strcheck != '') {
                drupal_set_message('Sheet '.$arrSheetName[$i].' dòng '.$r.' bị lỗi ('.$strcheck.')', 'error');
                continue;
            }
            $idcheck = db_select('uit_ctdaotao', 't')->fields('t', array('id'))
                ->condition('mamh', $fields['mamh'])
                ->condition('nganh', $fields['nganh'])
                ->condition('mactdt',$fields['mactdt'])
                ->execute()->fetchField();
            try {
                unset($fields['tenmh']);unset($fields['giaidoan']);
//                foreach($fields as $entry){
//                    if($entry=='') unset($entry);
//                }
                if(!$idcheck) { // Insert
                    db_insert('uit_ctdaotao')->fields($fields)->execute();
                } else { // Update
                    db_update('uit_ctdaotao')->fields($fields)->condition('id', $idcheck)->execute();
                }
            } catch (Exception $exc) {
                drupal_set_message('Dòng '.$r.' bị lỗi ('.$exc->getMessage().')', 'error');
                $err++;
                continue;
            }
            $succ++;
        }
    }
    if($succ > 0) {
        drupal_set_message('Có '.$succ.' dòng import thành công');
        ghilogs('Import CTDT ('.$succ.' dòng thành công)', basename($filepath), 'qlctdt');
    }
    if($err > 0 && $succ == 0) {
        drupal_set_message('Import không thành công', 'error');
        return;
    }
    if($err > 0) {
        drupal_set_message('Có '.$err.' dòng import không thành công', 'error');
    }
    drupal_goto('admin/qlctdt/import');
}

function KiemTraCTDT($data) {
    //Kiểm tra môn học tồn tại
//    $check = db_select('uit_monhoc', 't')->fields('t', array('mamh'))->condition('mamh', $data['mamh'])->condition('nganh', $data['nganh'])->execute()->fetch();
    $check = db_select('uit_monhoc', 't')->fields('t', array('mamh'))->condition('mamh', $data['mamh'])->execute()->fetch();
    if(!$check) {
        if($data['mamh'] != '') {
            db_insert('uit_monhoc')->fields(array(
                'mamh' => $data['mamh'],
                'tenmh' => $data['tenmh'],
                'tenta' => $data['tenmh'],
                'nganh' => $data['nganh'],
                'sotc' => $data['sotc'],
                'sotc_lt' => $data['sotc_lt'],
                'sotc_th' => $data['sotc_th'],
                'hedt' => $data['hedt'],
            ))->execute();
            drupal_set_message('Thêm mới môn học '.$data['mamh'].' thành công.');
        } else {
            return 'Mã môn học - '.$data['mamh'].' - không hợp lệ';
        }
    }
    //Kiểm tra SoTC hợp lệ
    if($data['sotc'] < 0 || $data['sotc_lt'] < 0 || $data['sotc_th'] < 0 ) {
        return 'Số TC / tiết không hợp lệ';
    }
    //Kiểm tra Khoa - Ngành - hệ ĐT hợp lệ
    $check = db_select('uit_nganh', 't')->fields('t')
            ->condition('nganh', $data['nganh'])
            ->condition('hedt', '%'.$data['hedt'].'%', 'LIKE')->execute()->fetch();
    if(!$check) {
        return 'Thông tin Hệ ĐT - Ngành không hợp lệ';
    }
    //Kiểm tra trùng CTĐT
    $check = db_select('uit_ctdaotao', 't')->fields('t', array('mamh'))
            ->condition('mamh', $data['mamh'])
            ->condition('nganh', $data['nganh'])
            ->condition('mactdt',$data['mactdt'])
            ->execute()->fetch();
    if($check) {
        return 'Mã môn học ('.$data['mamh'].') bị trùng trong CTĐT (cùng mamh, nganh)';
    }
    return;
}
