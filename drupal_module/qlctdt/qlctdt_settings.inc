<?php
function qlhocphi_settings($form, &$form_state) {
    if (!user_access('Quản lý CTĐT') && !user_access('Administer permissions') ) {
        drupal_set_message(t('Bạn không được phép truy cập trang này.'), 'error');
        return;
    }
    
    $html = '<center><h1>QUẢN LÝ THAM SỐ CHƯƠNG TRÌNH ĐÀO TẠO</h1></center>';
    
    $form['markup'] = array('#markup' => $html);
    //Thêm các tham số của module ở đây
   
    return $form; 
}
