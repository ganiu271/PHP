<?php
function qlgiangvien_syncportal_form($form, &$form_state, $func=NULL, $para1=NULL, $para2=NULL) {
    global $base_url;
    if( !user_access('UIT qlgiangvien') ) {
        drupal_set_message('Bạn không được phép truy cập chức năng này.','error');
        return;
    }
    $form['information'] = array(
        '#markup' =>
        '<center><h1><font color="blue"><strong>ĐỒNG BỘ THÔNG TIN CB-GV VỚI PORTAL</strong></font></h1></center><br>');
    $options = array(
            '' => 'Chọn chức năng ...',
            'add' => 'Thêm CB-GV từ portal',
            'update' => 'Cập nhật thông tin CB-GV từ portal',
            );
    
    $form['chucnang'] = array(
            '#title'=>t('Chọn chức năng thực hiện:'),
            '#type'=>'select',
            '#options'=>$options,
            '#default_value'=> (isset($func) ? $func:NULL),
            '#attributes' => array('onchange' => 'location.href=\''.$base_url.'/admin/qlgiangvien/syncportal/\'+document.getElementById(\'edit-chucnang\').value;'),
        );
    
    $arrFields = array(
        'username' => 'UserName',
        'hoten' => 'HoTen',
        'ngaysinh' => 'NgaySinh',
        'gioitinh' => 'GioiTinh',
        'noisinh' => 'NoiSinh',
        'hocvi_cn' => 'ChuyenNganh',
        'donvi' => 'MaPB',
        'tinhtrang' => 'MaTT',
        'tongiao' => 'TonGiao',
        'quequan' => 'DiaChiThuongTru',
        'ngayhopdong' => 'NgayKyHDLanDau',
        'hocvi' => 'MaHocVi',
        'hocham' => 'MaHocHam',
        'luong_heso' => 'HeSoLuong',
        'luong_mangach' => 'MaNgach',
        'luong_mocnl' => 'MocNangLuongTT',
        'bienche' => 'BienChe',
        'dangvien' => 'DangVien',
        'cmnd' => 'CMND',
        'cmnd_ngaycap' => 'CMNDNgayCap',
        'cmnd_noicap' => 'CMNDNoiCap',
        'email' => 'Email',
        'dienthoai' => 'DienThoaiDD',
        'masothue' => 'MaSoThue',
        'danhhieunhagiao' => 'DanhHieuNhaGiao',
        'dantoc' => 'MaDT',
        'tongiao' => 'MaTG',
    );
    
    if($func == 'add' && $para1 == '') {
        $n = 0;
        $rows = array();
        $arrCBGV = syncportal_get();
        foreach ($arrCBGV as $CBGV) {
            if(!is_numeric($CBGV->MSCBGV)) continue;
            $check = db_select('uit_thongtingv', 't')->fields('t', array('username'))->condition('magv', $CBGV->MSCBGV)->execute()->fetch();
            if(!$check) {
                $rows[$CBGV->MSCBGV] = array($CBGV->MSCBGV, $CBGV->HoTen, $CBGV->PhongBan_DonVi);
                $n++;
            }
        }
        $headers = array('MaGV', 'Họ tên', 'Đơn vị');
        $form['data_table'] = array(
            '#type' => 'tableselect',
            '#caption' => 'Danh sách CB-GV có bên portal nhưng chưa có bên QLTC',
            '#header' => $headers,
            '#options' => $rows,
            '#empty' => 'Không có CB-GV nào chưa cập nhật'
        );
        $form['capnhat'] = array( '#type' => 'button',
            '#value' => t('Cập nhật'),
            '#attributes' => array('title' =>'Cập nhật các dòng được chọn'),
            '#executes_submit_callback' => TRUE,
            '#submit' => array('synportal_them'),
        );
    }
    
    if($func == 'update') {
        $form['update_field'] = array(
            '#title'=>t('Chọn thông tin cần update:'),
            '#type'=>'select',
            '#options'=> (array(''=>'Chọn thông tin ...') + $arrFields),
            '#default_value'=> $para1,
            '#attributes' => array('onchange' => 'location.href=\''.$base_url.'/admin/qlgiangvien/syncportal/update/\'+document.getElementById(\'edit-update-field\').value;'),
        );
        $form['update_magv'] = array(
            '#title'=>t('Nhập Mã của CB-GV:'),
            '#size'=>20,
            '#type'=>'textfield',
            '#default_value'=> '',
        );
        $form['update_capnhat'] = array( '#type' => 'button',
            '#value' => t('Cập nhật'),
            '#attributes' => array('title' =>'Cập nhật tất cả thông tin của CB-GV'),
            '#executes_submit_callback' => TRUE,
            '#submit' => array('synportal_capnhat_magv'),
        );
        if(key_exists($para1, $arrFields)) {
            $arrCBGV = syncportal_get();
            $rows = array();
            foreach ($arrCBGV as $CBGV) {
                $data = (array)$CBGV;
                if(substr($data['MSCBGV'], 0, 1) == '7') continue;
                $giatri = db_select('uit_thongtingv', 't')->fields('t', array($para1))->condition('magv', $data['MSCBGV'])->execute()->fetchField();
                
                switch ($para1) {
                    //case 'hoten':
                    //    $gtportal = $data['Ho'].' '.$data['Ten'];
                    //    break;
                    case 'ngaysinh':
                    case 'luong_mocnl':
                    case 'cmnd_ngaycap':
                    case 'ngayhopdong':
                        $gtportal = ConvertToDate($data[$arrFields[$para1]]);
                        break;
                    case 'gioitinh':
                        $gtportal = ($data[$arrFields[$para1]] == 'Nam' ? 1:0);
                        break;
                    case 'bienche':
                    case 'dangvien':
                        $gtportal = ($data[$arrFields[$para1]] == 'X' ? 1:0);
                        break;

                    default:
                        $gtportal = $data[$arrFields[$para1]];
                        break;
                }
                
                if($giatri == $gtportal) continue;
                $rows[$data['MSCBGV'].'<=>'.$gtportal] = array(
                    $data['MSCBGV'], 
                    $data['HoTen'], 
                    $gtportal,
                    $giatri,
                    );
            }
            if(count($rows) > 0) {
                $headers = array('Mã CB-GV','Họ tên','Giá trị portal','Giá trị hiện tại');
                $form['data_table'] = array(
                    '#type' => 'tableselect',
                    '#caption' => 'DANH SÁCH KHÁC BIỆT',
                    '#header' => $headers,
                    '#options' => $rows,
                    '#empty' => 'Không có giá trị khác biệt nào'
                );
                $form['capnhat'] = array( '#type' => 'button',
                    '#value' => t('Cập nhật'),
                    '#attributes' => array('title' =>'Cập nhật các dòng được chọn'),
                    '#executes_submit_callback' => TRUE,
                    '#submit' => array('synportal_capnhat'),
                );
                $form['fieldcapnhat'] = array('#type'=>'hidden', '#value'=>$para1);
            }
        }
    }
    
    return $form;
}

function CapNhatThongTinCBGV($magv, $field, $giatri) {
    try {
        db_update('uit_thongtingv')->fields(array($field=>$giatri))->condition('magv', $magv)->execute();
        drupal_set_message('Cập nhật thành công hocvi: '.$CBGV->MSCBGV.' => '.$CBGV->MaHocVi);
    } catch (Exception $exc) {
        drupal_set_message('Phát sinh lỗi ('.$exc->getMessage().')', 'error');
    }
}

function synportal_capnhat($form, &$form_state) {
    $fieldcapnhat = $form_state['values']['fieldcapnhat'];
    $arrID = array_filter($form_state['values']['data_table']);
    $succ = 0;
    foreach ($arrID as $value) {
        $arr = explode('<=>', $value);
        $magv = $arr[0];
        $giatri = $arr[1];
        switch ($fieldcapnhat) {
            case 'ngaysinh':
            case 'luong_mocnl':
            case 'cmnd_ngaycap':
            case 'ngayhopdong':
                if(strlen($giatri) == 7) $giatri = '01/'.$giatri;
                if(strlen($giatri) == 4) $giatri = '01/01/'.$giatri;
                if(strlen($giatri) == 10) $giatri = ConvertToDate($giatri);
                break;
            case 'gioitinh':
                $giatri = ($giatri == 'Nam' ? 1:0);
                break;
            case 'bienche':
            case 'dangvien':
                $giatri = ($giatri == 'X' ? 1:0);
                break;

            default:
                break;
        }
        try {
            db_update('uit_thongtingv')->fields(array($fieldcapnhat => $giatri))->condition('magv', $magv)->execute();
            $succ++;
        } catch (Exception $exc) {
            drupal_set_message('Phát sinh lỗi ('.$exc->getMessage().')', 'error');
        }
    }
    drupal_set_message('Đã cập nhật thành công '.$succ.' dòng');
    drupal_goto('admin/qlgiangvien/syncportal/update/'.$fieldcapnhat);
}

function synportal_capnhat_magv($form, &$form_state) {
    $magv = $form_state['values']['update_magv'];
    update_magv($magv, TRUE);
}

function synportal_them($form, &$form_state, $allowupdate = FALSE) {
    $arrID = array_filter($form_state['values']['data_table']);
    $succ = 0;
    foreach ($arrID as $magv) {
        if(update_magv($magv)) $succ++;
    }
    drupal_set_message('Đã cập nhật thành công '.$succ.' CB-GV.');
    drupal_goto('admin/qlgiangvien/syncportal/add');
}

function update_magv($magv, $allowupdate = FALSE) {
    $arrFields = array(
        'username' => 'UserName',
        'hoten' => 'HoTen',
        'ngaysinh' => 'NgaySinh',
        'gioitinh' => 'GioiTinh',
        'noisinh' => 'NoiSinh',
        'hocvi_cn' => 'ChuyenNganh',
        'donvi' => 'MaPB',
        'tinhtrang' => 'MaTT',
        'tongiao' => 'TonGiao',
        'quequan' => 'DiaChiThuongTru',
        'donvi' => 'MaPB',
        'ngayhopdong' => 'NgayKyHDLanDau',
        'hocvi' => 'MaHocVi',
        'hocham' => 'MaHocHam',
        'luong_heso' => 'HeSoLuong',
        'luong_mangach' => 'MaNgach',
        'luong_mocnl' => 'MocNangLuongTT',
        'bienche' => 'BienChe',
        'dangvien' => 'DangVien',
        'cmnd' => 'CMND',
        'cmnd_ngaycap' => 'CMNDNgayCap',
        'cmnd_noicap' => 'CMNDNoiCap',
        'email' => 'Email',
        'dienthoai' => 'DienThoaiDD',
        'masothue' => 'MaSoThue',
        'danhhieunhagiao' => 'DanhHieuNhaGiao',
        'dantoc' => 'MaDT',
        'tongiao' => 'MaTG',
    );
    $arrCBGV = syncportal_get($magv);
    if($arrCBGV) {
        $CBGV = (array)$arrCBGV[0];
        $fields = array('magv'=>$CBGV['MSCBGV']);
        foreach ($arrFields as $key => $value) {
            switch ($key) {
                case 'ngaysinh':
                case 'luong_mocnl':
                case 'cmnd_ngaycap':
                case 'ngayhopdong':
                    $fields[$key] = ConvertToDate($CBGV[$value]);
                    break;
                case 'gioitinh':
                    $fields[$key] = ($CBGV[$value] == 'Nam' ? 1:0);
                    break;
                case 'username':
                    $fields[$key] = ($CBGV[$value] ? $CBGV[$value]:$magv);
                    break;
                case 'bienche':
                case 'dangvien':
                    $fields[$key] = ($CBGV[$value] == 'X' ? 1:0);
                    break;

                default:
                    $fields[$key] = $CBGV[$value];
                    break;
            }
        }
        $check = db_select('uit_thongtingv', 't')->fields('t', array('id'))->condition('magv', $fields['magv'])->execute()->fetchField();
        if($check) {
            if($allowupdate) {
                $magv = $fields['magv'];
                unset($fields['magv']);
                db_update('uit_thongtingv')->fields($fields)->condition('magv', $magv)->execute();
                drupal_set_message('Đã cập nhật thành công magv: '.$magv.' ('.$fields['hoten'].')');
            } else {
                drupal_set_message($fields['magv'].' đã có trên hệ thống.');
            }
        } else {
            try {
                db_insert('uit_thongtingv')->fields($fields)->execute();
                drupal_set_message('Đã thêm thành công magv: '.$fields['magv'].' ('.$fields['hoten'].')');
                /*
                if($fields['luong_heso'] > 0 && $fields['luong_mangach'] != '') {
                    $check = db_select('qltc_thamsogv', 't')->fields('t', array('id'))->condition('magv', $fields['magv'])
                            ->condition('tenthamso', 'luong')->condition('tuthang', (int)date('m', strtotime($fields['ngayhopdong'])))
                            ->condition('tunam', (int)date('Y', strtotime($fields['ngayhopdong'])))->execute()->fetchField();
                    if(!$check) {
                        db_insert('qltc_thamsogv')->fields(array(
                            'magv' => $fields['magv'],
                            'tenthamso' => 'luong',
                            'giatri' => $fields['luong_heso'],
                            'tungay' => (int)date('d', strtotime($fields['ngayhopdong'])),
                            'tuthang' => (int)date('m', strtotime($fields['ngayhopdong'])),
                            'tunam' => (int)date('Y', strtotime($fields['ngayhopdong'])),
                        ))->execute();
                    }
                }
                 * 
                 */
                return TRUE;
            } catch (Exception $exc) {
                drupal_set_message('Có lỗi xãy ra ('.$exc->getMessage().')', 'error');
            }
        }
    }
    return FALSE;
}