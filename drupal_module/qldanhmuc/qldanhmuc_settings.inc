<?php
function qltuyensinh_settings($form, &$form_state) {
    if(!user_access('Quản lý danh mục') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>THIẾT LẬP THAM SỐ TUYỂN SINH</h1></center>';
    $form['markup'] = array('#markup' => $html);
    $form['settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('Thông tin cấu hình phân hệ tuyển sinh')
    );
    $form['settings']['TS_NAMHOC'] = array(
        '#type' => 'textfield',
        '#title' => t('Năm học:'),
        '#default_value' => variable_get('TS_NAMHOC', date('Y')),
        '#description' => t('Năm tuyển sinh đang xử lý'),
    );
    $form['settings']['TS_MATRUONG'] = array(
        '#type' => 'textfield',
        '#title' => t('Mã Trường:'),
        '#default_value' => variable_get('TS_MATRUONG', 'QSC'),
        '#description' => t('Mã trường theo qui định của Bộ'),
    );
    $form['settings']['TS_MONTN'] = array(
        '#type' => 'textfield',
        '#title' => t('Môn thi trắc nghiệm:'),
        '#default_value' => variable_get('TS_MONTN', 'ANHA1,VLIA1,HOAA1'),
        '#description' => t('Các môn thi trắc nghiệm'),
    );
    $form['settings']['TS_MADETUCHON'] = array(
        '#type' => 'textfield',
        '#title' => t('Mã đề có câu tự chọn:'),
        '#default_value' => variable_get('TS_MADETUCHON', 'VLIA1,HOAA1'),
        '#description' => t('Mã đề có phần câu tự chọn'),
    );
    $form['settings']['TS_CAUTUCHON'] = array(
        '#type' => 'textfield',
        '#title' => t('Câu bắt đầu phần tự chọn:'),
        '#default_value' => variable_get('TS_CAUTUCHON', '40'),
        '#description' => t('Câu bắt đầu phần tự chọn'),
    );
    $form['settings']['TS_SOCAUTUCHON'] = array(
        '#type' => 'textfield',
        '#title' => t('Số câu tự chọn:'),
        '#default_value' => variable_get('TS_SOCAUTUCHON', '10'),
        '#description' => t('Số câu tự chọn'),
    );
    $form['settings']['TS_THUMUCANH'] = array(
        '#type' => 'textfield',
        '#title' => t('Thư mục ảnh:'),
        '#default_value' => variable_get('TS_THUMUCANH', ''),
        '#description' => t('Thư mục chứa ảnh đã scan của tất cả các môn (mỗi môn đặt tên thư mục theo mã môn)'),
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Lưu lại'),
    );
    $form['cancel'] = array(
        '#type' => 'button', 
        '#value' => t('Đóng'),
        '#validate' => FALSE,
        '#executes_submit_callback' => FALSE,
        '#attributes' => array('onclick'=>'window.close();'),
    );
    
    return $form;
}

function qltuyensinh_settings_validate($form, &$form_state) {
    $namhoc = (int)$form_state['values']['TS_NAMHOC'];
    if ($namhoc < 2006 || $namhoc > 2100) {
        form_set_error('', t('Năm học không hợp lệ'));
    }    
}

function qltuyensinh_settings_submit($form, &$form_state) {
    variable_set(TS_NAMHOC, (int)$form_state['values']['TS_NAMHOC']);
    variable_set(TS_MATRUONG, $form_state['values']['TS_MATRUONG']);
    variable_set(TS_MONTN, $form_state['values']['TS_MONTN']);
    variable_set(TS_MADETUCHON, $form_state['values']['TS_MADETUCHON']);
    variable_set(TS_CAUTUCHON, $form_state['values']['TS_CAUTUCHON']);
    variable_set(TS_SOCAUTUCHON, $form_state['values']['TS_SOCAUTUCHON']);
    variable_set(TS_THUMUCANH, $form_state['values']['TS_THUMUCANH']);
    
    $strlog = (int)$form_state['values']['TS_NAMHOC'].'; '.$form_state['values']['TS_MATRUONG'].'; '.$form_state['values']['TS_MONTN'].'; ';
    $strlog .= $form_state['values']['TS_MADETUCHON'].'; '.$form_state['values']['TS_CAUTUCHON'].'; '.$form_state['values']['TS_SOCAUTUCHON'];
    ghilogs('Cập nhật tham số tuyển sinh', $strlog, 'qltuyensinh');
    drupal_set_message('Cập nhật thành công.');
    return;
}