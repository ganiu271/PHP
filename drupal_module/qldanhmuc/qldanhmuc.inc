<?php
function qldanhmuc_form($form, &$form_state, $formid=NULL, $cmd=NULL, $itemid=NULL) {
    global $base_url;
    if(!user_access('Quản lý danh mục') && !user_access('Xem danh mục') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>QUẢN LÝ DANH MỤC</h1></center>';
    $form['markup'] = array('#markup' => $html);
    $options = array(
            '' => 'Chọn danh mục cần xem',
            //'monhoc' => 'Danh sách môn học',
            'coso' => 'Cơ sở liên kết',
            'ctdt' => 'Chương trình đào tạo',
            );
    $form['viewform'] = array(
            '#title'=>t('Chọn thông tin cần xem:'),
            '#type'=>'select',
            '#options'=> $options,
            '#default_value'=> (isset($formid) ? $formid:NULL),
//            '#attributes' => array('onchange' => 'location.href=\''.$base_url.'/admin/qldanhmuc/\'+document.getElementById(\'edit-viewform\').value;'),
            '#attributes' => array('onchange' => 'location.href=\''.$base_url.'/?q=admin/qldanhmuc/\'+document.getElementById(\'edit-viewform\').value;'),
    );

    if($formid != '') {
        unset($_SESSION['qldanhmuc_filter']); // không nên bỏ
        $form['filter_wraper'] = array(
            '#type' => 'fieldset',
            '#title' => t('Chọn điều kiện lọc'),
            '#attributes' => array('class' => array('container-inline')),
            '#collapsible' => TRUE,
            '#collapsed' => FALSE);
        qldanhmuc_set_filters($form_state, $formid);
        $form['filter_wraper'][] = qldanhmuc_filter_form($formid);
        $form['data'] = array(
            '#prefix' => '<div id="table_data">',
            '#suffix' => '</div>',);
        $form['data']['data_info'] = qldanhmuc_form_info($formid);
        if ($_GET['q'] == 'system/ajax') {
            $q = 'admin/qldanhmuc/'.$formid;
            $_GET['q'] = $q;
        }
        $form['data']['data_table'] = qldanhmuc_get_data($formid);
        $form['data']['pager'] = array('#markup' => theme('pager'));
        $form['data']['congcu'] = array('#type' => 'fieldset', '#title' => t('Công cụ xử lý'), '#attributes' => array('class' => array('container-inline')));
        $form['data']['congcu'][] = qldanhmuc_chucnang_form($formid);
        $form['data']['formid'] = array( '#type' => 'hidden', '#value' => $formid,);
    }
    
    return $form;
}

function qldanhmuc_xoa($form, &$form_state) {
    $formid = $form_state['values']['formid'];
    $arrID = array_filter($form_state['values']['data_table']);
    if(count($arrID) == 0) return;
    $succ = 0;
    $strLogs = '';
    foreach ($arrID as $rid) {
        switch ($formid) {
            case 'coso':
                $macs = db_select('uit_coso', 't')->fields('t', array('macs'))->condition('id', $rid)->execute()->fetchField();
                if($macs) {
                   //Kiểm tra ràng buộc trong uit_thongtinsv
                   $check = db_select('uit_thongtinsv', 't')->fields('t',array('coso'))->condition('coso', $macs)->range(0, 1)->execute()->fetch();
                   if($check) {
                       drupal_set_message('Cơ sở có mã '.$macs.' không xóa được do dữ liệu còn liên quan', 'error');
                       continue;
                   }
                   if(XoaDanhMuc($rid, 'uit_coso')) {
                       $succ++;
                       $strLogs .= $macs.';';
                   }
                }
                break;
            case 'ctdt':
                $macs = db_select('uit_quanly_ctdt', 't')->fields('t', array('ma_ctdaotao'))->condition('id', $rid)->execute()->fetchField();
                if($macs) {
                   //Kiểm tra ràng buộc trong uit_thongtinsv
                   $check = db_select('uit_ctdaotao', 't')->fields('t',array('id'))->condition('mactdt', $macs)->range(0, 1)->execute()->fetch();
                   if($check) {
                       drupal_set_message('CTĐT có mã '.$macs.' không xóa được do dữ liệu còn liên quan', 'error');
                       continue;
                   }
                   if(XoaDanhMuc($rid, 'uit_quanly_ctdt')) {
                       $succ++;
                       $strLogs .= $macs.';';
                   }
                }
                break;
            case 'monhoc':
                $mamh = db_select('uit_monhoc', 't')->fields('t', array('mamh'))->condition('id', $rid)->execute()->fetchField();
                if($mamh) {
                   //Kiểm tra ràng buộc trong TKB
                   $check = db_select('uit_tkb', 't')->fields('t',array('mamh'))->condition('mamh', $mamh)->range(0, 1)->execute()->fetch();
                   if($check) {
                       drupal_set_message('Mã môn học '.$mamh.' không xóa được do dữ liệu còn liên quan', 'error');
                       continue;
                   }
                   //Kiểm tra ràng buộc trong DIEM
                   $check = db_select('uit_diem', 't')->fields('t',array('mamh'))->condition('mamh', $mamh)->range(0, 1)->execute()->fetch();
                   if($check) {
                       drupal_set_message('Mã môn học '.$mamh.' không xóa được do dữ liệu còn liên quan', 'error');
                       continue;
                   }
                   if(XoaDanhMuc($rid, 'uit_monhoc')) {
                       $succ++;
                       $strLogs .= $mamh.';';
                   }
                }
                break;

            default:
                break;
        }
    }
    if($succ > 0) {
        drupal_set_message('Đã xóa thành công '.$succ.' dòng ('.$strLogs.')');
//        ghilogs('Xóa danh mục '.$formid.' các mã: '.$strLogs, $formid, 'qldanhmuc');
    }
    return;
}

function XoaDanhMuc($rid, $table) {
    try {
        db_delete($table)->condition('id', $rid)->execute();
    } catch (Exception $exc) {
        drupal_set_message('Có lỗi xãy ra ('.$exc->getMessage().')', 'error');
        return FALSE;
    }
    return TRUE;
}

function qldanhmuc_export($form, &$form_state) {
    $formid = $form_state['values']['formid'];
    if($formid == '') return;
    $filepath = Export_Excel($formid);
    if(file_exists($filepath)) {
        drupal_add_http_header('Pragma', 'public');
        drupal_add_http_header('Expires', '0');
        drupal_add_http_header('Cache-Control', 'must-revalidate, post-check=0, pre-check=0');
        drupal_add_http_header('Content-Type', 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
        drupal_add_http_header('Content-Disposition', 'attachment; filename=' . basename($filepath) . ';');
        drupal_add_http_header('Content-Transfer-Encoding', 'binary');
        drupal_add_http_header('Content-Length', filesize($filepath));
        readfile($filepath);
        unlink($filepath);
        drupal_exit();
    }
    return;
}

function Export_Excel($formid) {
    $fields = array();
    $rows = array();
    switch ($formid) {
        case 'coso':
            $fields = $_SESSION['qldanhmuc_fields_'.$formid];
            $q = db_select('uit_coso', 't');
            $q->fields('t');
            qldanhmuc_build_filter_query($q);
            $rows = $q->execute()->fetchAll();
            break;
        case 'ctdt':
            $fields = $_SESSION['qldanhmuc_fields_'.$formid];
            $q = db_select('uit_quanly_ctdt', 't');
            $q->fields('t');
            qldanhmuc_build_filter_query($q);
            $rows = $q->execute()->fetchAll();
            break;
        case 'monhoc':
            $fields = $_SESSION['qldanhmuc_fields_'.$formid];
            $q = db_select('uit_monhoc', 't');
            $q->fields('t');
            qldanhmuc_build_filter_query($q);
            $rows = $q->execute()->fetchAll();
            break;

        default:
            return;
    }
    
//    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    require_once '/var/www/html/qldt/sites/all/libraries/PHPExcel.php';
    $filepath = '/var/www/html/qldt/sites/default/files/exports';
//    $filepath = 'sites/default/files/exports';
    if(!file_exists($filepath)) mkdir($filepath);
    $filepath .= '/'.$formid.'_'.date('Ymd_his').'.xlsx';
    
    $objPHPExcel = new PHPExcel();
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    $c = 'A';
    $r = 1;
    //Headers
    foreach ($fields as $key => $value) {
        $ActiveSheet->setCellValue($c++.$r, $value);
    }
    //Rows
    foreach ($rows as $row) {
        $c = 'A';
        $r++;
        $data = (array)$row;
        foreach ($fields as $key => $value) {
            $ActiveSheet->setCellValue($c++.$r, $data[$key]);
        }
    }
    $objWriter = new PHPExcel_Writer_Excel2007($objPHPExcel);
    ob_end_clean();
    $objWriter->save($filepath);
    
    return $filepath;
}

function qldanhmuc_set_filters(&$form_state, $formid) {
    //unset($_SESSION['qldanhmuc_filter']);
    $filters = qldanhmuc_filters($formid);
    foreach ($filters as $filter => $options) {
      if (isset($form_state['values'][$filter])) {
        if( $form_state['values'][$filter] != '') {
          $_SESSION['qldanhmuc_filter'][$filter] = $form_state['values'][$filter];
        }
        else {
          unset($_SESSION['qldanhmuc_filter'][$filter]);
        }
      } 
    }
}

function qldanhmuc_filter_form($formid) {
    $form = array();
    $filters = qldanhmuc_filters($formid);
    foreach ($filters as $key => $filter) {
        $arr = array();
        foreach ($filter as $key2 => $value2) {
            $arr['#'.$key2] = $value2;
        }
        $form['filters'][$key] = $arr;
    }
    return $form;
}

function qldanhmuc_chucnang_form($formid) {
    global $base_url;
    $form = array();
    switch ($formid) {
        case 'monhoc':
            break;
        case 'ctdt':
            $form['export'] = array(
                '#type' => 'button',
                '#value' => t('Export'),
                '#attributes' => array('title' =>'Xuất dữ liệu ra file excel'),
                '#executes_submit_callback' => TRUE,
                '#submit' => array('qldanhmuc_export'),
            );
            $form['delete'] = array(
                '#type' => 'button',
                '#value' => t('Xóa'),
                '#attributes' => array('onclick'=>'return confirm(\'Bạn có chắc xóa các dòng được chọn ?\');','title' =>'Xóa các dòng được chọn'),
                '#executes_submit_callback' => TRUE,
                '#submit' => array('qldanhmuc_xoa'),
            );
            $form['import_single'] = array(
                '#type' => 'button',
                '#value' => t('Import '),
                '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/?q=admin/qldanhmuc/edit/-1'.'\');','title' =>'Import dữ liệu từ file excel'),
                '#executes_submit_callback' => FALSE,
            );
            $form['import'] = array(
                '#type' => 'button',
                '#value' => t('Import Excel'),
                '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/?q=admin/qldanhmuc/import/'.$formid.'\');','title' =>'Import dữ liệu từ file excel'),
                '#executes_submit_callback' => FALSE,
            );
            break;
        case 'coso':
            $form['export'] = array( 
                '#type' => 'button',
                '#value' => t('Export'),
                '#attributes' => array('title' =>'Xuất dữ liệu ra file excel'),
                '#executes_submit_callback' => TRUE,
                '#submit' => array('qldanhmuc_export'),
            );
            $form['delete'] = array( 
                '#type' => 'button',
                '#value' => t('Xóa'),
                '#attributes' => array('onclick'=>'return confirm(\'Bạn có chắc xóa các dòng được chọn ?\');','title' =>'Xóa các dòng được chọn'),
                '#executes_submit_callback' => TRUE,
                '#submit' => array('qldanhmuc_xoa'),
            );
            $form['import'] = array( 
                '#type' => 'button',
                '#value' => t('Import'),
                '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/admin/qldanhmuc/import/'.$formid.'\');','title' =>'Import dữ liệu từ file excel'),
                '#executes_submit_callback' => FALSE,
            );
            break;
        
        default:
            break;
    }
    return $form;
}

function qldanhmuc_get_data($formid) {
    $headers = array();
    $options = array();
    $fields = array();
    $fieldkey = null;
    switch ($formid) {
        case 'coso':
            $fieldkey = 'macs';
            if(!isset($_SESSION['qldanhmuc_fields_'.$formid])) {
                $_SESSION['qldanhmuc_fields_'.$formid] = array(
                    'macs'=>'Mã CS',
                    'tencs'=>'Tên cơ sở',
                    'tenvt'=>'Tên viết tắt',
                    'chuquan'=>'Chủ quản',
                    'diachi'=>'Địa chỉ',
                    'daidien'=>'Đại diện',
                    'dienthoai'=>'Điện thoại',
                    'ghichu'=>'Ghi chú');
            }
            $fields = $_SESSION['qldanhmuc_fields_'.$formid];
            foreach ($fields as $key=>$value) {
                $headers[] = array('data' => $value, 'field' => $key);
            }
            $q = db_select('uit_coso', 't')
                ->extend('PagerDefault')->limit(50)->extend('TableSort')->orderByHeader($headers);
            $q->fields('t');
            qldanhmuc_build_filter_query($q);
            $rows = $q->execute()->fetchAll();
            break;
        case 'monhoc':
            $fieldkey = 'mamh';
            if(!isset($_SESSION['qldanhmuc_fields_'.$formid])) {
                $_SESSION['qldanhmuc_fields_'.$formid] = array('mamh'=>'Mã MH','tenmh'=>'Tên MH','loaimh'=>'Loại MH',
                    'sotinchi'=>'Số TC','sotc_lt'=>'Số TCLT','sotc_th'=>'Số TCTH',
                    'nganh'=>'Ngành','hedt'=>'Hệ ĐT','ghichu'=>'Ghi chú');
            }
            $fields = $_SESSION['qldanhmuc_fields_'.$formid];
            foreach ($fields as $key=>$value) {
                $headers[] = array('data' => $value, 'field' => $key);
            }
            $q = db_select('uit_monhoc', 't')->extend('PagerDefault')->limit(50)->extend('TableSort')->orderByHeader($headers);
            $q->fields('t');
            qldanhmuc_build_filter_query($q);
            $rows = $q->execute()->fetchAll();
            break;
        case 'ctdt':
            $fieldkey = 'ma_ctdaotao';
            if(!isset($_SESSION['qldanhmuc_fields_'.$formid])) {
                $_SESSION['qldanhmuc_fields_'.$formid] = array(
                    'ma_ctdaotao'=>'Mã CTĐT',
                    'ten_ctdaotao'=>'Tên CTĐT',
                    'hedt'=>'Hệ ĐT',
                    'khoa_daotao'=>'Khóa đào tạo',
                    'giaidoan'=>'Giai đoạn',
                    'thoigian_daotao'=>'Thời gian ĐT',
                    'thoigian_batdau'=>'Thời gian bắt đầu',
                    'ghichu'=>'Ghi chú');
            }
            $fields = $_SESSION['qldanhmuc_fields_'.$formid];
            foreach ($fields as $key=>$value) {
                $headers[] = array('data' => $value, 'field' => $key);
            }
            $q = db_select('uit_quanly_ctdt', 't')
                ->extend('PagerDefault')
                ->limit(50)
                ->extend('TableSort')
                ->orderByHeader($headers);
            $q->fields('t');
            qldanhmuc_build_filter_query($q);
            $rows = $q->execute()->fetchAll();
            break;

        default:
            break;
    }
    //$n = 0;
    foreach ($rows as $row) {
        $data = (array)$row;
        foreach ($fields as $key=>$value) {
            $val = $data[$key];
            if($key == $fieldkey) {
                if(isset($row->id)) {
                    $options[$row->id][] = l($val,'admin/qldanhmuc/edit/'.$formid.'/'.$row->id,array('attributes'=>array('target'=>'_Blank')));
                } else {
                    $options[$data[$key]][] = l($val,'admin/qldanhmuc/edit/'.$formid.'/'.$row->id,array('attributes'=>array('target'=>'_Blank')));
                }
                continue;
            }
            if(isset($row->id)) {
                $options[$row->id][] = $val;
            } else {
                $options[$data[$key]][] = $val;
            }
        }
        //$n++;
    }
    $form = array(
        '#title' => 'Table',
        '#type' => 'tableselect',
        '#header' => $headers,
        '#options' => $options,
        '#empty' => t('Không tìm thấy thông tin.'),
    );
    return $form;
}

function qldanhmuc_form_info($formid) {
    $total = 0;
    $form = array();
    switch ($formid) {
        case 'coso':
            $q = db_select('uit_coso', 't');
            $q->addExpression('count(id)');
            qldanhmuc_build_filter_query($q);
            $total = $q->execute()->fetchField();
            break;
        case 'monhoc':
            $q = db_select('uit_monhoc', 't');
            $q->addExpression('count(id)');
            qldanhmuc_build_filter_query($q);
            $total = $q->execute()->fetchField();
            break;
        case 'ctdt':
            $q = db_select('uit_quanly_ctdt', 't');
            $q->addExpression('count(ma_ctdaotao)');
            qldanhmuc_build_filter_query($q);
            $total = $q->execute()->fetchField();
            break;
        
        default:
            break;
    }
    if($total > 0) {
        $form = array('#markup' =>'<strong>Tổng cộng: '.number_format($total).'</strong>');
    }
    return $form;
}

function qldanhmuc_filters($formid) {
    $filters = array();
    $session = isset($_SESSION['qldanhmuc_filter']) ? $_SESSION['qldanhmuc_filter'] : array();
    switch ($formid) {
        case 'monhoc':
            $mamh = !empty($session['mamh']) ? $session['mamh'] : NULL;
            $tenmh = !empty($session['tenmh']) ? $session['tenmh'] : NULL;
//            $nganh = !empty($session['nganh']) ? $session['nganh'] : NULL;
            $hedt = !empty($session['hedt']) ? $session['hedt'] : NULL;
            
            $filters['mamh'] = array(
                'type' => 'textfield',
                'title' => t('Mã MH:'),
                'size' => 30,
                'default_value' => array($mamh),
                'attributes' => array('title' =>'Có thể nhập nhiều mã môn học cách nhau bởi dấu phẩy (,).'),
            );
            $filters['tenmh'] = array(
                'type' => 'textfield',
                'title' => t('Tên MH:'),
                'size' => 30,
                'default_value' => array($tenmh),
            );
//            $filters['nganh'] = array(
//                'type' => 'select',
//                'title' => t('Ngành:'),
//                'options' => (array(''=>'Tất cả') + db_select('uit_nganh', 'd')->distinct()->orderBy('d.nganh')->fields('d', array('nganh', 'nganh'))->execute()->fetchAllKeyed()),
//                'default_value' => array($nganh),
//            );
            $filters['hedt'] = array(
                'type' => 'select',
                'title' => t('Hệ ĐT:'),
                'options' => (array(''=>'Tất cả') + db_select('uit_hedt', 'd')->distinct()->orderBy('d.kyhieu')->fields('d', array('kyhieu', 'kyhieu'))->execute()->fetchAllKeyed()),
                'default_value' => array($hedt),
            );
            
            break;

        case 'ctdt':
            $hedt = !empty($session['hedt']) ? $session['hedt'] : NULL;
            $ma_ctdaotao = !empty($session['ma_ctdaotao']) ? $session['ma_ctdaotao'] : NULL;
            $ten_ctdaotao = !empty($session['ten_ctdaotao']) ? $session['ten_ctdaotao'] : NULL;
            $khoa_daotao = !empty($session['khoa_daotao']) ? $session['khoa_daotao'] : NULL;
            $giaidoan = !empty($session['giaidoan']) ? $session['giaidoan'] : NULL;
            $filters['ma_ctdaotao'] = array(
                'type' => 'textfield',
                'title' => t('Mã CTĐT:'),
                'size' => 30,
                'default_value' => array($ma_ctdaotao),
                'attributes' => array('title' =>'Có thể nhập nhiều mã môn học cách nhau bởi dấu phẩy (,).'),
            );
            $filters['ten_ctdaotao'] = array(
                'type' => 'textfield',
                'title' => t('Tên CTĐT:'),
                'size' => 30,
                'default_value' => array($ten_ctdaotao),
            );
            $filters['hedt'] = array(
                'type' => 'select',
                'title' => t('Hệ ĐT:'),
                'options' => (array(''=>'Tất cả') + db_select('uit_hedt', 'd')->distinct()->orderBy('d.kyhieu')->fields('d', array('kyhieu', 'kyhieu'))->execute()->fetchAllKeyed()),
                'default_value' => array($hedt),
            );
            $filters['khoa_daotao'] = array(
                'type' => 'select',
                'title' => t('Khóa đào tạo:'),
                'options' => (array(''=>'Tất cả') + db_select('uit_quanly_ctdt', 'd')->distinct()->orderBy('d.id')->fields('d', array('khoa_daotao', 'khoa_daotao'))->execute()->fetchAllKeyed()),
                'default_value' => array($khoa_daotao),
            );
            $filters['giaidoan'] = array(
                'type' => 'select',
                'title' => t('Giai đoạn:'),
                'options' => (array(''=>'Tất cả','1'=>'1','2'=>'2')),
                'default_value' => array($giaidoan),
            );
            break;
        default:
            break;
    }
    if(count($filters) > 0) {
        $filters['timkiem'] = array(
            'type' => 'button',
            'value' => t('Tìm kiếm'),
            'ajax' => array(
              'event' => 'click',
              'callback' => 'ajax_filter_qldanhmuc_callback',
              'wrapper' => 'table_data',
            ),
        );
    }
    return $filters;
}

function qldanhmuc_build_filter_query(SelectQueryInterface $query) {
    $filter_data = isset($_SESSION['qldanhmuc_filter']) ? $_SESSION['qldanhmuc_filter'] : array();
    foreach ($filter_data as $key => $value) {
        switch ($key) {
            case 'timkiem':
                break;
            case 'hedt':
                $query->condition($key, '%'.$value.'%', 'LIKE');
                break;
            case 'tenmh':
                $query->where('(tenmh like \'%'.$value.'%\' or tenta like \'%'.$value.'%\' )');
                break;
            case 'ten_ctdaotao':
                $query->condition($key, '%'.$value.'%', 'LIKE');
                break;
            case 'ma_ctdaotao':
                $arrmactdt = explode(',', $value);
                $query->condition('t.'.$key, $arrmactdt, 'IN');
                break;
            default:
                $query->condition($key, $value);
        }
    }
}

function ajax_filter_qldanhmuc_callback($form, $form_state) {
  return $form['data'];
}

function qldanhmuc_edit($form, &$form_state, $formid, $rid) {
    switch ($formid) {
        case 'coso':
            $row = db_select('uit_coso','t')->fields('t')->condition('id',$rid)->execute()->fetch();
            if(!$row) return;
            $data = (array)$row;
            
            foreach ($_SESSION['qldanhmuc_fields_coso'] as $key => $value) {
                $form[$key] = array(
                    '#type' => 'textfield',
                    '#title' => $value,
                    '#size' => 120,
                    '#default_value' => array($data[$key]),
                    '#disabled' => ($key == 'macs' ? TRUE:FALSE),
                );
            }
            break;
        
        case 'ctdt':
            $row=NULL;
            $data=NULL;
            $key_disabled='';
            if($rid!=-1){
                $row = db_select('uit_quanly_ctdt','t')->fields('t')->condition('id',$rid)->execute()->fetch();
                if($row==FALSE){
                    drupal_set_message('Có lỗi xãy ra : '.$rid.' không tồn tại ', 'error');
                    drupal_goto('admin/qldanhmuc/ctdt');
                }else{
                    $key_disabled='ma_ctdaotao';
                    $data = (array)$row;
                }
            }
            foreach ($_SESSION['qldanhmuc_fields_ctdt'] as $key => $value) {
                if($key == 'hedt') {
                    $form[$key] = array(
                        '#type' => 'select',
                        '#title' => $value,
                        '#options' => db_select('uit_hedt', 'd')->distinct()->orderBy('d.kyhieu')->fields('d', array('kyhieu', 'kyhieu'))->execute()->fetchAllKeyed(),
                        '#default_value' => array($data[$key]),
                    );
                }//if($key='ma_ctdaotao'){ }
                else {
                    $form[$key] = array(
                        '#type' => 'textfield',
                        '#title' => $value,
                        '#size' => 120,
                        '#default_value' => array($data[$key]),
                        '#disabled' => ($key == $key_disabled ? TRUE:FALSE),
                    );
                }
            }
            break;
        
        default:
            break;
    }
    $form['formid'] = array( 
        '#type' => 'hidden',
        '#default_value' => $formid,
    );
    $form['rid'] = array( 
        '#type' => 'hidden',
        '#default_value' => $rid,
    );
    $form['capnhat'] = array( 
        '#type' => 'button',
        '#value' => t('Cập nhật'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qldanhmuc_capnhat'),
    );
    $form['dong'] = array(
        '#type' => 'button', 
        '#value' => t('Đóng'),
        '#executes_submit_callback' => FALSE,
        '#attributes' => array('onclick'=>'window.close();'),
    );
    
    return $form;
}

function qldanhmuc_capnhat($form, &$form_state) {
    $formid = $form_state['values']['formid'];
    $rid = $form_state['values']['rid'];
    if($formid == '' || $rid == '') return FALSE;
    switch ($formid) {
        case 'coso':
            $tencs = trim($form_state['values']['tencs']);
            $tenvt = trim($form_state['values']['tenvt']);
            if($tencs == '' || $tenvt == '') {
                drupal_set_message('Thông tin tên cơ sở và tên viết tắt phải có','error');
                return FALSE;
            }
            try {
            db_update('uit_coso')->fields(array(
                'tencs' => $tencs,
                'tenvt' => $tenvt,
                'chuquan' => trim($form_state['values']['chuquan']),
                'diachi' => trim($form_state['values']['diachi']),
                'daidien' => trim($form_state['values']['daidien']),
                'dienthoai' => trim($form_state['values']['dienthoai']),
                'ghichu' => trim($form_state['values']['ghichu']),
            ))->condition('id',$rid)->execute();
            drupal_set_message('Cập nhật thành công');
            } catch (Exception $exc) {
                drupal_set_message('Có lỗi xãy ra: '.$exc->getMessage(),'error');
                return FALSE;
            }
            break;
        
        case 'ctdt':
            $tenctdt = trim($form_state['values']['ten_ctdaotao']);
            if($tenctdt == '') {
                drupal_set_message('Thông tin tên CTĐT phải có','error');
                return FALSE;
            }
            if($rid==-1){
                try {
                    db_insert('uit_quanly_ctdt')->fields(array(
                        'ma_ctdaotao'=>trim($form_state['values']['ma_ctdaotao']),
                        'ten_ctdaotao' => $tenctdt,
                        'hedt' => $form_state['values']['hedt'],
                        'khoa_daotao' => trim($form_state['values']['khoa_daotao']),
                        'giaidoan'=>trim($form_state['values']['giaidoan']),
                        'thoigian_daotao' => trim($form_state['values']['thoigian_daotao']),
                        'thoigian_batdau'=>trim($form_state['values']['thoigian_batdau']),
                        'ghichu' => trim($form_state['values']['ghichu']),
                    ))->execute();
                    drupal_set_message('Cập nhật thành công');
                } catch (Exception $exc) {
                    drupal_set_message('Có lỗi xãy ra: '.$exc->getMessage(),'error');
                    return FALSE;
                }
            }else{
                $transaction=db_transaction();
                try {
                    db_update('uit_quanly_ctdt')->fields(array(
                        'ten_ctdaotao' => $tenctdt,
                        'hedt' => $form_state['values']['hedt'],
                        'khoa_daotao' => trim($form_state['values']['khoa_daotao']),
                        'giaidoan'=>trim($form_state['values']['giaidoan']),
                        'thoigian_daotao' => trim($form_state['values']['thoigian_daotao']),
                        'thoigian_batdau'=>trim($form_state['values']['thoigian_batdau']),
                        'ghichu' => trim($form_state['values']['ghichu']),
                    ))->condition('id',$rid)->execute();
                    drupal_set_message('Cập nhật thành công');
                } catch (Exception $exc) {
                    drupal_set_message('Có lỗi xãy ra: '.$exc->getMessage(),'error');
                    $transaction->rollback();
                    return FALSE;
                }
            }
            break;
        
        default:
            break;
    }
    return TRUE;
}