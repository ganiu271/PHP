<?php
function qlctdt_form($form, &$form_state, $cmd=NULl, $rid=NULL) {
    global $base_url;
    if(!user_access('Quản lý CTĐT') && !user_access('Xem CTĐT') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $form = array();
    $html = '<center><h1>QUẢN LÝ CHƯƠNG TRÌNH ĐÀO TẠO</h1></center>';
    $help = '<p><font color="blue">SoTC: tổng số TC; LT: số TC lý thuyết; TH: số TC thực hành; HTTH: hình thức thực hành; GĐ: giai đoạn; HK: học kỳ</font></p>';
    $help .= '<p><font color="blue">Học kỳ 20 được qui ước là các môn học tự chọn.</font></p>';
    $help .= '<p><font color="blue">Nhấn vào mã môn học để cập nhật.</font></p>';
    $form['markup'] = array('#markup' => $html);
    //$form['qlctdt_help'] = array('#type' => 'fieldset', '#title' => t('Hướng dẫn / Giải thích'), '#attributes' => array('class' => array('container-inline')), '#collapsible' => TRUE, '#collapsed' => TRUE);
    //$form['qlctdt_help']['help'] = array('#markup' => $help);
    if(user_access('Quản lý CTĐT')) {
        $titleupdate = 'Thêm mới môn học'; 
        $colapupdate = TRUE;
        $strcmd = 'Thêm mới';
        $arrUpdate = array();
        if($cmd == 'edit' && $rid != '') {
            $titleupdate = 'Cập nhật môn học';
            $colapupdate = FALSE;
            $strcmd = 'Cập nhật';
            $arrUpdate = db_select('uit_ctdaotao', 't')->fields('t')->condition('id', $rid)->execute()->fetch();
            if($arrUpdate->khoadt!=""){
                $arrUpdate->khoadt=explode(',',$arrUpdate->khoadt);
            }
        }
        $form['qlctdt_update'] = array('#type' => 'fieldset', '#title' => t($titleupdate), '#attributes' => array('class' => array('container-inline')), '#collapsible' => TRUE, '#collapsed' => $colapupdate);
        $form['qlctdt_update']['id_update'] = array('#type'=>'hidden',  '#default_value'=> (isset($rid) ? $rid : ''),  );
        $form['qlctdt_update']['mamh_update'] = array(
                '#title'=>t('Mã MH:'),
                '#size'=>10,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->mamh) ? $arrUpdate->mamh : ''),
//                '#required'=>TRUE,
        );
        $form['qlctdt_update']['sotc_update'] = array(
                '#title'=>t('Số TC:'),
                '#size'=>4,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->sotc) ? $arrUpdate->sotc : 0),
//                '#required'=>TRUE,
        );
        $form['qlctdt_update']['sotc_lt_update'] = array(
                '#title'=>t('Tiết LT:'),
                '#size'=>4,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->sotc_lt) ? $arrUpdate->sotc_lt : 0),
                //'#required'=>TRUE,
        );
        $form['qlctdt_update']['sotc_th_update'] = array(
                '#title'=>t('Tiết TH:'),
                '#size'=>4,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->sotc_th) ? $arrUpdate->sotc_th : 0),
                //'#required'=>TRUE,
        );
        $arrHocky = array();
        for($i=1;$i<12;$i++) $arrHocky[$i] = $i;
        $form['qlctdt_update']['hocky_update'] = array(
                '#title'=>t('Học phần:'),
                '#type'=>'select',
                '#options'=>$arrHocky,
                '#default_value'=> (isset($arrUpdate->hocky) ? $arrUpdate->hocky : 1),
        );
        $form['qlctdt_update']['khoadt_update'] = array(
                '#title'=>t('Khóa:'),
                '#type'=>'select',
                '#multiple' => TRUE,
//                '#required'=>TRUE,
                '#options'=>db_select('uit_thongtinhv', 'd')->distinct()->orderBy('d.khoahoc')->fields('d', array('khoahoc', 'khoahoc'))->execute()->fetchAllKeyed(),
                '#default_value'=>(isset($arrUpdate->khoadt) ? $arrUpdate->khoadt : ''),
                '#attributes' => array('title' =>t('Nhấn giữ Ctrl để chọn nhiều khóa')),
        );
        $form['qlctdt_update']['hedt_update'] = array(
            '#title'=>t('Hệ ĐT :'),
            '#type'=>'select',
            '#options'=>db_select('uit_hedt', 'd')->distinct()->orderBy('d.kyhieu')->fields('d', array('kyhieu', 'kyhieu'))->execute()->fetchAllKeyed(),
            '#default_value'=> (isset($arrUpdate->hedt) ? $arrUpdate->hedt : 'CQUI'),
        );
        $form['qlctdt_update']['mactdt_update'] = array(
            '#title'=>t('Thuộc CTĐT :'),
            '#type'=>'select',
            '#options'=>db_select('uit_quanly_ctdt', 'd')->distinct()->orderBy('d.ma_ctdaotao')->fields('d', array('ma_ctdaotao', 'ma_ctdaotao'))->execute()->fetchAllKeyed(),
//            '#options'=>db_select('uit_quanly_ctdt', 'd')->distinct()->orderBy('d.ma_ctdaotao')->fields('d', array('ten_ctdaotao', 'ma_ctdaotao'))->execute()->fetchAllKeyed(),
            '#default_value'=> (isset($arrUpdate->mactdt) ? $arrUpdate->mactdt : ''),
        );
        $form['qlctdt_update']['nganh_update'] = array(
                '#title'=>t('Ngành :'),
                '#type'=>'select',
                '#options'=>db_select('uit_nganh', 'd')->distinct()->orderBy('d.nganh')->fields('d', array('nganh', 'tennganh'))->execute()->fetchAllKeyed(),
                '#default_value'=> (isset($arrUpdate->nganh) ? $arrUpdate->nganh : ''),
        );
        $form['qlctdt_update']['loaihinh_update'] = array(
            '#title'=>t('Loại môn học:'),
            '#type'=>'select',
            '#options'=>db_select('uit_monhoc', 'd')->distinct()->orderBy('d.loaimh')->fields('d', array('loaimh', 'loaimh'))->execute()->fetchAllKeyed(),
            '#default_value'=> (isset($arrUpdate->loaihinh) ? $arrUpdate->loaihinh : ''),
        );
        $form['qlctdt_update']['ghichu_update'] = array(
                '#title'=>t('Ghi chú:'),
                '#size'=>50,
                '#type'=>'textfield',
                '#default_value'=> (isset($arrUpdate->ghichu) ? $arrUpdate->ghichu : ''),
        );
        $form['qlctdt_update']['capnhat_update'] = array(
                '#value'=>t($strcmd),
                '#type'=>'button',
                '#executes_submit_callback' => TRUE,
                '#submit' => array('qlctdt_capnhat'),
        );
    }
    $form['qlctdt_filter_wraper'] = array(
        '#type' => 'fieldset',
        '#title' => t('Chọn điều kiện lọc'),
        '#attributes' => array('class' => array('container-inline')), '#collapsible' => TRUE, '#collapsed' => !$colapupdate);
    qlctdt_set_filters($form_state);
    $form['qlctdt_filter_wraper'][] = qlctdt_filter_form();
    
    $headers = array(
        array('data' => 'Mã MH', 'field' => 'mamh'),
        array('data' => 'Tên môn học', 'field' => 'tenmh'),
        array('data' => 'SoTC', 'field' => 'sotc'),
        array('data' => 'Tiết/TC LT', 'field' => 'sotc_lt'),
        array('data' => 'Tiết/TC TH', 'field' => 'sotc_th'),
        array('data' => 'Học phần', 'field' => 'hocky'),
//        array('data' => 'Năm ', 'field' => 'khoadt'),
        array('data' => 'Khóa ', 'field' => 'khoadt'),
        array('data' => 'Hệ ĐT ', 'field' => 'hedt'),
        array('data' => 'Ngành ', 'field' => 'nganh'),
        array('data' => 'Loại hình ', 'field' => 'loaihinh'),
        array('data' => 'Ghi chú', 'field' => 'ghichu'),
        array('data' => 'Mã CTĐT', 'field' => 'mactdt'),
    );
    $query = db_select('uit_ctdaotao','c')->extend('PagerDefault') ->limit($_SESSION['qlctdt_filter']['sodong'])->extend('TableSort');
    $query->leftJoin('uit_monhoc', 'm', 'm.mamh = c.mamh');
    $query->fields('c');
    $query->fields('m', array('tenmh'));
    qlctdt_build_filter_query( $query);
//    $query->orderBy('c.hedt','ASC');
    $query->orderBy('c.khoadt','ASC');
//    $query->orderBy('c.hocky','ASC');
//    $query->orderBy('c.mamh','ASC');
//    $query->orderBy('c.mactdt','ASC');
    $data = $query->execute();
    
    //$arrTrangThai = array(0=>'Hủy',1=>'BT');
    $rows = array();
    foreach ($data as $hp) {
        $rows[$hp->id] = array(
                            l($hp->mamh,'admin/qlctdt/edit/'.$hp->id),
                            $hp->tenmh,
                            $hp->sotc,
                            $hp->sotc_lt,
                            $hp->sotc_th,
                            $hp->hocky,
                            $hp->khoadt,
                            $hp->hedt,
                            $hp->nganh,
                            $hp->loaihinh,
                            $hp->ghichu,
                            $hp->mactdt,
                            );
    }
    $form['qlctdt_export'] = array(
        '#prefix' => '<div id="export_qlctdt">',
        '#suffix' => '</div>',);
    $form['qlctdt_wraper'] = array(
        '#prefix' => '<div id="table_qlctdt">',
        '#suffix' => '</div>',);
    $form['qlctdt_wraper']['info'] = qlctdt_info_form();
    if ($_GET['q'] == 'system/ajax') {
        $q = 'admin/qlctdt' ;
        $_GET['q'] = $q;
    }
    $form['qlctdt_wraper']['table_hp'] = array(
        '#type' => 'tableselect',
        '#caption' => 'CHƯƠNG TRÌNH ĐÀO TẠO',
        '#header' => $headers,
        '#options' => $rows,
        '#empty' => 'Không có thông tin'
    );
    //$tmp_q = $_GET['q']; 
    //$_GET['q'] = 'admin/qlctdt';
    $form['qlctdt_wraper']['pager'] = array('#markup' => theme('pager'));
    //$_GET['q'] = $tmp_q;
    
    $form['export'] = array( '#type' => 'button',
                             '#value' => t('Xuất ra file excel'),
                             '#attributes' => array('title' =>'Xuất CTĐT ra file excel'),
                             '#executes_submit_callback' => TRUE,
                             '#validate' => FALSE,
                             '#submit' => array('qlctdt_xuatexcel'),
                            );
    if(user_access('Quản lý CTĐT')) {
        $form['xoa'] = array( '#type' => 'button',
                                 '#value' => t('Xóa'),
                                 '#attributes' => array('onclick'=>'return confirm(\'Bạn có chắc xóa các dòng được chọn ?\');','title' =>'Xóa các dòng được chọn'),
                                 '#executes_submit_callback' => TRUE,
                                 '#validate' => FALSE,
                                 '#submit' => array('qlctdt_xoa'),
                                );
        $form['import'] = array( '#type' => 'button',
                                 '#value' => t('Import'),
//                                 '#attributes' => array('title' =>'Import CTĐT từ file excel','onclick'=>'window.open(\''.$base_url.'/admin/qlctdt/import\');'),
                                 '#attributes' => array('title' =>'Import CTĐT từ file excel','onclick'=>'window.open(\''.$base_url.'/?q=admin/qlctdt/import\');'),
                                 '#executes_submit_callback' => FALSE,
                                 '#validate' => FALSE,
                                );
    }
    return $form;
}

function qlctdt_xoa($form, &$form_state) {
   if(!user_access('Quản lý CTĐT') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $arrID = array_filter($form_state['values']['table_hp']);
    $strmsg = '';
    foreach ($arrID as $value) {
        if(XoaCTDT($value)) $strmsg .= $value.'; ';
    }
    if($strmsg != '') {
        drupal_set_message ('Đã xóa thành công các ID: '.$strmsg);
    }else{
        drupal_set_message (' Fail ! ','error');
    }
    drupal_goto('admin/qlctdt');
}

function qlctdt_form_validate($form, &$form_state) {
    $mamh = trim($form_state['values']['mamh_update'], " ");
    //Kiểm tra môn học tồn tại
    $check = db_select('uit_monhoc', 't')->fields('t', array('mamh'))->condition('mamh', $mamh)->execute()->fetchAssoc();
    if(!$check) {
        form_set_error('mamh_update','Mã môn học không hợp lệ');
        return;
    }
    if($form_state['values']['khoadt_update']=="") {
        form_set_error('khoadt_update','Xin vui lòng chọn khóa');
        return;
    }

    $sotc = (int)$form_state['values']['sotc_update'];
    $sotc_lt = (int)$form_state['values']['sotc_lt_update'];
    $sotc_th = (int)$form_state['values']['sotc_th_update'];
    $khoadt = $form_state['values']['khoadt_update'];
    $hedt = $form_state['values']['hedt_update'];
    $nganh = $form_state['values']['nganh_update'];
//    $makhoa = $form_state['values']['makhoa_update'];
    //Kiểm tra SoTC hợp lệ
    if($sotc < 0 || $sotc_lt < 0 || $sotc_th < 0) {
        form_set_error('sotc_update','Số TC không hợp lệ');
    }
    //Kiểm tra Khoa - Ngành - hệ ĐT hợp lệ
    $check = db_select('uit_nganh', 't')->fields('t')->condition('nganh', $nganh)->condition('hedt', '%'.$hedt.'%', 'LIKE')->execute()->fetch();
    if(!$check) {
        form_set_error('hedt_update','Thông tin Hệ ĐT - Ngành không hợp lệ');
        return;
    }
    //Kiểm tra trùng CTĐT
    $check = db_select('uit_ctdaotao', 't')->fields('t', array('mamh'))
            ->condition('mamh', $mamh)->condition('khoadt', $khoadt)
            ->condition('hedt', $hedt)
            ->execute()->fetch();
    if($check) {
        $rID = $form_state['values']['id_update'];
        if($rID == ''){
            form_set_error('mamh_update','Mã môn học ('.$mamh.') bị trùng trong CTĐT (cùng hệ ĐT, Khoa, khóa học)');
            return;
        }
    }
    // Kiểm tra mã chương trình đào tạo
}

function qlctdt_capnhat($form, &$form_state) {
    if(!user_access('Quản lý CTĐT') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $rID = $form_state['values']['id_update'];
//    $mamh = trim($form_state['values']['mamh_update'], " ");
//    $sotc = (int)$form_state['values']['sotc_update'];
//    $sotc_lt = (int)$form_state['values']['sotc_lt_update'];
//    $sotc_th = (int)$form_state['values']['sotc_th_update'];
//    $mactdt=trim($form_state['values']['mactdt_update'], " ");
//    $khoadt = $form_state['values']['khoadt_update'];
//    $hedt = $form_state['values']['hedt_update'];
//    $nganh = $form_state['values']['nganh_update'];
    
    $arrData = array(
        'mamh' => trim($form_state['values']['mamh_update'], " "),
        'sotc' => (int)$form_state['values']['sotc_update'],
        'sotc_lt' =>(int)$form_state['values']['sotc_lt_update'],
        'sotc_th' => (int)$form_state['values']['sotc_th_update'],
        'hocky' => $form_state['values']['hocky_update'],
        'mactdt'=>trim($form_state['values']['mactdt_update'], " "),
        'khoadt' =>  $form_state['values']['khoadt_update'],
        'hedt' => $form_state['values']['hedt_update'],
        'nganh' =>  $form_state['values']['nganh_update'],
        'loaihinh'=>trim($form_state['values']['loaihinh_update'], " "),
        'ghichu' => trim($form_state['values']['ghichu_update'], " "),
    );
    $khoa_data="";
    foreach($arrData['khoadt'] as $single_khoa){
        if($khoa_data!=""){
            $khoa_data.=",".$single_khoa;
        }else{
            $khoa_data=$single_khoa;
        }
    }
    $arrData['khoadt']=$khoa_data;
    try {
        if($rID != '') { //Cập nhật
            db_update('uit_ctdaotao')->fields($arrData)->condition('id', $rID)->execute();
        } else { //Thêm mới
            db_insert('uit_ctdaotao')->fields($arrData)->execute();
        }
    } catch (Exception $exc) {
        drupal_set_message('Có lỗi xảy ra ('.$exc->getMessage().')','error');
        return;
    }
    drupal_set_message('Cập nhật thành công.');
    drupal_goto('admin/qlctdt');
}

function XoaCTDT($id) {
    try {
        db_delete('uit_ctdaotao')->condition('id',$id)->execute();
    } catch (Exception $exc){
        drupal_set_message('Xóa bị lỗi ('.$exc->getMessage().')', 'error');
        return FALSE;
    }
    return TRUE;
}


function qlctdt_info_form() {
    $query = db_select('uit_ctdaotao','c');
    $query->addExpression('count(c.id)', 'tong');
    //$query->leftJoin('uit_monhoc', 'm', 'm.mamh = c.mamh');
    qlctdt_build_filter_query( $query);
    $tong = $query->execute()->fetchField();
    $form['info'] = array(array('#markup' =>'<strong>Tổng cộng: '.number_format($tong).'</strong>'));
    return $form;
}

function qlctdt_xuatexcel($form, &$form_state) {
    //global $base_url;
    $query = db_select('uit_ctdaotao','c');
    $query->leftJoin('uit_monhoc', 'm', 'm.mamh = c.mamh');
    $query->fields('c');
    $query->fields('m', array('tenmh'));
    qlctdt_build_filter_query( $query);
    $query->orderBy('c.hedt','ASC');
    $query->orderBy('c.khoadt','ASC');
    $query->orderBy('c.hocky','ASC');
    $query->orderBy('c.mamh','ASC');
    $data = $query->execute();
    $filename = doExport($data);
    if(!file_exists($filename)) {
        drupal_set_message ('Có lỗi xảy ra, không xuất được file.', 'error');
        return;
    } 
    drupal_add_http_header('Pragma', 'public');
    drupal_add_http_header('Expires', '0');
    drupal_add_http_header('Cache-Control', 'must-revalidate, post-check=0, pre-check=0');
    drupal_add_http_header('Content-Type', 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
    drupal_add_http_header('Content-Disposition', 'attachment; filename=' . basename($filename) . ';');
    drupal_add_http_header('Content-Transfer-Encoding', 'binary');
    drupal_add_http_header('Content-Length', filesize($filename));
    readfile($filename);
    unlink($filename);
    drupal_exit();
}

function doExport($rows){
    if(count($rows) == 0) return;
    
    $filename = 'CTDT_'.date('d').''.date('m').''.date('h').''.date('i').''.date('s').'.xlsx';
    $path = variable_get('file_public_path', conf_path() . '/files') . '/exports';
    if(!file_exists($path)) mkdir($path);
    $path .= '/'.$filename;
    $arrHTTH = array(0=>'Không TH',1=>'HT1',2=>'HT2');
    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    $objPHPExcel = new PHPExcel();
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    $c = 'A';
    $r = 1;
    $ActiveSheet->setCellValue($c++.$r, 'STT');
    $ActiveSheet->setCellValue($c++.$r, 'Mã MH');
    $ActiveSheet->setCellValue($c++.$r, 'Tên môn học');
    $ActiveSheet->setCellValue($c++.$r, 'Số TC');
    $ActiveSheet->setCellValue($c++.$r, 'Tiết LT');
    $ActiveSheet->setCellValue($c++.$r, 'Tiết TH');
    $ActiveSheet->setCellValue($c++.$r, 'Học phần');
    $ActiveSheet->setCellValue($c++.$r, 'Năm');
    $ActiveSheet->setCellValue($c++.$r, 'Hệ ĐT');
    $ActiveSheet->setCellValue($c++.$r, 'Ngành');
    $ActiveSheet->setCellValue($c++.$r, 'Loại môn học');
    $ActiveSheet->setCellValue($c++.$r, 'Ghi chú');
    $r = 2;
    $n = 1;
    //$ActiveSheet->getStyle('H')->getNumberFormat()->setFormatCode("#,##0");
    foreach($rows as $row){
        $c = 'A';
        $ActiveSheet->setCellValue($c++.$r, $n++);
        $ActiveSheet->setCellValue($c++.$r, $row->mamh);
        $ActiveSheet->setCellValue($c++.$r, $row->tenmh);
        $ActiveSheet->setCellValue($c++.$r, $row->sotc);
        $ActiveSheet->setCellValue($c++.$r, $row->sotc_lt);
        $ActiveSheet->setCellValue($c++.$r, $row->sotc_th);
        $ActiveSheet->setCellValue($c++.$r, $row->hocky);
        $ActiveSheet->setCellValue($c++.$r, $row->khoadt);
        $ActiveSheet->setCellValue($c++.$r, $row->hedt);
        $ActiveSheet->setCellValue($c++.$r, $row->nganh);
        $ActiveSheet->setCellValue($c++.$r, $row->loaihinh);
        $ActiveSheet->setCellValue($c++.$r, $row->ghichu);
        $r++;
    }
    $objWriter = new PHPExcel_Writer_Excel2007($objPHPExcel);
    ob_end_clean();
    $objWriter->save($path);
    return $path;
}



function ajax_filter_qlctdt_callback($form, $form_state) {
  return $form['qlctdt_wraper'];
}

function qlctdt_set_filters(&$form_state) {
  $filters = qlctdt_filters();
  foreach ($filters as $filter => $options) {
    if (isset($form_state['values'][$filter])) {
      if( $form_state['values'][$filter] != '') {
        $_SESSION['qlctdt_filter'][$filter] = $form_state['values'][$filter];
      }
      else {
        unset($_SESSION['qlctdt_filter'][$filter]);
      }
    } 
  }
}

function qlctdt_filters() {
  $filters = array();  
  //unset($_SESSION['qlctdt_filter']);
  if(!isset($_SESSION['qlctdt_filter']['sodong'])) $_SESSION['qlctdt_filter']['sodong'] = 100;
  $session = isset($_SESSION['qlctdt_filter']) ? $_SESSION['qlctdt_filter'] : array();
  $mamh = !empty($session['mamh']) ? $session['mamh'] : NULL;
  $hedt = !empty($session['hedt']) ? $session['hedt'] : NULL;
  $nganh = !empty($session['nganh']) ? $session['nganh'] : NULL;
  $hocky = !empty($session['hocky']) ? $session['hocky'] : NULL;
  $sodong = !empty($session['sodong']) ? $session['sodong'] : 50;
  
  $arrhocky = array(''=>'Tất cả');
  for($i = 1;$i < 15;$i++) {
      $arrhocky[$i] = $i;
  }
  
    $filters['mamh'] = array(
        'type' => 'textfield',
        'title' => t('Mã MH:'),
        'size' => 36,
        'default_value' => array($mamh),
        'attributes' => array('title' =>'Có thể nhập nhiều mã môn học cách nhau bởi dấu phẩy (,).'),
    );
    $filters['hedt'] = array(
                      'type' => 'select',
                      'title' => t('Hệ ĐT'),
                      'options' => (array(''=>'Tất cả') + db_select('uit_thongtinhv', 'd')->distinct()->orderBy('d.hedt')->fields('d', array('hedt', 'hedt'))->execute()->fetchAllKeyed()),
                      'default_value' => array($hedt),
                    );
    $filters['nganh'] = array(
                      'type' => 'select',
                      'title' => t('Ngành'),
                      'options' => (array(''=>'Tất cả') + db_select('uit_nganh', 'd')->orderBy('d.nganh')->fields('d', array('nganh', 'tennganh'))->execute()->fetchAllKeyed()),
                      'default_value' => array($nganh),
                    );
    $filters['hocky'] = array(
                      'type' => 'select',
                      'title' => t('Học phần'),
                      'options' => $arrhocky,
                      'default_value' => array($hocky),
                    );
    $filters['sodong'] = array(
          'type' => 'select',
          'title' => t('Số dòng'),
          'options' => (array('10' => t('10'),'50' => t('50'),'100'=>t('100'),'200'=>t('200'),'500'=>t('500'))),
          'default_value' => array($sodong),
    );
    
    $filters['timkiem'] = array(
        'type' => 'button',
        'value' => t('Tìm kiếm'),
        'validate' => FALSE,
        'ajax' => array(
          'event' => 'click',
          'callback' => 'ajax_filter_qlctdt_callback',
          'wrapper' => 'table_qlctdt',
        ),
    );
  
  return $filters;
}

function qlctdt_filter_form() {
    $form = array();
    $filters = qlctdt_filters();
    foreach ($filters as $key => $filter) {
        $arr = array();
        foreach ($filter as $key2 => $value2) {
            $arr['#'.$key2] = $value2;
        }
        $form['filters'][$key] = $arr;
    }
    return $form;
}

function qlctdt_build_filter_query(SelectQueryInterface $query) {
    $filter_data = isset($_SESSION['qlctdt_filter']) ? $_SESSION['qlctdt_filter'] : array();
    foreach ($filter_data as $key => $value) {
        switch ($key) {
            case 'timkiem':
            case 'sodong':
                break;
            case 'mamh':
                $arrmamh = explode(',', $value);
                $query->condition('c.'.$key, $arrmamh, 'IN');
                break;
            
            default:
                $query->condition('c.'.$key, $value);
        }
    }
}
