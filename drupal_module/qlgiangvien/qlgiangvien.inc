<?php
function qlgiangvien_form($form, &$form_state) {
    global $base_url;
    if(!user_access('UIT qlgiangvien') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $form = array();
    $form['#prefix'] = '<div id="form-qlgiangvien-wrapper">';
    $form['#suffix'] = '</div>';
    $form['information'] = array(
      '#markup' =>
      '<center><h1><font color="blue"><strong>QUẢN LÝ GIẢNG VIÊN</strong></font></h1></center><br>');
    
    $form['table_filter_wraper'] = array('#type' => 'fieldset', '#title' => t('Chọn điều kiện lọc'), 
        '#attributes' => array('class' => array('container-inline')), '#collapsible' => TRUE, '#collapsed' => FALSE);
    qlsv_set_filters($form_state);
    $form['table_filter_wraper'][] = qlgv_filter_form();
    
    $form['qlsv_data'] = array(
        '#prefix' => '<div id="table_dsgv">',
        '#suffix' => '</div>',);
    if ($_GET['q'] == 'system/ajax') {
        $q = 'admin/qlgiangvien' ;
        $_GET['q'] = $q;
    }
    $form['qlsv_data']['info'] = qlsv_info_form();
    $form['qlsv_data']['dsgiangvien_table'] = _qlgiangvien_get_table_dsgiangvien();
    $form['qlsv_data']['pager'] = array('#markup' => theme('pager'));
    
    $form['xuatexcel'] = array(
        '#type' => 'button',
        '#value' => t('Xuất excel'),
        '#attributes' => array('title' =>'Xuất excel'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlgiangvien_xuatexcel'),
    );
    if(user_access('UIT qlgiangvien')) {
        $form['xoa'] = array(
            '#type' => 'button',
            '#value' => t('Xóa'),
            '#attributes' => array('title' =>'Xóa các dòng được chọn'),
            '#executes_submit_callback' => TRUE,
            '#submit' => array('qlgiangvien_xoa'),
        );
        $form['importsv'] = array( 
            '#type' => 'button',
            '#value' => t('Import GV'),
            '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/admin/qlgiangvien/import/giangvien\');', 'title' =>'Import GV từ file excel'),
            '#executes_submit_callback' => FALSE,
        );
        $form['synportal'] = array( 
            '#type' => 'button',
            '#value' => t('Đồng bộ với portal'),
            '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/admin/qlgiangvien/syncportal\');', 'title' =>'Đồng bộ thông tin CB-GV với portal'),
            '#executes_submit_callback' => FALSE,
        );
    }
    return $form;
}

function qlgiangvien_themsv() {
    drupal_goto('admin/qlgiangvien/them');
}

function _qlgiangvien_get_table_dsgiangvien() {
    $header = array(
        //array('data' => t('STT'), 'field' => ''),
        array('data' => t('Mã GV'), 'field' => 'magv'),
        array('data' => t('Username'), 'field' => 'username'),
        array('data' => t('Họ tên'), 'field' => 'hoten'),
        array('data' => t('Ngày sinh'), 'field' => 'ngaysinh'),
        array('data' => t('Giới tính'), 'field' => 'gioitinh'),
        array('data' => t('Học vị'), 'field' => 'tenhv'),
        array('data' => t('Học hàm'), 'field' => 'tenhh'),
        array('data' => t('Đơn vị'), 'field' => 'tendv'),
        array('data' => t('Thao tác'), 'field' => '')
    );
    $query = db_select('uit_thongtingv', 'gv')->extend('PagerDefault')->limit($_SESSION['qlgv_filter']['sodong'])->extend('TableSort')->orderByHeader($header);
    $query->leftJoin('uit_danhmuc', 'dv', "dv.loaidm = 'donvi' AND gv.donvi = dv.madm");
    $query->leftJoin('uit_danhmuc', 'hv', "hv.loaidm = 'hocvi' AND gv.hocvi = hv.madm");
    $query->leftJoin('uit_danhmuc', 'hh', "hh.loaidm = 'hocham' AND gv.hocham = hh.madm");
    $query->fields('gv', array('magv', 'hoten', 'username', 'ngaysinh', 'gioitinh'));
    $query->addField('dv','tenvt','tendv');
    $query->addField('hv','tenvt','tenhv');
    $query->addField('hh','tenvt','tenhh');
    //$query->fields('dv', array('tendm'));
    $query->orderBy('gv.magv', 'ASC');
    qlsv_build_filter_query( $query);
    $rows = $query->execute();
    $options = array();
    //$i = 1;
    foreach ($rows as $row) {
        $options[$row->magv] = array(
            //$i++,
            l($row->magv,'admin/qlgiangvien/xem/'.$row->magv,array('attributes' => array('target' => '_blank'))),
            //$row->magv,
            $row->username,
            $row->hoten,
            (strlen($row->ngaysinh) == 10 ? date('d-m-Y',  strtotime($row->ngaysinh)) :$row->ngaysinh),
            ($row->gioitinh == 1 ? 'Nam':'Nữ'),
            $row->tenhv,
            $row->tenhh,
            $row->tendv,
            l('Sửa','admin/qlgiangvien/capnhat/'.$row->magv,array('attributes' => array('target' => '_blank'))),
        );
    }
    
    $form = array(
        '#title' => 'Table',
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        //'#default_value' => drupal_map_assoc($default),
        '#empty' => t('Không tìm thấy giảng viên nào.'),
        //'#sticky' => FALSE,
        '#prefix' => '<div class="table_dssinhvien"  id="table_dssinhvien">',
        '#suffix' => '</div>'
    );
    return $form;
}

function qlgiangvien_xoa($form, &$form_state) {
    $arrMaGV = array_filter($form_state['values']['dsgiangvien_table']);
    $strmagv = NULL;
    foreach ($arrMaGV as $magv) {
        if(qlgiangvien_checkxoa($magv)) {
            try {
                db_delete('uit_thongtingv')->condition('magv',$magv)->execute();
                drupal_set_message('Đã xóa thành công CB-GV: '.$magv);
                if(!$strmagv) $strmagv = $magv; else $strmagv .= '; '.$magv;
            } catch (Exception $exc){
                drupal_set_message('Xóa không thành công CB-GV: '.$magv.' ('.$exc->getMessage().')', 'error');
            }
        } else {
            drupal_set_message('Không thể xóa CB-GV '.$magv.' vì còn dữ liệu liên quan', 'error');
        }            
    }
    if($strmagv) ghilogs('Xóa các SV: '.$strmagv, '', 'qlgiangvien');
}

function qlgiangvien_checkxoa($magv){
    $check = db_select('qltc_thunhapcanhan', 'd')->fields('d', array('magv'))->condition('magv', $magv)->execute()->fetchField();
    if($check) return false;
    $check = db_select('qltc_bangluong', 'd')->fields('d', array('magv'))->condition('magv', $magv)->execute()->fetchField();
    if($check) return false;
    
    return true;
}


function qlgiangvien_xuatexcel($form, &$form_state) {
    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    $filename = 'dscbgv_'.date('Ymd_His').'.xlsx';
    $path = variable_get('file_public_path', conf_path() . '/files') . '/exports';
    if(!file_exists($path)) mkdir($path);
    $path .= '/'.$filename;
    
    $objPHPExcel = new PHPExcel();
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    
    $headers = array('magv'=>'Mã GV','username'=>'Username','hoten'=>'Họ tên','ngaysinh'=>'Ngày sinh','gioitinh'=>'Giới tính',
        'tenhv'=>'Học vị','tenhh'=>'Học hàm','tendv'=>'Đơn vị');
    $query = db_select('uit_thongtingv', 'gv');
    $query->leftJoin('uit_danhmuc', 'dv', "dv.loaidm = 'donvi' AND gv.donvi = dv.madm");
    $query->leftJoin('uit_danhmuc', 'hv', "hv.loaidm = 'hocvi' AND gv.hocvi = hv.madm");
    $query->leftJoin('uit_danhmuc', 'hh', "hh.loaidm = 'hocham' AND gv.hocham = hh.madm");
    $query->fields('gv', array('magv', 'hoten', 'username', 'ngaysinh', 'gioitinh'));
    $query->addField('dv','tenvt','tendv');
    $query->addField('hv','tenvt','tenhv');
    $query->addField('hh','tenvt','tenhh');
    //$query->fields('dv', array('tendm'));
    $query->orderBy('gv.magv', 'ASC');
    qlsv_build_filter_query( $query);
    $rows = $query->execute()->fetchAll();
    
    if(!$rows) {
        drupal_set_message('Không tìm thấy thông tin để xuất', 'error');
        return;
    }
    //Headers
    $r = 1;
    $c = $col_start = 'A';
    $ActiveSheet->setCellValue($c++.$r, 'STT');
    foreach ($headers as $key=>$value) {
        $ActiveSheet->setCellValue($c++.$r, $value);
    }
    
    //Rows
    $r++;
    $stt = 1;
    foreach ($rows as $row) {
        $c = $col_start;
        $data = (array)$row;
        $ActiveSheet->setCellValue($c++.$r, $stt++);
        foreach ($headers as $key=>$value) {
            switch ($key) {
                case 'ngaysinh':
                    $ActiveSheet->setCellValue($c++.$r, strlen($data[$key]) == 10 ? date('d-m-Y',  strtotime($data[$key])) : $data[$key]);
                    break;
                case 'gioitinh':
                    $ActiveSheet->setCellValue($c++.$r, $data[$key] == 1 ? 'Nam' : 'Nữ');
                    break;

                default:
                    $ActiveSheet->setCellValue($c++.$r, $data[$key]);
                    break;
            }
        }
        $r++;        
    }
    
    $objWriter = new PHPExcel_Writer_Excel2007($objPHPExcel);
    ob_end_clean();
    $objWriter->save($path);
    if(filesize($path)){
        drupal_add_http_header('Pragma', 'public');
        drupal_add_http_header('Expires', '0');
        drupal_add_http_header('Cache-Control', 'must-revalidate, post-check=0, pre-check=0');
        drupal_add_http_header('Content-Type', 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
        drupal_add_http_header('Content-Disposition', 'attachment; filename=' . basename($path) . ';');
        drupal_add_http_header('Content-Transfer-Encoding', 'binary');
        drupal_add_http_header('Content-Length', filesize($path));
        readfile($path);
        unlink($path);
        drupal_exit();
    } else {
        drupal_set_message(t("Xuất file thất bại, Vui lòng kiểm tra lại dữ liệu!"), 'error');
    }
}

function qlsv_build_filter_query(SelectQueryInterface $query) {
    $filter_data = isset($_SESSION['qlgv_filter']) ? $_SESSION['qlgv_filter'] : array();
    foreach ($filter_data as $key => $value) {
        switch ($key) {
            case 'timkiem':
            case 'sodong':
                break;
            case 'magv':
                $arrmagv = explode(',', $value);
                $query->condition('gv.'.$key, $arrmagv, 'IN');
                break;
            case 'hoten':
                $query->where('(gv.hoten like \'%'.$value.'\' OR gv.hoten like \''.$value.'%\')');
                break;
            case 'phanloai':
                if($value == 1) $query->where("(gv.magv like '8%' OR gv.magv like '9%')");
                if($value == 2) $query->where("(gv.magv like '7%' OR gv.magv like '1%')");
                break;
            
            default:
                $query->condition('gv.'.$key, $value);
        }
    }
}

function qlsv_set_filters(&$form_state) {
  $filters = qlgv_filters();
  //Apply every filter that has a choice selected other than 'any'.
  foreach ($filters as $filter => $options) {
    if (isset($form_state['values'][$filter])) {
      if( $form_state['values'][$filter] != '') {
        // Flatten the options array to accommodate hierarchical/nested options.
        //$flat_options = form_options_flatten($filters[$filter]['options']);
        $_SESSION['qlgv_filter'][$filter] = $form_state['values'][$filter];
        // Only accept valid selections offered on the dropdown, block bad input.
        //if (isset($flat_options[$form_state['values'][$filter]])) {
        //  $_SESSION['qlgv_filter'][$filter] = $form_state['values'][$filter];
        //}
      }
      else {
        unset($_SESSION['qlgv_filter'][$filter]);
      }
    } 
  }
}

function qlgv_filters() {
    $filters = array();  
    if(!isset($_SESSION['qlgv_filter']['sodong'])) $_SESSION['qlgv_filter']['sodong'] = 50;
    if(!isset($_SESSION['qlgv_filter']['phanloai'])) $_SESSION['qlgv_filter']['phanloai'] = 1;
    $session = isset($_SESSION['qlgv_filter']) ? $_SESSION['qlgv_filter'] : array();
    $magv = !empty($session['magv']) ? $session['magv'] : NULL;
    $hoten = !empty($session['hoten']) ? $session['hoten'] : NULL;
    $phanloai = !empty($session['phanloai']) ? $session['phanloai'] : 1;
    $donvi = !empty($session['donvi']) ? $session['donvi'] : NULL;
    $tinhtrang = !empty($session['tinhtrang']) ? $session['tinhtrang'] : NULL;
    $sodong = !empty($session['sodong']) ? $session['sodong'] : 50;

    $filters['magv'] = array(
        'type' => 'textfield',
        'title' => t('Mã GV:'),
        'size' => 50,
        'default_value' => array($magv),
        'attributes' => array('title' =>t('Mỗi Mã GV cách nhau bởi dấu phẩy (,)')),
    );
    $filters['hoten'] = array(
        'type' => 'textfield',
        'title' => t('Họ tên:'),
        'size' => 25,
        'default_value' => array($hoten),
        'attributes' => array('title' =>t('Nhập họ hoặc tên hoặc cả 2')),
    );
    $filters['phanloai'] = array(
        'type' => 'select',
        'title' => t('Phân loại'),
        'options' => array('0' => 'Tất cả','1' => t('Trong trường'),'2'=>t('Ngoài trường')),
        'default_value' => array($phanloai),
    );
    $filters['tinhtrang'] = array(
        'type' => 'select',
        'title' => t('Tình trạng'),
        'options' => (array(''=>'Tất cả') + db_select('uit_danhmuc', 't')->condition('loaidm', 'tinhtranggv')->orderBy('thutu')->fields('t', array('madm', 'tendm'))->execute()->fetchAllKeyed()),
        'default_value' => array($tinhtrang),
    );
    $filters['donvi'] = array(
        'type' => 'select',
        'title' => t('<br>Đơn vị'),
        'options' => (array(''=>'Tất cả') + db_select('uit_danhmuc', 't')->condition('loaidm', 'donvi')->orderBy('thutu')->fields('t', array('madm', 'tendm'))->execute()->fetchAllKeyed()),
        'default_value' => array($donvi),
    );
    $filters['sodong'] = array(
        'type' => 'select',
        'title' => t('Số dòng'),
        'options' => (array('50' => t('50'),'100'=>t('100'),'200'=>t('200'),'500'=>t('500'))),
        'default_value' => array($sodong),
    );
  
    $filters['timkiem'] = array(
        'type' => 'button',
        'value' => t('Tìm kiếm'),
        'ajax' => array(
            'event' => 'click',
            'callback' => 'ajax_filter_qlgiangvien_callback',
            'wrapper' => 'table_dsgv',
        ),
    );
  
    return $filters;
}

function qlgv_filter_form() {
    $form = array();
    $filters = qlgv_filters();
    foreach ($filters as $key => $filter) {
        $arr = array();
        foreach ($filter as $key2 => $value2) {
            $arr['#'.$key2] = $value2;
        }
        $form['filters'][$key] = $arr;
    }
    return $form;
}

function qlsv_info_form() {
    $query = db_select('uit_thongtingv','gv');
    $query->addExpression('count(gv.magv)', 'tongsv');
    qlsv_build_filter_query( $query);
    $tongsv = $query->execute()->fetchField();
    $form['info'] = array(array('#markup' =>'<p class="info_count">Tổng số giảng viên: '.number_format($tongsv).'</p>'));
    return $form;
}

function ajax_filter_qlgiangvien_callback($form, $form_state) {
  return $form['qlsv_data'];
}

?>