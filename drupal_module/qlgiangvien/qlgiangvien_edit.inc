<?php
function qlgiangvien_form_xem($form, &$form_state, $magv) {
    if(!user_access('UIT qlgiangvien')) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    if(!is_numeric($magv)) {
        drupal_set_message('Thông tin GV không hợp lệ', 'error');
        return;
    }
    
    $html = '<center><h1>XEM THÔNG TIN GIẢNG VIÊN</h1></center>';
    $form['markup'] = array('#markup' => $html);
    
    $arrdatefield = array('ngaysinh','cmnd_ngaycap','hochieu_ngaycap','ngayhopdong');
    $arrfields = db_select('uit_formfield','t')->fields('t')->condition('tablename','uit_thongtingv')->orderBy('thutu')->orderBy('id')->execute()->fetchAll();
    $query = db_select('uit_thongtingv', 't')->fields('t')->condition('magv', $magv)->execute()->fetch();
    if(!$query) {
        drupal_set_message('Không tìm thấy thông tin GV này', 'error');
        return;
    }
    $thongtingv = (array)$query;
    $danhmuc = array();
    $danhmuc['gioitinh'] = array(0=>'Nữ',1=>'Nam');
    $danhmuc['tinhtrang'] = db_select('uit_danhmuc','k')->fields('k',array('madm','tendm'))->condition('loaidm', 'tinhtranggv')->execute()->fetchAllKeyed();
    $danhmuc['donvi'] = db_select('uit_danhmuc','k')->fields('k',array('madm','tendm'))->condition('loaidm','donvi')->execute()->fetchAllKeyed();
    $danhmuc['hocham'] = db_select('uit_danhmuc','k')->fields('k',array('madm','tendm'))->condition('loaidm', 'hocham')->execute()->fetchAllKeyed();
    $danhmuc['hocvi'] = db_select('uit_danhmuc','k')->fields('k',array('madm','tendm'))->condition('loaidm', 'hocvi')->execute()->fetchAllKeyed();
    
    $form['submit0'] = array(
        '#type' => 'submit',
        '#value' => t('Cập nhật'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlgiangvien_request_capnhat'),
    );
    $form['goback0'] = array( '#type' => 'button',
                             '#value' => t('Đóng'),
                             '#attributes' => array('onclick'=>'window.close();'),
                             '#executes_submit_callback' => FALSE,
                            );
    
    //$html = '<div class="hinhsv"><img src="'.$base_url.'/image.php?user='.$magv.'&id='.substr(md5($magv.$magv.'jpg'), 5, 10).'" width="110" height="140"></div>';
    $html = '<div class="info"><table>';
    $n = 0;
    foreach ($arrfields as $key => $value) {
        $fvalue = (isset($thongtingv[$value->field]) ? $thongtingv[$value->field]:'');        
        //if($value == NULL) continue;
        if($n%2 == 0) $html .= '<tr>';
        $html .= '<td><strong>'.$value->fieldname.': </strong><font color="blue">';
        switch ($value->field) {
            case 'gioitinh':
            case 'tinhtrang':
            case 'donvi':
            case 'hocham':
            case 'hocvi':
                $html .= $danhmuc[$value->field][$fvalue];
                break;
            
            default:
                $html .= (in_array($value->field, $arrdatefield) && $fvalue != '' && strlen($fvalue) == 10) ? date("d-m-Y",strtotime($fvalue)):$fvalue;
                break;
        }
        $html .= '</font></td>';
        $n++;
        if($n%2 == 0) $html .= '</tr>';
        
    }
    $html .= '</table></div>';
    $form[$value->field] = array('#markup' => $html);
    $form['id'] = array('#type'=>'hidden','#default_value' => $thongtingv['id']);
    $form['mssv'] = array('#type'=>'hidden','#default_value' => $thongtingv['magv']);
    
    $form['submit'] = array(
        '#type' => 'button',
        '#value' => t('Cập nhật'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlgiangvien_request_capnhat'),
    );
    $form['goback'] = array( '#type' => 'button',
                             '#value' => t('Đóng'),
                             '#attributes' => array('onclick'=>'window.close();'),
                             '#executes_submit_callback' => FALSE,
                            );
    
    return $form;
}

function qlgiangvien_request_capnhat($form, &$form_state) {
    drupal_goto('admin/qlgiangvien/capnhat/'.$form_state['values']['mssv']);
}

function qlgiangvien_form_edit($form, &$form_state, $magv) {
    if(!user_access('UIT qlgiangvien admin')) {
        echo 'Bạn không được phép truy cập menu này';
        die;
    }
    if(!is_numeric($magv)) {
        drupal_set_message('Thông tin GV không hợp lệ', 'error');
        return;
    }
    
    $html = '<center><h1>CẬP NHẬT THÔNG TIN GIẢNG VIÊN</h1></center>';
    $form['markup'] = array('#markup' => $html);
    
    //$arrdatefield = array('ngaysinh','cmnd_ngaycap','hochieu_ngaycap','ngayhopdong');
    $arrfields = db_select('uit_formfield','t')->fields('t')->condition('tablename','uit_thongtingv')->orderBy('thutu')->orderBy('id')->execute()->fetchAll();
    $query = db_select('uit_thongtingv', 't')->fields('t')->condition('magv', $magv)->execute()->fetch();
    if(!$query) {
        drupal_set_message('Không tìm thấy thông tin GV này', 'error');
        return;
    }
    $form['submit0'] = array(
        '#type' => 'submit',
        '#value' => t('Cập nhật'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlgiangvien_capnhat'),
    );
    $form['goback0'] = array( '#type' => 'button',
                             '#value' => t('Đóng'),
                             '#attributes' => array('onclick'=>'window.close();'),
                             '#executes_submit_callback' => FALSE,
                             '#validate' => FALSE,
                            );
    $thongtingv = (array)$query;
    
    $danhmuc = array();
    $danhmuc['quanly'] = array(0=>'Không',1=>'Có');
    $danhmuc['gioitinh'] = array(0=>'Nữ',1=>'Nam');
    $danhmuc['tinhtrang'] = db_select('uit_danhmuc','k')->fields('k',array('madm','tendm'))->condition('loaidm', 'tinhtranggv')->execute()->fetchAllKeyed();
    $danhmuc['donvi'] = (array(''=>'Chọn đơn vị') + db_select('uit_danhmuc','k')->fields('k',array('madm','tendm'))->condition('loaidm', 'donvi')->execute()->fetchAllKeyed());
    $danhmuc['hocham'] = (array(''=>'Không có') + db_select('uit_danhmuc','k')->fields('k',array('madm','tendm'))->condition('loaidm', 'hocham')->execute()->fetchAllKeyed());
    $danhmuc['hocvi'] = (array(''=>'Không có') + db_select('uit_danhmuc','k')->fields('k',array('madm','tendm'))->condition('loaidm', 'hocvi')->execute()->fetchAllKeyed());
    $danhmuc['danhhieunhagiao'] = (array(''=>'Không có') + db_select('uit_danhmuc','k')->fields('k',array('madm','tendm'))->condition('loaidm', 'danhhieunhagiao')->execute()->fetchAllKeyed());

    $form['qlsv_data'] = array(
        '#prefix' => '<div id="table_dsgv">',
        '#suffix' => '</div>',);
    $frame = '';
    foreach ($arrfields as $field) {
        $fvalue = (isset($thongtingv[$field->field]) ? $thongtingv[$field->field]:NULL);
        if($field == NULL) continue;
        /*
        $arrform = array('#title' => $value->fieldname,
            '#default_value' => (in_array($value->field, $arrdatefield) && $fvalue != '' && strlen($fvalue) == 10) ? date("d-m-Y",strtotime($fvalue)):$fvalue,
            '#type' => $value->fieldtype,
            '#required' => $value->required);
        if($value->fieldtype == 'textfield') {
            $arrform = array_merge($arrform, array('#size' => 120, '#attributes' => array('style' => 'color:blue;')));
        }
        if($value->field == 'magv') {
            $arrform = array_merge($arrform, array('#attributes' => 
                array('style' => 'color:blue;','disabled'=>True)));
        }
        
        if($value->fieldtype == 'select') {
            $options = array();
            switch ($value->field) {
                case 'gioitinh':
                case 'tinhtrang':
                case 'donvi':
                case 'hocham':
                case 'hocvi':
                case 'quanly':
                //case 'chucvu':
                case 'danhhieunhagiao':
                    $options = $danhmuc[$value->field];
                    break;
                
                default:
                    break;
            }
            $arrform = array_merge($arrform, array('#options'=>$options));
        }
        $form[$value->field] = $arrform;
         * 
         */
        $readonly = FALSE;
        if($field->field == 'magv') $readonly = TRUE;
        $form[$field->field] = render_form($field, $fvalue, (isset($danhmuc[$field->field]) ? $danhmuc[$field->field]:NULL),$readonly);
    }
    $form['id'] = array('#type'=>'hidden','#default_value' => $thongtingv['id']);
    $form['magv_old'] = array('#type'=>'hidden','#default_value' => $thongtingv['magv']);
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Cập nhật'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlgiangvien_capnhat'),
    );
    $form['goback'] = array( '#type' => 'button',
                             '#value' => t('Đóng'),
                             '#attributes' => array('onclick'=>'window.close();'),
                             '#executes_submit_callback' => FALSE,
                             '#validate' => FALSE,
                            );
    
    return $form;
}

function qlgiangvien_form_edit_validate($form, &$form_state) {
    $ngaysinh = trim($form_state['values']['ngaysinh']);
    if(strlen($ngaysinh) == 10 && !checkdate(substr($ngaysinh, 3,2),substr($ngaysinh, 0,2),substr($ngaysinh, 6,4))) {
        form_set_error('ngaysinh', t('Ngày sinh không hợp lệ.'));
        return;
    }
}

function qlgiangvien_capnhat($form, &$form_state) {
    $sid = (int)$form_state['values']['id'];
    $magv = $form_state['values']['magv'];
    if($sid <= 0) {
        drupal_set_message('Dữ liệu không phù hợp','error');
        return;
    }
    $arrdatefield = array('ngaysinh','cmnd_ngaycap','hochieu_ngaycap','ngayhopdong');
    $arrfields = db_select('uit_formfield','t')->fields('t')->condition('tablename','uit_thongtingv')->execute()->fetchAll();
    $fupdate = array();
    foreach ($arrfields as $value) {
        if(!isset($form_state['values'][$value->field])) continue;
        $fvalue = trim($form_state['values'][$value->field]);
        if(in_array($value->field, $arrdatefield) && strlen($fvalue) == 10) {
            $fvalue = ConvertToDate($fvalue);
        }
        $fupdate = array_merge($fupdate, array($value->field => $fvalue));
    }
    try {
        db_update('uit_thongtingv')->fields($fupdate)->condition('id', $sid)->execute();
        drupal_set_message('Cập nhật thành công');
        ghilogs('Cập nhật thông tin GV '.$magv, $sid, 'qlgiangvien');
    } catch (Exception $exc) {
        drupal_set_message('Cập nhật bị lỗi ('.$exc->getMessage().')','error');
    }
    drupal_goto('admin/qlgiangvien/capnhat/'.$form_state['values']['magv']);
}
