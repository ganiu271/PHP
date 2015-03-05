<?php
function qlmonhoc_form($form, &$form_state, $formid=NULL, $cmd=NULL, $itemid=NULL) {
    global $base_url;
    if(!user_access('Quản lý môn học') && !user_access('Xem môn học') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>QUẢN LÝ MÔN HỌC</h1></center>';
    $form['markup'] = array('#markup' => $html);
    $options = array(
            '' => 'Chọn danh mục cần xem',
            'monhoc' => 'Danh sách môn học',
            'monhoctd' => 'Môn học tương đương',
            );
    $form['viewform'] = array(
            '#title'=>t('Chọn thông tin cần xem:'),
            '#type'=>'select',
            '#options'=> $options,
            '#default_value'=> (isset($formid) ? $formid:NULL),
            '#attributes' => array('onchange' => 'location.href=\''.$base_url.'/?q=admin/qlmonhoc/\'+document.getElementById(\'edit-viewform\').value;'),
    );
    
    if($formid != '') {
        if($cmd == 'edit' && $itemid != '') {
           $form['edit_wraper'] = array('#type' => 'fieldset', '#title' => t('Cập nhật'),
                '#attributes' => array('class' => array('container-inline')), '#collapsible' => TRUE, '#collapsed' => FALSE); 
           $form['edit_wraper'][] = qlmonhoc_edit_form($formid, $itemid);
           $form['edit_wraper']['capnhat'] = array(
                '#value'=>t('Cập nhật'),
                '#type'=>'button',
                '#executes_submit_callback' => TRUE,
                '#submit' => array('qlmonhoc_capnhat'),
                '#validate' => array('qlmonhoc_capnhat_validate'),
            );
        }
        $form['filter_wraper'] = array('#type' => 'fieldset', '#title' => t('Chọn điều kiện lọc'),
            '#attributes' => array('class' => array('container-inline')), '#collapsible' => TRUE, '#collapsed' => FALSE);
        qlmonhoc_set_filters($form_state, $formid);
        $form['filter_wraper'][] = qlmonhoc_filter_form($formid);
        $form['data'] = array(
            '#prefix' => '<div id="table_data">',
            '#suffix' => '</div>',);
        $form['data']['data_info'] = qlmonhoc_form_info($formid);
        if ($_GET['q'] == 'system/ajax') {
            $q = 'admin/qlmonhoc/'.$formid;
            $_GET['q'] = $q;
        }
        $form['data']['data_table'] = qlmonhoc_get_data($formid);
        $form['data']['pager'] = array('#markup' => theme('pager'));
        $form['data']['congcu'] = array('#type' => 'fieldset', '#title' => t('Công cụ xử lý'), '#attributes' => array('class' => array('container-inline')));
        $form['data']['congcu'][] = qlmonhoc_chucnang_form($formid);
        $form['data']['formid'] = array( '#type' => 'hidden', '#value' => $formid,);
    }
    
    return $form;
}

function qlmonhoc_edit_form($formid, $itemid) {
    if($formid == '' || $itemid == '') return;
    $form = array();
    switch ($formid) {
     case 'monhoc':
        $arrUpdate = db_select('uit_monhoc', 't')->fields('t')->condition('id', $itemid)->execute()->fetch();
        if(!$arrUpdate) return;
        $form['id_update'] = array(
                '#type'=>'hidden',
                '#default_value'=> (isset($arrUpdate->id) ? $arrUpdate->id : ''),
        );
        //$form['mamhbandau_update'] = array(
        //        '#type'=>'hidden',
        //        '#default_value'=> (isset($arrUpdate->mamh) ? $arrUpdate->mamh : ''),
        //);
        $form['mamh_update'] = array(
                '#title'=>t('Mã MH:'),
                '#size'=>10,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->mamh) ? $arrUpdate->mamh : ''),
                '#disabled'=>TRUE,
        );
        $form['tenmh_update'] = array(
                '#title'=>t('Tên MH:'),
                '#size'=>20,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->tenmh) ? $arrUpdate->tenmh : ''),
                //'#required'=>TRUE,
        );
        $form['tenta_update'] = array(
                '#title'=>t('Tên tiếng Anh:'),
                '#size'=>20,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->tenta) ? $arrUpdate->tenta : ''),
                //'#required'=>TRUE,
        );
        $form['sotc_update'] = array(
                '#title'=>t('Số TC:'),
                '#size'=>4,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->sotinchi) ? $arrUpdate->sotinchi : 0),
                //'#required'=>TRUE,
        );
        $form['sotc_lt_update'] = array(
                '#title'=>t('TCLT:'),
                '#size'=>4,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->sotc_lt) ? $arrUpdate->sotc_lt : 0),
                //'#required'=>TRUE,
        );
        $form['sotc_th_update'] = array(
                '#title'=>t('TCTH:'),
                '#size'=>4,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->sotc_th) ? $arrUpdate->sotc_th : 0),
                //'#required'=>TRUE,
        );
        $form['hinhthucth_update'] = array(
                '#title'=>t('HTTH:'),
                '#type'=>'select',
                '#options'=>array(0=>'Không TH',1=>'HT1',2=>'HT2'),
                '#default_value'=> (isset($arrUpdate->hinhthucth) ? $arrUpdate->hinhthucth : 1),
        );
        $form['loaimh_update'] = array(
                '#title'=>t('<br>Loại MH:'),
                '#type'=>'select',
                '#options' => db_select('uit_danhmuc', 't')->fields('t', array('tenvt', 'tenvt'))->condition('loaidm', 'loaimh')->orderBy('thutu')->execute()->fetchAllKeyed(),
                '#default_value'=> (isset($arrUpdate->loaimh) ? $arrUpdate->loaimh : NULL),
        );
        $form['khoaql_update'] = array(
                '#title'=>t('Khoa QL:'),
                '#type'=>'select',
                '#options' => db_select('uit_khoa', 't')->fields('t', array('makhoa', 'makhoa'))->execute()->fetchAllKeyed(),
                '#default_value'=> (isset($arrUpdate->khoaql) ? $arrUpdate->khoaql : NULL),
        );
        $form['hedt_update'] = array(
                '#title'=>t('Hệ ĐT:'),
                '#size'=>30,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->hedt) ? $arrUpdate->hedt : 'CQUI,CNTN,KSTN,CLC'),
                '#attributes'=>array('title'=>'Nhập danh sách các hedt sử dụng môn học này'),
        );
        $form['mamhcu_update'] = array(
                '#title'=>t('MaMH cũ:'),
                '#size'=>10,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->mamhcu) ? $arrUpdate->mamhcu : ''),
                //'#required'=>TRUE,
        );
        $form['ghichu_update'] = array(
                '#title'=>t('Ghi chú:'),
                '#size'=>25,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->ghichu) ? $arrUpdate->ghichu : ''),
        );
        break;

     default:
         break;
    }
    
    return $form;
}

function qlmonhoc_capnhat_validate($form, &$form_state) {
    $formid = $form_state['values']['formid'];
    switch ($formid) {
        case 'monhoc':
            if(trim($form_state['values']['tenmh_update']) == '') {
                form_set_error('tenmh_update', 'Tên môn học không hợp lệ');
                return;
            }
            $sotc = (int)$form_state['values']['sotc_update'];
            $sotc_lt = (int)$form_state['values']['sotc_lt_update'];
            $sotc_th = (int)$form_state['values']['sotc_th_update'];
            if($sotc != $sotc_lt + $sotc_th) {
                form_set_error('sotc_update', 'Số TC không hợp lệ');
                return;
            }
            break;

        default:
            break;
    }
}

function qlmonhoc_capnhat($form, &$form_state) {
    $formid = $form_state['values']['formid'];
    $id_update = $form_state['values']['id_update'];
    switch ($formid) {
        case 'monhoc':
            $fields = array(
                'tenmh' => trim($form_state['values']['tenmh_update']),
                'tenta' => trim($form_state['values']['tenta_update']),
                'loaimh' => trim($form_state['values']['loaimh_update']),
                'sotinchi' => (int)$form_state['values']['sotc_update'],
                'sotc_lt' => (int)$form_state['values']['sotc_lt_update'],
                'sotc_th' => (int)$form_state['values']['sotc_th_update'],
                'hedt' => $form_state['values']['hedt_update'],
                'khoaql' => $form_state['values']['khoaql_update'],
                'hinhthucth' => $form_state['values']['hinhthucth_update'],
                'mamhcu' => trim($form_state['values']['mamhcu_update']),
                'ghichu' => trim($form_state['values']['ghichu_update']),
            );
            try {
                db_update('uit_monhoc')->fields($fields)->condition('id', $id_update)->execute();
            } catch (Exception $exc) {
                drupal_set_message('Cập nhật bị lỗi ('.$exc->getMessage().')');
            }
            break;

        default:
            break;
    }
}

function qlmonhoc_xoa($form, &$form_state) {
    $formid = $form_state['values']['formid'];
    $arrID = array_filter($form_state['values']['data_table']);
    if(count($arrID) == 0) return;
    $succ = 0;
    $strLogs = '';
    foreach ($arrID as $rid) {
        switch ($formid) {
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
            case 'monhoctd':
                if(XoaDanhMuc($rid, 'uit_monhoctd')) {
                    $succ++;
                    $strLogs .= $rid.';';
                }
                break;

            default:
                break;
        }
    }
    if($succ > 0) {
        drupal_set_message('Đã xóa thành công '.$succ.' dòng ('.$strLogs.')');
        ghilogs('Xóa danh mục '.$formid.' các mã: '.$strLogs, $formid, 'qldanhmuc');
    }
    return;
}

function qlmonhoc_export($form, &$form_state) {
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
        case 'monhoc':
            $fields = array('mamh'=>'Mã MH','tenmh'=>'Tên MH','tenta'=>'Tên TA','loaimh'=>'Loại MH',
                'sotinchi'=>'Số TC','sotc_lt'=>'Số TCLT','sotc_th'=>'Số TCTH','hedt'=>'Hệ ĐT',
                'khoaql'=>'Khoa QL','hinhthucth'=>'HTTH','mamhcu'=>'Mã cũ','ghichu'=>'Ghi chú');
            $q = db_select('uit_monhoc', 't');
            $q->fields('t');
            qlmonhoc_build_filter_query($q);
            $rows = $q->execute()->fetchAll();
            break;
        case 'monhoctd':
            $fields = array('mamh1'=>'Mã MH cũ','mamh2'=>'Mã MH mới','hedt'=>'Hệ ĐT','ghichu'=>'Ghi chú');
            $q = db_select('uit_monhoctd', 't');
            $q->fields('t');
            qlmonhoc_build_filter_query($q);
            $rows = $q->execute()->fetchAll();
            break;

        default:
            return;
    }
    
    require_once 'sites/all/libraries/PHPExcel.php';
    $filepath = 'sites/default/files/exports';
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

function qlmonhoc_set_filters(&$form_state, $formid) {
    //unset($_SESSION['qlmonhoc_filter']);
    $filters = qlmonhoc_filters($formid);
    foreach ($filters as $filter => $options) {
      if (isset($form_state['values'][$filter])) {
        if( $form_state['values'][$filter] != '') {
          $_SESSION['qlmonhoc_filter'][$filter] = $form_state['values'][$filter];
        }
        else {
          unset($_SESSION['qlmonhoc_filter'][$filter]);
        }
      } 
    }
}

function qlmonhoc_filter_form($formid) {
    $form = array();
    $filters = qlmonhoc_filters($formid);
    foreach ($filters as $key => $filter) {
        $arr = array();
        foreach ($filter as $key2 => $value2) {
            $arr['#'.$key2] = $value2;
        }
        $form['filters'][$key] = $arr;
    }
    return $form;
}

function qlmonhoc_chucnang_form($formid) {
    global $base_url;
    $form = array();
    switch ($formid) {
        case 'monhoc':
        case 'monhoctd':
            $form['export'] = array( 
                '#type' => 'button',
                '#value' => t('Export'),
                '#attributes' => array('title' =>'Xuất dữ liệu ra file excel'),
                '#executes_submit_callback' => TRUE,
                '#submit' => array('qlmonhoc_export'),
            );
            $form['delete'] = array( 
                '#type' => 'button',
                '#value' => t('Xóa'),
                '#attributes' => array('onclick'=>'return confirm(\'Bạn có chắc xóa các dòng được chọn ?\');','title' =>'Xóa các dòng được chọn'),
                '#executes_submit_callback' => TRUE,
                '#submit' => array('qlmonhoc_xoa'),
            );
            $form['import'] = array( 
                '#type' => 'button',
                '#value' => t('Import'),
                '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/?q=admin/qlmonhoc/import/'.$formid.'\');','title' =>'Import dữ liệu từ file excel'),
                '#executes_submit_callback' => FALSE,
            );
            break;
        
        default:
            break;
    }
    return $form;
}


function qlmonhoc_get_data($formid) {
    $headers = array();
    $options = array();
    $fields = array();
    switch ($formid) {
        case 'monhoc':
            $fields = array('mamh'=>'Mã MH','tenmh'=>'Tên MH','tenta'=>'Tên TA','loaimh'=>'Loại MH',
                'sotinchi'=>'Số TC','sotc_lt'=>'Số TCLT','sotc_th'=>'Số TCTH','hedt'=>'Hệ ĐT',
                'khoaql'=>'Khoa QL','hinhthucth'=>'HTTH','mamhcu'=>'Mã cũ','ghichu'=>'Ghi chú');
            foreach ($fields as $key=>$value) {
                $headers[] = array('data' => $value, 'field' => $key);
            }
            $q = db_select('uit_monhoc', 't')->extend('PagerDefault')->limit($_SESSION['qlmonhoc_filter']['sodong'])->extend('TableSort')->orderByHeader($headers);
            $q->fields('t');
            qlmonhoc_build_filter_query($q);
            $rows = $q->execute()->fetchAll();
            break;
        case 'monhoctd':
            $fields = array('id'=>'ID','mamh1'=>'Mã MH cũ','mamh2'=>'Mã MH mới','hedt'=>'Hệ ĐT','ghichu'=>'Ghi chú');
            foreach ($fields as $key=>$value) {
                $headers[] = array('data' => $value, 'field' => $key);
            }
            $q = db_select('uit_monhoctd', 't')->extend('PagerDefault')->limit($_SESSION['qlmonhoc_filter']['sodong'])->extend('TableSort')->orderByHeader($headers);
            $q->fields('t');
            qlmonhoc_build_filter_query($q);
            $rows = $q->execute()->fetchAll();
            break;

        default:
            break;
    }
    $n = 0;
    foreach ($rows as $row) {
        $data = (array)$row;
        foreach ($fields as $key=>$value) {
            $val = $data[$key];
            if(in_array($key, array('mamh', 'id'))) {
                $val = l($val,'admin/qldanhmuc/'.$formid.'/edit/'.$row->id);
            }
            if(isset($row->id)) {
                $options[$row->id][] = $val;
            } else {
                $options[$n][] = $val;
            }
        }
        $n++;
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

function qlmonhoc_form_info($formid) {
    $total = 0;
    $form = array();
    switch ($formid) {
        case 'monhoc':
            $q = db_select('uit_monhoc', 't');
            $q->addExpression('count(id)');
            qlmonhoc_build_filter_query($q);
            $total = $q->execute()->fetchField();
            break;
        case 'monhoctd':
            $q = db_select('uit_monhoctd', 't');
            $q->addExpression('count(id)');
            qlmonhoc_build_filter_query($q);
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

function qlmonhoc_filters($formid) {
    $filters = array();
    if(!isset($_SESSION['qlmonhoc_filter']['sodong'])) $_SESSION['qlmonhoc_filter']['sodong'] = 50;
    $session = isset($_SESSION['qlmonhoc_filter']) ? $_SESSION['qlmonhoc_filter'] : array();
    $sodong = !empty($session['sodong']) ? $session['sodong'] : 50;
    switch ($formid) {
        case 'monhoc':
            $mamh = !empty($session['mamh']) ? $session['mamh'] : NULL;
            $tenmh = !empty($session['tenmh']) ? $session['tenmh'] : NULL;
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
            $filters['hedt'] = array(
                'type' => 'select',
                'title' => t('Hệ ĐT:'),
                'options' => (array(''=>'Tất cả') + db_select('uit_hedt', 'd')->distinct()->orderBy('d.kyhieu')->fields('d', array('kyhieu', 'kyhieu'))->execute()->fetchAllKeyed()),
                'default_value' => array($hedt),
            );
            
            break;
        case 'monhoctd':
            $mamh1 = !empty($session['mamh1']) ? $session['mamh1'] : NULL;
            $mamh2 = !empty($session['mamh2']) ? $session['mamh2'] : NULL;
            $hedt = !empty($session['hedt']) ? $session['hedt'] : NULL;
            
            $filters['mamh1'] = array(
                'type' => 'textfield',
                'title' => t('Mã MH cũ:'),
                'size' => 30,
                'default_value' => array($mamh1),
                'attributes' => array('title' =>'Có thể nhập nhiều mã môn học cách nhau bởi dấu phẩy (,).'),
            );
            $filters['mamh2'] = array(
                'type' => 'textfield',
                'title' => t('Mã MH mới:'),
                'size' => 30,
                'default_value' => array($mamh2),
            );
            $filters['hedt'] = array(
                'type' => 'select',
                'title' => t('Hệ ĐT:'),
                'options' => (array(''=>'Tất cả') + db_select('uit_hedt', 'd')->distinct()->orderBy('d.kyhieu')->fields('d', array('kyhieu', 'kyhieu'))->execute()->fetchAllKeyed()),
                'default_value' => array($hedt),
            );
            
            break;

        default:
            break;
    }
    $filters['sodong'] = array(
          'type' => 'select',
          'title' => t('Số dòng'),
          'options' => (array('50' => t('50'),'100'=>t('100'),'200'=>t('200'),'500'=>t('500'))),
          'default_value' => array($sodong),
    );
    if(count($filters) > 0) {
        $filters['timkiem'] = array(
            'type' => 'button',
            'value' => t('Tìm kiếm'),
            'ajax' => array(
              'event' => 'click',
              'callback' => 'ajax_filter_qlmonhoc_callback',
              'wrapper' => 'table_data',
            ),
        );
    }
    return $filters;
}

function qlmonhoc_build_filter_query(SelectQueryInterface $query) {
    $filter_data = isset($_SESSION['qlmonhoc_filter']) ? $_SESSION['qlmonhoc_filter'] : array();
    foreach ($filter_data as $key => $value) {
        switch ($key) {
            case 'timkiem':
            case 'sodong':
                break;
            case 'hedt':
                $query->condition($key, '%'.$value.'%', 'LIKE');
                break;
            case 'tenmh':
                $query->where('(tenmh like \'%'.$value.'%\' or tenta like \'%'.$value.'%\' )');
                break;
            
            default:
                $query->condition($key, $value);
        }
    }
}

function ajax_filter_qlmonhoc_callback($form, $form_state) {
  return $form['data'];
}
