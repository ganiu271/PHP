<?php
/**
 * Form xem thông tin học viên
 * @param $form
 * @param $form_state
 * @param $mahv
 * @return mixed
 */
function qlhocvien_form_xem($form, &$form_state, $mahv) {
    global $base_url;
    if(!user_access('UIT qlhocvien')) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    if($mahv == '') {
        drupal_set_message('Thông tin HV không hợp lệ', 'error');
        return;
    }

    $html = '<center><h1>XEM THÔNG TIN HỌC VIÊN</h1></center>';
    $form['markup'] = array('#markup' => $html);

    $arrdatefield = array('ngaysinh','cmnd_ngaycap','ngayvaodoan','ngayvaodang');
    $arrfields = db_select('uit_formfield','t')->fields('t')->condition('tablename','uit_thongtinhv')->orderBy('thutu')->orderBy('id')->execute()->fetchAll();
    $query = db_select('uit_thongtinhv', 'sv')->fields('sv')->condition('mahv', $mahv)->execute()->fetch();
    if(!$query) {
        drupal_set_message('Không tìm thấy thông tin SV này', 'error');
        return;
    }
    $thongtinhv = (array)$query;

    $gioitinhs = array(0=>'Nữ',1=>'Nam');
//    $cosos = db_select('uit_coso','k')->fields('k',array('macs','tencs'))->execute()->fetchAllKeyed();
    $hedts = db_select('uit_hedt','k')->fields('k',array('kyhieu','tenhedt'))->execute()->fetchAllKeyed();
    $chuyennganhs = db_select('uit_nganh','k')->fields('k',array('nganh','nganh'))->execute()->fetchAllKeyed();

    $tinhtrangs = db_select('uit_tinhtrang','k')->fields('k',array('id','ten'))->condition('doituong', 'SV')->execute()->fetchAllKeyed();
    $dantocs = db_select('uit_dantoc','k')->fields('k',array('id','ten'))->execute()->fetchAllKeyed();
    $tongiaos = db_select('uit_tongiao','k')->fields('k',array('id','ten'))->execute()->fetchAllKeyed();
    $xuatthans = db_select('uit_xuatthan','k')->fields('k',array('id','ten'))->execute()->fetchAllKeyed();
    $loaihinhdaotao=array('0'=>'Chưa có thông tin', '1'=>'Chuyên tu','2'=>'Tại chức', '3'=>'Mở rộng','4'=>'Chính quy' );
    $xephangtotnghiep=array( '0'=>'Chưa có thông tin', '1'=>'Giỏi', '2'=>'Khá',  '3'=>'Trung bình khá','4'=>'Trung bình','5'=>'Xuất sắc',);
    $form['submit0'] = array(
        '#type' => 'submit',
        '#value' => t('Cập nhật'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlhocvien_request_capnhat'),
    );
    $form['goback0'] = array( '#type' => 'button',
        '#value' => t('Đóng'),
        '#attributes' => array('onclick'=>'window.close();'),
        '#executes_submit_callback' => FALSE,
    );

    $html = '<div class="hinhsv"><img src="'.$base_url.'/image.php?user='.$mahv.'&id='.substr(md5($mahv.$mahv.'jpg'), 5, 10).'" width="110" height="140"></div>';
    $html .= '<div class="info"><table>';
    $n = 0;
    foreach ($arrfields as $key => $value) {
        $fvalue = (isset($thongtinhv[$value->field]) ? $thongtinhv[$value->field]:'');
        //if($value == NULL) continue;
        if($n%2 == 0) $html .= '<tr>';
        $html .= '<td><strong>'.$value->fieldname.': </strong><font color="blue">';
        switch ($value->field) {
            case 'gioitinh':
                $html .= $gioitinhs[$fvalue];
                break;
//            case 'coso':
//                $html .= $cosos[$fvalue];
//                break;
//            case 'hedt':
//                $html .= $hedts[$fvalue];
//                break;
//            case 'chuyennganh':
//                $html .= $chuyennganhs[$fvalue];
//                break;
            case 'tinhtrang':
                $html .= $tinhtrangs[$fvalue];
                break;
            case 'dantoc':
                $html .= $dantocs[$fvalue];
                break;
            case 'tongiao':
                $html .= $tongiaos[$fvalue];
                break;
            case 'xuatthan':
                $html .= $xuatthans[$fvalue];
                break;
            case 'loaihinh_daotao':
                $html .= $loaihinhdaotao[$fvalue];
                break;
            case 'xephangtotnghiep':
                $html .= $xephangtotnghiep[$fvalue];
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
    $form['id'] = array('#type'=>'hidden','#default_value' => $thongtinhv['id']);
    $form['mssv'] = array('#type'=>'hidden','#default_value' => $thongtinhv['mahv']);

    $form['submit'] = array(
        '#type' => 'button',
        '#value' => t('Cập nhật'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlhocvien_request_capnhat'),
    );
    $form['goback'] = array( '#type' => 'button',
        '#value' => t('Đóng'),
        '#attributes' => array('onclick'=>'window.close();'),
        '#executes_submit_callback' => FALSE,
    );

    //Lịch sử tình trạng
    $query = db_select('uit_lichsu_tinhtrang_hocvien', 't');
    $query->leftJoin('uit_tinhtrang', 'tt', 'tt.id = t.tinhtrang');
    $query->fields('t')->fields('tt', array('ten'));
    $query->condition('mahv', $mahv)->condition('trangthai', 1);
    $query->orderBy('namhoc')->orderBy('hocky');
    $rows = $query->execute()->fetchAll();
    if($rows) {
        $form['lichsu'] = array('#type' => 'fieldset', '#title' => t('Lịch sử'),
            '#attributes' => array('class' => array('container-inline')), '#collapsible' => TRUE, '#collapsed' => FALSE);
        $headers = array(
            array('data' => t('Năm'), 'field' => 'namhoc'),
            //array('data' => t('Học kỳ'), 'field' => 'hocky'),
            array('data' => t('Tình trạng'), 'field' => 'ten'),
            array('data' => t('Số QĐ'), 'field' => 'soqd'),
            array('data' => t('Ngày QĐ'), 'field' => 'ngayqd'),
            array('data' => t('Ghi chú'), 'field' => 'ghichu'),
        );
        $options = array();
        foreach ($rows as $row) {
            $options[] = array(
                $row->namhoc,
                //$row->hocky,
                $row->ten,
                $row->soqd,
                (strlen($row->ngayqd) == 8 ? date('d-m-Y',  strtotime($row->ngayqd)):$row->ngayqd),
                $row->ghichu,
            );
        }
        $form['lichsu']['thongtin'] = array(
            '#title' => 'Table',
            '#type' => 'tableselect',
            '#header' => $headers,
            '#options' => $options,
        );
    }
    return $form;
}

/**
 * Form cập nhật thông tin học viên (view)
 * @param $form
 * @param $form_state
 * @param $mahv
 */
function qlhocvien_form_edit($form, &$form_state, $mahv) {
    global $base_url;
    if(!user_access('UIT qlhocvien admin')) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    if($mahv == '') {
        drupal_set_message('Thông tin SV không hợp lệ', 'error');
        return;
    }

    $html = '<center><h1>CẬP NHẬT THÔNG TIN HỌC VIÊN</h1></center>';
    $form['markup'] = array('#markup' => $html);

    $arrdatefield = array('ngaysinh','cmnd_ngaycap','ngayvaodoan','ngayvaodang');
    $arrfields = db_select('uit_formfield','t')->fields('t')->condition('tablename','uit_thongtinhv')->orderBy('thutu')->orderBy('id')->execute()->fetchAll();
    $query = db_select('uit_thongtinhv', 'sv')->fields('sv')->condition('mahv', $mahv)->execute()->fetch();
    if(!$query) {
        drupal_set_message('Không tìm thấy thông tin SV này', 'error');
        return;
    }
    $form['submit0'] = array(
        '#type' => 'submit',
        '#value' => t('Cập nhật'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlhocvien_capnhat'),
    );
    $form['goback0'] = array( '#type' => 'button',
        '#value' => t('Đóng'),
        '#attributes' => array('onclick'=>'window.close();'),
        '#executes_submit_callback' => FALSE,
        '#validate' => FALSE,
    );
    $html = '<div class="hinhsv_edit"><a href="'.$base_url.'/admin/qlhocvien/import/hinhanh" target="_Blank" style="align=\'center\';" title="Cập nhật hình SV">';
    $html .= '<img src="'.$base_url.'/image.php?user='.$mahv.'&id='.substr(md5($mahv.$mahv.'jpg'), 5, 10).'" width="110" height="140"></a></div>';
    $form['markup2'] = array('#markup' => $html);
    $thongtinhv = (array)$query;
    foreach ($arrfields as $key => $value) {
        //if(in_array($value->field, array('hedt','chuyennganh','tinhtrang'))) continue;
        $fvalue = (isset($thongtinhv[$value->field]) ? $thongtinhv[$value->field]:NULL);
        if($value == NULL) continue;
        $arrform = array('#title' => $value->fieldname,
            '#default_value' => (in_array($value->field, $arrdatefield) && $fvalue != '' && strlen($fvalue) == 10) ? date("d-m-Y",strtotime($fvalue)):$fvalue,
            '#type' => $value->fieldtype,
            '#required' => $value->required);
        if($value->fieldtype == 'textfield') {
            $arrform = array_merge($arrform, array('#size' => 120, '#attributes' => array('style' => 'color:blue;')));
        }
        if($value->field == 'mahv') {
            $arrform = array_merge($arrform, array('#attributes' =>
                array('style' => 'color:blue;','onblur'=>"document.getElementsByName('email')[0].value = document.getElementsByName('mahv')[0].value+'@gm.uit.edu.vn'")));
        }
        if($value->fieldtype == 'select') {
            $options = array();
            switch ($value->field) {
                case 'gioitinh':
                    $options = array(0=>'Nữ',1=>'Nam');
                    break;
                case 'coso':
                    $options = db_select('uit_coso','k')->fields('k',array('macs','tencs'))->execute()->fetchAllKeyed();
                    break;
                case 'hedt':
                    $options = db_select('uit_hedt','k')->fields('k',array('kyhieu','tenhedt'))->execute()->fetchAllKeyed();
                    break;
                case 'chuyennganh':
                    $options = db_select('uit_nganh','k')->fields('k',array('nganh','nganh'))->execute()->fetchAllKeyed();
                    break;
                case 'tinhtrang':
                    $options = db_select('uit_tinhtrang','k')->fields('k',array('id','ten'))->condition('doituong', 'SV')->execute()->fetchAllKeyed();
                    break;
                case 'dantoc':
                    $options = db_select('uit_dantoc','k')->fields('k',array('id','ten'))->execute()->fetchAllKeyed();
                    break;
                case 'tongiao':
                    $options = db_select('uit_tongiao','k')->fields('k',array('id','ten'))->execute()->fetchAllKeyed();
                    break;
                case 'xuatthan':
                    $options = db_select('uit_xuatthan','k')->fields('k',array('id','ten'))->execute()->fetchAllKeyed();
                    break;
                case 'chinhsach':
                    $options = array(''=>'Không','LS'=>'Con Liệt sĩ','TB'=>'Con Thương binh','BB'=>'Con Bệnh binh');
                    break;
                case 'vungmien':
                    $options = array(''=>'Chưa xác định','MN'=>'Miền núi','VS'=>'Vùng sâu','VC'=>'Vùng cao','NT'=>'Nông thôn','HĐ'=>'Hải đảo','NN'=>'Nước ngoài');
                    break;
                case 'loaihinh_daotao':
                    $options=array(
                        '0'=>'Chưa có thông tin',
                        '1'=>'Chuyên tu',
                        '2'=>'Tại chức',
                        '3'=>'Mở rộng',
                        '4'=>'Chính quy'
                    );
                    break;
                case 'daiornganhan':
                    $options=array(
                        '0'=>'Dài hạn',
                        '1'=>'Ngắn hạn'
                    );
                    break;
                case 'xephangtotnghiep':
                    $options=array(
                        '0'=>'Chưa có thông tin',
                        '1'=>'Giỏi',
                        '2'=>'Khá',
                        '3'=>'Trung bình khá',
                        '4'=>'Trung bình',
                        '5'=>'Xuất sắc',
                    );
                    break;
                case 'lopsh':
                    break;
                default:
                    break;
            }
            $arrform = array_merge($arrform, array('#options'=>$options));
        }
        $form[$value->field] = $arrform;
    }
    $form['id'] = array('#type'=>'hidden','#default_value' => $thongtinhv['id']);
    $form['mahv_old'] = array('#type'=>'hidden','#default_value' => $thongtinhv['mahv']);
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Cập nhật'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlhocvien_capnhat'),
    );
    $form['goback'] = array( '#type' => 'button',
        '#value' => t('Đóng'),
        '#attributes' => array('onclick'=>'window.close();'),
        '#executes_submit_callback' => FALSE,
        '#validate' => FALSE,
    );

    return $form;
}

/**
 * Form cập nhật thông tin học viên (validate)
 * @param $form
 * @param $form_state
 */
function qlhocvien_form_edit_validate($form, &$form_state) {
    $mahv = trim($form_state['values']['mahv']);
    $mahv_old = trim($form_state['values']['mahv_old']);
//    if(strlen($mahv) != 8) {
//        form_set_error('mahv', t('Mã SV không hợp lệ.'));
//        return;
//    }
    if($mahv != $mahv_old) {
        $check = db_select('uit_thongtinhv', 'sv')->fields('sv')->condition('mahv', $mahv)->execute()->fetch();
        if($check) {
            form_set_error('mahv', t('Mã HV bị trùng.'));
            return;
        }
    }
    $ngaysinh = trim($form_state['values']['ngaysinh']);
    if(strlen($ngaysinh) == 10 && !checkdate(substr($ngaysinh, 3,2),substr($ngaysinh, 0,2),substr($ngaysinh, 6,4))) {
        form_set_error('ngaysinh', t('Ngày sinh không hợp lệ.'));
        return;
    }
}

/**
 * Form cập nhật thông tin học viên (submit)
 * @param $form
 * @param $form_state
 */
function qlhocvien_capnhat($form, &$form_state) {
    $sid = (int)$form_state['values']['id'];
    $mahv_old = $form_state['values']['mahv_old'];
    $mahv = $form_state['values']['mahv'];
    if($sid <= 0) {
        drupal_set_message('Dữ liệu không phù hợp','error');
        return;
    }
    $arrdatefield = array('ngaysinh','cmnd_ngaycap','ngayvaodoan','ngayvaodang');
    $arrfields = db_select('uit_formfield','t')->fields('t')->condition('tablename','uit_thongtinhv')->execute()->fetchAll();
    $fupdate = array();
    foreach ($arrfields as $value) {
        //if(in_array($value->field, array('khoa', 'tinhtrang', 'chuyennganh'))) continue;
        if(!isset($form_state['values'][$value->field])) continue;
        $fvalue = trim($form_state['values'][$value->field]);
        if(in_array($value->field, $arrdatefield) && $fvalue != '') {
            if(strlen($fvalue) == 10) {
                $fvalue = ConvertToDate($fvalue);
            }
        }
        $fupdate = array_merge($fupdate, array($value->field => $fvalue));
    }
    try {
//        dvm($fupdate);
        db_update('uit_thongtinhv')->fields($fupdate)->condition('id', $sid)->execute();
        drupal_set_message('Cập nhật thành công');
        ghilogs('Cập nhật thông tin SV '.$mahv_old, $mahv, 'qlhocvien');
    } catch (Exception $exc) {
        drupal_set_message('Cập nhật bị lỗi ('.$exc->getMessage().')','error');
    }
    drupal_goto('admin/qlhocvien/capnhat/'.$form_state['values']['mahv']);
}

/**
 * Form thêm học viên (view)
 * @param $form
 * @param $form_state
 */
function qlhocvien_form_add($form, &$form_state) {
    if(!user_access('UIT qlhocvien admin')) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>THÊM HỌC VIÊN</h1></center>';
    $form['markup'] = array('#markup' => $html);

    //$arrdatefield = array('ngaysinh','cmnd_ngaycap','ngayvaodoan','ngayvaodang');
    //$arrfieldreq = array('mahv','hoten','ngaysinh');
    $arrfields = db_select('uit_formfield','t')->fields('t')->condition('tablename','uit_thongtinhv')->execute()->fetchAll();

    foreach ($arrfields as $key => $value) {
        $arrform = array('#title' => $value->fieldname,
            '#default_value' => $value->defaultvalue,
            '#type' => $value->fieldtype,
            '#required' => $value->required);
        if($value->fieldtype == 'textfield') {
            $arrform = array_merge($arrform, array('#size' => 120));
        }
        if($value->field == 'mahv') {
            $arrform = array_merge($arrform, array('#attributes' =>
                array('onblur'=>"document.getElementsByName('email')[0].value = document.getElementsByName('mahv')[0].value+'@gm.uit.edu.vn'")));
        }
        if($value->fieldtype == 'select') {
            $options = array();
            switch ($value->field) {
                case 'gioitinh':
                    $options = array(0=>'Nữ',1=>'Nam');
                    break;
                case 'coso':
                    $options = (array(''=>'Chưa xác định') + db_select('uit_coso','k')->fields('k',array('macs','tencs'))->execute()->fetchAllKeyed());
                    break;
                case 'hedt':
                    $options = db_select('uit_hedt','k')->fields('k',array('kyhieu','tenhedt'))->execute()->fetchAllKeyed();
                    break;
                case 'chuyennganh':
                    $options = db_select('uit_nganh','k')->fields('k',array('nganh','nganh'))->execute()->fetchAllKeyed();
                    break;
                case 'tinhtrang':
                    $options = db_select('uit_tinhtrang','k')->fields('k',array('id','ten'))->condition('doituong', 'SV')->execute()->fetchAllKeyed();
                    break;
                case 'dantoc':
                    $options = db_select('uit_dantoc','k')->fields('k',array('id','ten'))->execute()->fetchAllKeyed();
                    break;
                case 'tongiao':
                    $options = db_select('uit_tongiao','k')->fields('k',array('id','ten'))->execute()->fetchAllKeyed();
                    break;
                case 'xuatthan':
                    $options = db_select('uit_xuatthan','k')->fields('k',array('id','ten'))->execute()->fetchAllKeyed();
                    break;
                case 'loaihinh_daotao':
                    $options=array(
                        '0'=>'Chưa có thông tin',
                        '1'=>'Chuyên tu',
                        '2'=>'Tại chức',
                        '3'=>'Mở rộng',
                        '4'=>'Chính quy'
                    );
                    break;
                case 'daiornganhan':
                    $options=array(
                        '0'=>'Dài hạn',
                        '1'=>'Ngắn hạn'
                    );
                    break;
                case 'xephangtotnghiep':
                    $options=array(
                        '0'=>'Chưa có thông tin',
                        '1'=>'Giỏi',
                        '2'=>'Khá',
                        '3'=>'Trung bình khá',
                        '4'=>'Trung bình',
                        '5'=>'Xuất sắc',
                    );
                    break;
                case 'lopsh':
                    break;
                default:
                    break;
            }
            $arrform = array_merge($arrform, array('#options'=>$options));
        }
        $form[$value->field] = $arrform;
    }
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Thực hiện'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlhocvien_themsv'),
    );
    $form['goback'] = array( '#type' => 'button',
        '#value' => t('Quay lại'),
        '#validate' => FALSE,
        '#attributes' => array('onclick'=>'window.close();'),
        '#executes_submit_callback' => FALSE,
    );

    return $form;
}

/**
 * Form thêm học viên (validate)
 * @param $form
 * @param $form_state
 */
function qlhocvien_form_add_validate($form, &$form_state) {
    $mahv = trim($form_state['values']['mahv']);
    if(strlen($mahv) != 8) {
        form_set_error('mahv', t('Mã HV không hợp lệ.'));
        return;
    }
    $check = db_select('uit_thongtinhv', 'sv')->fields('sv')->condition('mahv', $mahv)->execute()->fetch();
    if($check) {
        form_set_error('mahv', t('Mã HV bị trùng.'));
        return;
    }
    $ngaysinh = trim($form_state['values']['ngaysinh']);
    if(strlen($ngaysinh) == 10 && !checkdate(substr($ngaysinh, 3,2),substr($ngaysinh, 0,2),substr($ngaysinh, 6,4))) {
        form_set_error('ngaysinh', t('Ngày sinh không hợp lệ.'));
        return;
    }
}

/**
 * Form thêm học viên (submit)
 * @param $form
 * @param $form_state
 */
function qlhocvien_themsv($form, &$form_state) {
    $arrdatefield = array('ngaysinh','cmnd_ngaycap','ngayvaodoan','ngayvaodang');
    $arrfields = db_select('uit_formfield','t')->fields('t')->condition('tablename','uit_thongtinhv')->execute()->fetchAll();
    $fupdate = array();
    foreach ($arrfields as $key => $value) {
        if($value->field == 'id') continue;
        $fvalue = trim($form_state['values'][$value->field]);
        if(in_array($value->field, $arrdatefield) && $fvalue != '') $fvalue = ConvertToDate($fvalue);
        $fupdate = array_merge($fupdate, array($value->field => $fvalue));
    }
    try {
//        db_insert('uit_thongtinhv')->fields($fupdate)->execute();
        dvm($fupdate);
        drupal_set_message('Cập nhật thành công');
//        ghilogs('Thêm SV mới ('.trim($form_state['values']['mahv']).')', '', 'qlhocvien');
    } catch (Exception $exc) {
        drupal_set_message('Cập nhật bị lỗi ('.$exc->getMessage().')','error');
    }
    drupal_goto('admin/qlhocvien');
}

/**
 * Form cập nhật ghi chú
 * @param $form
 * @param $form_state
 * @param int $mahv
 */
function qlhocvien_capnhat_ghichu($form, &$form_state, $mahv=-1) {
    global $user;
    if(!user_access('UIT qlhocvien cap nhat ghi chu')) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>CẬP NHẬT THÔNG TIN GHI CHÚ HỌC VIÊN</h1></center>';
    $form['markup'] = array('#markup' => $html);

    $form['mahv'] = array(
        '#title'=>t('Nhập mã SV cần cập nhật:'),
        '#size'=>20,
        '#type'=>'textfield',
        '#default_value'=> (isset($mahv) ? $mahv : ''),
    );
    $form['tim'] = array(
        '#type' => 'button',
        '#value' => t('Tìm kiếm'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlhocvien_ghichu_tim'),
    );
    if($mahv) {
        $q = db_select('uit_thongtinhv', 't');
//        $q->fields('t', array('hoten','ngaysinh','gioitinh','coso'))->fields('cs', array('tenvt'));
//        $q->leftJoin('uit_coso','cs','cs.macs=t.coso');
        $q->fields('t');
        $q->condition('t.mahv', $mahv);
        $SV = $q->execute()->fetchAssoc();
        if(!$SV) {
            drupal_set_message('Mã HV không hợp lệ');
            return $form;
        }
        $donvi = '';
        if(in_array("administrator", $user->roles)) $donvi = 'Admin';
        if(in_array("Phong CTSV",$user->roles)) $donvi = 'CTSV';
        if(in_array("Phong DTDH",$user->roles)) $donvi = 'ĐTĐH';
        if(in_array("Phong KHTC",$user->roles)) $donvi = 'KHTC';

        if($donvi != '') {
            $thongtinhv = "<p><strong><font color='Blue'>Họ tên: ".$SV['hoten']."; Giới tính: ".($SV['gioitinh'] ? 'Nam':'Nữ');
            $thongtinhv .= "; Ngày sinh: ".(strlen($SV['ngaysinh']) == 10 ? date('d-m-Y',  strtotime($SV['ngaysinh'])):$SV['ngaysinh']);
//            $thongtinhv .= "; Cơ sở: ".$SV->tenvt."</font></strong></p>";
            $ghichu = db_select('uit_hocvien_ghichu', 't')->fields('t', array('noidung'))->condition('mahv', $mahv)->condition('donvi', $donvi)->execute()->fetchField();
            $form['thongtinhv'] = array('#markup' => $thongtinhv);
            $form['ghichu'] = array(
                '#title'=>t('Nội dung ('.$donvi.'):'),
                '#cols'=>50,
                '#rows'=>10,
                '#resizable' => TRUE,
                '#type'=>'textarea',
                '#default_value'=> (isset($ghichu) ? $ghichu : ''),
            );
            $form['capnhat'] = array(
                '#type' => 'button',
                '#value' => t('Cập nhật'),
                '#executes_submit_callback' => TRUE,
                '#submit' => array('qlhocvien_ghichu_capnhat'),
            );
            $form['mahv_2'] = array(
                '#type' => 'hidden',
                '#value' => $mahv,
            );
            $form['donvi_2'] = array(
                '#type' => 'hidden',
                '#value' => $donvi,
            );
        }
    }

    return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function qlhocvien_ghichu_capnhat($form, &$form_state) {
    global $user;
    $mahv = $form_state['values']['mahv_2'];
    $donvi = $form_state['values']['donvi_2'];
    $ghichu = $form_state['values']['ghichu'];
    $idghichu = db_select('uit_hocvien_ghichu', 't')->fields('t', array('id'))->condition('mahv', $mahv)->condition('donvi', $donvi)->execute()->fetchField();
    try {
        if($idghichu) {
            db_update('uit_hocvien_ghichu')->fields(array(
                'tgcapnhat' => date('Y-m-d'),
                'noidung' => $ghichu,
                'nguoicapnhat' => $user->name,
            ))->condition('id', $idghichu)->execute();
        } else {
            db_insert('uit_hocvien_ghichu')->fields(array(
                'mahv' => $mahv,
                'donvi' => $donvi,
                'tgcapnhat' => date('Y-m-d'),
                'noidung' => $ghichu,
                'nguoicapnhat' => $user->name,
            ))->execute();
        }
        drupal_set_message("Cập nhật thành công");
//        ghilogs('Cập nhật thông tin ghi chú SV', $mahv, 'qlhocvien');
    } catch (Exception $exc) {
        drupal_set_message("Cập nhật bị lỗi (".$exc->getMessage().")", 'error');
    }
}


function qlhocvien_ghichu_tim($form, &$form_state) {
    $mahv = $form_state['values']['mahv'];
    drupal_goto('admin/qlhocvien/ghichu/'.$mahv);
}

function qlhocvien_request_capnhat($form, &$form_state) {
    drupal_goto('admin/qlhocvien/capnhat/'.$form_state['values']['mssv']);
}

























