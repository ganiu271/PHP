<?php
function qldiem_form($form, &$form_state) {
    if(!user_access('Quan ly diem') && !user_access('Import diem') && !user_access('Xem diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>PHÂN HỆ QUẢN LÝ ĐIỂM SINH VIÊN</h1></center>';
    $html .= '<p>Vui lòng chọn chức năng cụ thể từ menu Điểm</p>';
    $form['markup'] = array('#markup' => $html);
    
    return $form;
}

function qldiem_form_ql($form, &$form_state) {
    global $base_url;
    if(!user_access('Quan ly diem') && !user_access('Import diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $form = array();
    $html = '<center><h1>QUẢN LÝ ĐIỂM HỌC VIÊN</h1></center>';
//    $html .= '<p>Chọn chức năng:</p>';
    $form['markup'] = array('#markup' => $html);
    $form['qldiem_filter_wraper'] = array('#type' => 'fieldset', '#title' => t('Chọn điều kiện lọc'), '#attributes' => array('class' => array('container-inline')));
    qldiem_set_filters($form_state);
    $form['qldiem_filter_wraper'][] = qldiem_filter_form();
    
    $headers = array(
        array('data' => 'STT', 'field' => ''),
        array('data' => 'Mã môn học', 'field' => 'mamh'),
        array('data' => 'Mã lớp', 'field' => 'malop'),
        array('data' => 'CTĐT', 'field' => 'nganh'),
//        array('data' => 'Học phần', 'field' => 'hocky'),
        array('data' => 'Số SV', 'field' => ''),
        array('data' => 'Chức năng', 'field' => ''),
    );
//    $query = db_select('uit_diem','d')->distinct()->extend('PagerDefault')->limit($_SESSION['qldiem_filter']['sodong']);
//    $query->join('uit_thongtinhv', 'sv', 'sv.mahv = d.mahv'); không cần
//    $query->join('uit_ctdaotao', 'm', 'm.mamh = d.mamh and m.nganh = d.nganh');
//    $query->fields('sv',array('coso','khoahoc'));   uit_ctdaotao có khoadt thay cho khoahoc
    $sodong = isset($_SESSION['qlhv_filter']['sodong']) ? $_SESSION['qldiem_filter']['sodong'] : 50;
    $query = db_select('uit_diem','d')->extend('PagerDefault');
    $query->join('uit_monhoc', 'm', 'm.mamh = d.mamh');
    $query->join('uit_thongtinhv', 'sv', 'sv.mahv = d.mahv');
    $query->fields('d')->fields('m')->fields('sv');
//    if(isset($_SESSION['qldiem_filter']['mahv']) && $_SESSION['qldiem_filter']['mahv'] != '') {
//        $query->fields('d',array('mahv','mamh','malop'));
//    } else {
//        $query->fields('d',array('mamh','malop'));
//    }
//    $query->fields('m', array('tenmh','nganh','hocky'));   không có field nganh trong uit_ctdaotao
//    $query->fields('m', array('tenmh','hocky'));  không có tenmh trong uit_ctdaotao.
//    $query->orderBy('d.id','ASC');
    $query->groupBy('d.malop');
    qldiem_build_filter_query($query);
//    $data = $query->execute();
    $data = $query->limit($sodong)->execute();
    $rows = array();
    $n=1;
    foreach ($data as $lop) {
        $strpara = $lop->malop;
        $q = db_select('uit_diem','t');
        $q->addExpression('count(mahv)','tongsv');
        $q->condition('malop',$lop->malop);
        $sosv = $q->execute()->fetchField();
        if(isset($_SESSION['qldiem_filter']['mahv']) && $_SESSION['qldiem_filter']['mahv'] != '') {
            $strfunc = l('Xem điểm',$base_url.'/?q=admin/qldiem/xemdiem/'.$strpara.';'.$lop->mahv,
                    array('attributes'=>array('target'=>'_blank')));
        } else {
            $strfunc = l('Xem điểm',$base_url.'/?q=admin/qldiem/xemdiem/'.$strpara,
                    array('attributes'=>array('target'=>'_blank')));
        }
        $rows[$strpara] = array(
                            $n++,
                            $lop->mamh,
                            $lop->malop,
                            $lop->mactdt,
//                            $lop->hocky,
                            $sosv,
                            $strfunc,
                            '#attributes'=>array('title'=>$lop->malop)
                            );
    }
    $form['xoadiem0'] = array( '#type' => 'button',
                             '#value' => t('Xóa điểm'),
                             '#attributes' => array('onclick'=>'return confirm(\'Bạn có chắc xóa điểm các lớp được chọn ?\');','title' =>'Xóa điểm các lớp được chọn'),
                             '#executes_submit_callback' => TRUE,
                             '#submit' => array('qldiem_xoadiem'),
                            );
    $form['import0'] = array( '#type' => 'button',
                             '#value' => t('Import điểm'),
                             '#attributes' => array('title' =>'Import điểm từ file excel'),
                             '#executes_submit_callback' => TRUE,
                             '#submit' => array('import_request'),
                            );
    $form['qldiem_wraper'] = array(
        '#prefix' => '<div id="table_qldiem">',
        '#suffix' => '</div>',);
    $form['qldiem_wraper']['table_lop'] = array(
        '#type' => 'tableselect',
        '#caption' => 'DANH SÁCH BẢNG ĐIỂM',
        '#header' => $headers,
        '#options' => $rows,
        '#empty' => 'Không có bảng điểm nào'
    );
    $tmp_q = $_GET['q'];
    $_GET['q'] = 'admin/qldiem/quanly';
    $form['qldiem_wraper']['pager'] = array('#markup' => theme('pager'));
    $_GET['q'] = $tmp_q;

    $form['xoadiem'] = array( '#type' => 'button',
                             '#value' => t('Xóa điểm'),
                             '#attributes' => array('onclick'=>'return confirm(\'Bạn có chắc xóa điểm các lớp được chọn ?\');','title' =>'Xóa điểm các lớp được chọn'),
                             '#executes_submit_callback' => TRUE,
                             '#submit' => array('qldiem_xoadiem'),
                            );
    $form['import'] = array( '#type' => 'button',
                             '#value' => t('Import điểm'),
                             '#attributes' => array('title' =>'Import điểm từ file excel'),
                             '#executes_submit_callback' => TRUE,
                             '#submit' => array('import_request'),
                            );

    return $form;
}

function qldiem_set_filters(&$form_state) {
  $filters = qldiem_filters();
  foreach ($filters as $filter => $options) {
    if (isset($form_state['values'][$filter])) {
      if( $form_state['values'][$filter] != '') {
        $_SESSION['qldiem_filter'][$filter] = $form_state['values'][$filter];
      }
      else {
        unset($_SESSION['qldiem_filter'][$filter]);
      }
    } 
  }
}

function qldiem_filters() {
  $filters = array();
  //unset($_SESSION['qldiem_filter']);
  if(!isset($_SESSION['qldiem_filter'])) {
      $_SESSION['qldiem_filter']['sodong'] = 50;
//      $_SESSION['qldiem_filter']['coso'] = '';
//      $_SESSION['qldiem_filter']['nganh'] = '';
  }
  $session = isset($_SESSION['qldiem_filter']) ? $_SESSION['qldiem_filter'] : array();
  $hocky = !empty($session['hocky']) ? $session['hocky'] : NULL;
  $mahv = !empty($session['mahv']) ? $session['mahv'] : NULL;
  $khoahoc = !empty($session['khoahoc']) ? $session['khoahoc'] : NULL;
  $mamh = !empty($session['mamh']) ? $session['mamh'] : NULL;
  $sodong = !empty($session['sodong']) ? $session['sodong'] : 50;
  $ctdt=!empty($session['mactdt']) ? $session['mactdt'] : NULL;
  $arrHocPhan = array(''=>'Tất cả');
  for($i=1; $i<16; $i++) $arrHocPhan[$i] = $i;
  
  $filters['mahv'] = array(
      'type' => 'textfield',
      'title' => t('Mã HV:'),
      'size' => 7,
      'default_value' => array($mahv),
  );
  $filters['mamh'] = array(
      'type' => 'textfield',
      'title' => t('Mã MH / Lớp:'),
      'size' => 10,
      'default_value' => array($mamh),
  );
  $filters['mactdt'] = array(
                    'type' => 'select',
                    'title' => t('CTĐT'),
                    'options' => (array('' => t('Chọn CTĐT')) + db_select('uit_quanly_ctdt', 'd')->distinct()->orderBy('d.ma_ctdaotao')->fields('d', array('ma_ctdaotao', 'ma_ctdaotao'))->execute()->fetchAllKeyed()),
                    'default_value' => array($ctdt),
                  );
  $filters['khoahoc'] = array(
                    'type' => 'select',
                    'title' => t('Khóa'),
                    'options' => (array('' => t('Chọn khóa')) + db_select('uit_thongtinhv', 'd')->distinct()->orderBy('d.khoahoc')->fields('d', array('khoahoc', 'khoahoc'))->execute()->fetchAllKeyed()),
                    'default_value' => '',
                  );
//  $filters['hocky'] = array(
//                    'type' => 'select',
//                    'title' => t('Học phần'),
//                    'options' => $arrHocPhan,
//                    'default_value' => array($hocky),
//                  );
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
                      'callback' => 'ajax_filter_qldiem_callback',
                      'wrapper' => 'table_qldiem',
                    ),
                );
  return $filters;
}

function qldiem_filter_form() {
    $form = array();
    $filters = qldiem_filters();
    foreach ($filters as $key => $filter) {
        $arr = array();
        foreach ($filter as $key2 => $value2) {
            $arr['#'.$key2] = $value2;
        }
        $form['filters'][$key] = $arr;
    }
    return $form;
}

function qldiem_build_filter_query(SelectQueryInterface $query) {
    $filter_data = isset($_SESSION['qldiem_filter']) ? $_SESSION['qldiem_filter'] : array();
    foreach ($filter_data as $key => $value) {
        switch ($key) {
            case 'timkiem':break;
            case 'sodong':break;
            case 'mactdt':
                $query->condition('d.malop', $value.'%','LIKE');
                break;
            case 'mamh':
                $query->condition('d.malop', '%'.$value.'%','LIKE');
                break;
            case 'khoahoc':
                $query->condition('sv.'.$key, $value);
                break;
            case 'nganh':break;
            case 'hocky':
                $query->condition('m.'.$key, $value);
                break;
            default:
                $query->condition('d.'.$key, $value);
        }
    }
}

function import_request() {
    drupal_goto('admin/qldiem/import');
}

function qldiem_xoadiem($form, &$form_state) {
    if(!user_access('Quan ly diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $arrMalop = array_filter($form_state['values']['table_lop']);
    $strlop = '';
    foreach ($arrMalop as $malop) {
        if(XoaDiem($malop)) {
            $strlop .= $malop.' | ';
        }
    }
    if($strlop != '') {
        drupal_set_message ('Đã xóa thành công các lớp: '.$strlop);
        ghilogs('Xóa điểm các lớp: '.$strlop, '');
    }
    return;
}

function XoaDiem($malop) {
    try {
        $query = db_select('uit_diem','d')->fields('d',array('mahv','mamh'));
        $query->condition('d.malop', $malop);
        $a_sv = $query->execute()->fetchAllKeyed();
        db_delete('uit_diem')->condition('malop',$malop)->execute();
        foreach ($a_sv as $mahv=>$mamh) {
            CapNhatTrangThai($mahv, $mamh);
        }
    } catch (Exception $exc){
        drupal_set_message('Có lỗi xảy ra ('.$exc->getMessage().')', 'error');
        return FALSE;
    }
    return TRUE;
}

function ajax_filter_qldiem_callback($form, $form_state) {
    return $form['qldiem_wraper'];
}