<?php
function qlhocvien_form_settings($form, &$form_state) {
    if(!user_access('UIT qlhocvien admin') && !user_access('UIT qlhocvien') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>THIẾT LẬP THAM SỐ PHÂN HỆ QUẢN LÝ SINH VIÊN</h1></center>';
    $form['markup'] = array('#markup' => $html);
    $form['settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('Thông tin cấu hình phân hệ quản lý sinh viên')
    );
    $form['settings']['QLSV_THUMUCANHSV'] = array(
        '#type' => 'textfield',
        '#title' => t('Thư mục ảnh:'),
        '#default_value' => variable_get('QLSV_THUMUCANHSV', ''),
        '#description' => t('Thư mục chứa ảnh của sinh viên'),
    );
    $form['settings']['QLSV_MAUIMPORTAD'] = array(
        '#type' => 'textarea',
        '#rows' => 15,
        '#title' => t('Mẫu tạo file import account vào AD:'),
        '#default_value' => variable_get('QLSV_MAUIMPORTAD', ''),
        '#description' => t('Mẫu tạo file import account vào AD'),
    );
    $form['settings']['QLSV_MAUIMPORTMAIL'] = array(
        '#type' => 'textfield',
        '#title' => t('Mẫu tạo file import account email:'),
        '#default_value' => variable_get('QLSV_MAUIMPORTMAIL', ''),
        '#description' => t('Mẫu tạo file import account email'),
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

function qlhocvien_form_settings_validate($form, &$form_state) {
        
}

function qlhocvien_form_settings_submit($form, &$form_state) {
    variable_set('QLSV_THUMUCANHSV', $form_state['values']['QLSV_THUMUCANHSV']);
    variable_set('QLSV_MAUIMPORTAD', $form_state['values']['QLSV_MAUIMPORTAD']);
    variable_set('QLSV_MAUIMPORTMAIL', $form_state['values']['QLSV_MAUIMPORTMAIL']);
    
    $strlog .= $form_state['values']['QLSV_THUMUCANHSV'];
    ghilogs('Cập nhật tham số phân hệ quản lý sinh viên', $strlog, 'qlhocvien');
    drupal_set_message('Cập nhật thành công.');
    return;
}