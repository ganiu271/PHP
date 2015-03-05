<?php
function qlhocphi_settings($form, &$form_state) {
    global $user;
    if (!in_array("administrator", $user->roles)) {
        drupal_set_message(t('Bạn không được phép truy cập trang này.'), 'error');
        return;
    }
    
    $html = '<center><h1>QUẢN LÝ THAM SỐ CHƯƠNG TRÌNH ĐÀO TẠO</h1></center>';
    
    $form['markup'] = array('#markup' => $html);
    
   
    return $form; 
}
