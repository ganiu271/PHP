<?php
//$nganh='000', $mamh='000', $coso='00', $mahv=NULL
function qldiem_form_xemdiem($form, &$form_state, $para) {
    global $base_url;
    if(!user_access('Quan ly diem') && !user_access('Xem diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $arr = explode(';', $para);
    if(count($arr) < 1){

    }else{
        $malop = $arr[0];  $mahv='';
        if(isset($arr[1])) {$mahv = $arr[1];}
        //$tenmh = db_select('uit_ctdaotao','mh')->fields('mh',array('tenmh'))->condition('mh.mamh', $mamh)->condition('mh.nganh', $nganh)->execute()->fetchField();
        $html = '<center><h1>BẢNG ĐIỂM LỚP '.strtoupper($malop).'</h1></center>';
        //$html .= '<h3>Môn học: '.$tenmh.' ('.$mamh.')</h3>';
        $form['markup'] = array('#markup' => $html);

        if($mahv != '') {
            $_SESSION['qldiem_xemdiem_filter']['mahv'] = $mahv;
        } elseif(!isset($form_state['values']['mahv'])) {
            unset($_SESSION['qldiem_xemdiem_filter']['mahv']);
        }
        $form['qldiem_xemdiem_filter_wraper'] = array('#type' => 'fieldset', '#title' => t('Chọn điều kiện lọc'), '#attributes' => array('class' => array('container-inline')));
        qldiem_xemdiem_set_filters($form_state);
        $form['qldiem_xemdiem_filter_wraper'][] = qldiem_xemdiem_filter_form();

        $headers = array(
            array('data' => 'STT', 'field' => ''),
            array('data' => 'Mã HV', 'field' => 'mahv'),
            array('data' => 'Điểm GK', 'field' => 'diem3'),
            array('data' => 'Điểm TH', 'field' => 'diem2'),
            array('data' => 'Điểm CK', 'field' => 'diem4'),
            array('data' => 'Điểm HP', 'field' => 'diem'),
            array('data' => 'Trạng thái', 'field' => ''),
            array('data' => 'Ghi chú', 'field' => 'thoigianketthuc'),
            array('data' => 'Chức năng', 'field' => ''),
        );

        $query = db_select('uit_diem','d')->extend('TableSort')->orderByHeader($headers);
        $query->join('uit_thongtinhv', 'hv', "hv.mahv = d.mahv");
        //$query->join('uit_ctdaotao', 'm', "m.mamh = d.mamh and m.nganh = d.nganh");
        $query->fields('d');
        //$query->condition('hv.coso', $coso);
        //$query->condition('hv.khoahoc', $khoahoc);
        //$query->condition('m.nganh', $nganh);
        $query->condition('d.malop', $malop);
        $query->orderBy('d.mahv','ASC');
        qldiem_xemdiem_build_filter_query( $query);
        $data = $query->execute()->fetchAll();

        $arrtrangthai = array(0 => 'Hủy', 1 => 'Bình thường', 2 => 'Trả nợ', 3 => 'Cải thiện', 4 => 'Miễn', 5 => 'Hoãn thi',6=>'Khác');
        $rows = array();
        $i=1;
        foreach ($data as $diem) {
            $strfunc = l('Sửa',$base_url.'/admin/qldiem/edit/'.$diem->id,array('attributes'=>array('target'=>'_blank')));
            $rows[$diem->id.';'.$diem->mahv] = array(
                $i++,
                $diem->mahv,
                array('data'=>$diem->diem3,'title'=>($diem->heso3 > 0 ? 'Trọng số: '.($diem->heso3*100).'%':'')),
                array('data'=>$diem->diem2,'title'=>($diem->heso2 > 0 ? 'Trọng số: '.($diem->heso2*100).'%':'')),
                array('data'=>$diem->diem4,'title'=>($diem->heso4 > 0 ? 'Trọng số: '.($diem->heso4*100).'%':'')),
                $diem->diem,
                $arrtrangthai[$diem->trangthai],
                $diem->thoigianketthuc,
                $strfunc,
            );
        }
        $form[] = CongCuXuLy('_h');
        $form['qldiem_xemdiem_wraper'] = array(
            '#prefix' => '<div id="table_qldiem_xemdiem">',
            '#suffix' => '</div>',);
        $form['qldiem_xemdiem_wraper']['table_diem'] = array(
            '#type' => 'tableselect',
            '#caption' => 'BẢNG ĐIỂM',
            '#header' => $headers,
            '#options' => $rows,
            '#empty' => 'Không có HV nào'
        );

        //$form['nganh'] = array( '#type' => 'hidden',
        //                        '#value' => $nganh,
        //                       );
        $form['malop'] = array( '#type' => 'hidden',
            '#value' => $malop,
        );
        $form[] = CongCuXuLy('_f');
    }
    return $form;
}

function CongCuXuLy($str = '') {
    $form = array();
    $form['xuatdiem'.$str] = array( '#type' => 'button',
                             '#value' => t('Xuất Excel'),
                             '#attributes' => array('title' =>'Xuất bảng điểm ra file excel'),
                             '#executes_submit_callback' => TRUE,
                             '#submit' => array('qldiem_xuatbangdiem'),
                            );
    if(user_access('Quan ly diem') ) {
        $form['xoadiem'.$str] = array( '#type' => 'button',
                                 '#value' => t('Xóa'),
                                 '#attributes' => array('onclick'=>'return confirm(\'Bạn có chắc xóa điểm các hv được chọn ?\');','title' =>'Xóa điểm các hv được chọn'),
                                 '#executes_submit_callback' => TRUE,
                                 '#submit' => array('qldiem_xoadiemhv'),
                                );
    }
    $form['goback'.$str] = array( '#type' => 'button',
                             '#value' => t('Đóng'),
                             '#attributes' => array('onclick'=>'window.close();'),
                             '#executes_submit_callback' => FALSE,
                            );
    return $form;
}

function qldiem_xuatbangdiem($form, &$form_state) {
    $malop = $form_state['values']['malop'];
    //$mamh = $form_state['values']['mamh'];
    
    $query = db_select('uit_diem','d');
    $query->join('uit_thongtinhv', 'hv', "hv.mahv = d.mahv");
    $query->fields('d');
    $query->fields('hv',array('hoten'));
    $query->condition('d.malop', $malop);
    //$query->condition('d.mamh', $mamh);
    $query->orderBy('d.mahv','ASC');    
    qldiem_xemdiem_build_filter_query( $query);
    $data = $query->execute();
    if(!$data) return;
    
    $filename = 'BangDiem_'.$malop.'.xlsx';
    $path = variable_get('file_public_path', conf_path() . '/files') . '/exports';
    if(!file_exists($path)) mkdir($path);
    $path .= '/'.$filename;
    
    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    $objPHPExcel = new PHPExcel();
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    $c = 'A';
    $r = 1;
    $ActiveSheet->setCellValue($c++.$r, 'STT');
    $ActiveSheet->setCellValue($c++.$r, 'Mã HV');
    $ActiveSheet->setCellValue($c++.$r, 'Họ tên');
    $ActiveSheet->setCellValue($c++.$r, 'Điểm GK');
    $ActiveSheet->setCellValue($c++.$r, 'Điểm TH');
    $ActiveSheet->setCellValue($c++.$r, 'Điểm CK');
    $ActiveSheet->setCellValue($c++.$r, 'Điểm HP');
    $ActiveSheet->setCellValue($c++.$r, 'Ghi chú');
    $r = 2;
    $n = 1;
    $ActiveSheet->getStyle('E')->getNumberFormat()->setFormatCode("#,##0");
    foreach($data as $row){
        $c = 'A';
        $ActiveSheet->setCellValue($c++.$r, $n++);
        $ActiveSheet->setCellValueExplicit($c++.$r,$row->mahv, PHPExcel_Cell_DataType::TYPE_STRING);
        $ActiveSheet->setCellValue($c++.$r, $row->hoten);
        $ActiveSheet->setCellValue($c++.$r, $row->diem3);
        $ActiveSheet->setCellValue($c++.$r, $row->diem2);
        $ActiveSheet->setCellValue($c++.$r, $row->diem4);
        $ActiveSheet->setCellValue($c++.$r, $row->diem);
        $ActiveSheet->setCellValue($c++.$r, $row->thoigianketthuc);
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

function qldiem_xoadiemhv($form, &$form_state) {
    if(!user_access('Quan ly diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $arrID = array_filter($form_state['values']['table_diem']);
    $malop = $form_state['values']['malop'];
    $arr = explode('.', $malop);
    $mamh = $arr[0];
    $strid = '';
    
    foreach ($arrID as $id) {
        $arr = explode(';', $id);
        if(XoaDiemhv($arr[0])) {
            CapNhatTrangThai($arr[1], $mamh);
            $strid .= $arr[1].'; ';
        }
    }
    if($strid != '') {
        drupal_set_message ('Đã xóa thành công các MShv: '.$strid);
        ghilogs('Xóa điểm hv lớp '.$malop.' MShv: '.$strid, $mamh);
    }
    return;
    //drupal_goto('admin/qldiem/xemdiem/'.$malop.'/'.$mamh);
}

function XoaDiemhv($id) {
    try {
        db_delete('uit_diem')->condition('id',$id)->execute();
    } catch (Exception $exc){
        drupal_set_message('Phát sinh lỗi: '.$exc->getMessage(),'error');
        return FALSE;
    }
    return TRUE;
}

function qldiem_themdiem($form, &$form_state) {
    $form = array();
    $form['markup'] = array('#markup' => '<h2>Chức năng này đang xây dựng</h2>');
    return $form;
}

function qldiem_xemdiem_form_edit($form, &$form_state, $id) {
    if(!user_access('Quan ly diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    
    $html = '<center><h1 class="title-page">CẬP NHẬT ĐIỂM</h1></center>';
    $form['markup'] = array('#markup' => $html);
    $diem = db_select('uit_diem','d')
            ->fields('d')
            ->condition('d.id', $id)
            ->execute()->fetch();
    //set value//
    $mahv = $diem->mahv;
    $diemgk = $diem->diem3;
    $diemck = $diem->diem4;
    $diemth = $diem->diem2;
    $diemhp = $diem->diem;
    $thoigianketthuc = $diem->thoigianketthuc;
    //$malop = $diem->malop;
    $mamh = $diem->mamh;
    $trangthai = $diem->trangthai;
    //default value
    $form['rowid'] = array('#type' => 'hidden', '#default_value'=>$id);
    //$form['malop'] = array('#type' => 'hidden', '#default_value'=>$malop);
    $form['mamh'] = array('#type' => 'hidden', '#default_value'=>$mamh);
    $form['mahv'] = array('#type' => 'hidden', '#default_value'=>$mahv);
    
    //form input update//
    $form['update_form_diemct'] = array('#type' => 'fieldset', 
        '#title' => t('Chỉnh sửa điểm sinh viên'), 
        '#attributes' => array('class' => array('container-inline')));
    $form['update_form_diemct']['GK'] = array(
        '#title'=>t('Điểm GK:'.($diem->heso3 > 0 ? ' ('.($diem->heso3*100).'%)':'')),
        '#type'=>'textfield',
        '#default_value'=>$diemgk,
        '#size'=>10
    );
    $form['update_form_diemct']['TH'] = array(
        '#title'=>t('Điểm TH:'.($diem->heso2 > 0 ? ' ('.($diem->heso2*100).'%)':'')),
        '#type'=>'textfield',
        '#default_value'=>$diemth,
        '#size'=>10
    );
    $form['update_form_diemct']['CK'] = array(
        '#title'=>t('Điểm CK:'.($diem->heso4 > 0 ? ' ('.($diem->heso4*100).'%)':'')),
        '#type'=>'textfield',
        '#default_value'=>$diemck,
        '#size'=>10
    );
    $form['update_form_diemct']['HP'] = array(
        '#title'=>t('Điểm HP:'),
        '#type'=>'textfield',
        '#default_value'=>$diemhp,
        '#size'=>($diem->heso4 > 0 ? 20:30),
        '#disabled'=>($diem->heso4 > 0 ? TRUE:FALSE),
        //'#attributes'=>array('disabled'=>'TRUE'),
    );
    $form['update_form_diemct']['trangthai'] = array(
        '#title'=>t('Trạng thái:'),
        '#type'=>'select',
        '#options'=>array(0 => 'Hủy', 1 => 'Bình thường', 2 => 'Đã trả nợ', 3 => 'Đã cải thiện', 4 => 'Khác', 5 => 'Hoãn thi'),
        '#default_value'=>$trangthai,
    );
    $form['update_form_diemct']['note'] = array(
        '#title'=>t('<br>Ghi chú:'),
        '#type'=>'textfield',
        '#id'=>'txtnote',
        '#default_value'=>$thoigianketthuc,
        '#size'=>100
    );
    //set value of form//
    
    //control button
    $form['update'] = array('#type'=>'submit'
        , '#value'=>'Cập nhật'
        , '#submit'=>array('edit_form_submit')
        );
    //button back//
    $form['goback'] = array( '#type' => 'button',
                             '#value' => t('Đóng'),
                             '#attributes' => array('onclick'=>'window.close();'),
                             '#executes_submit_callback' => FALSE,
                            );
    
    $form['bk_diemgk'] = array('#type' => 'hidden', '#default_value'=>$diemgk);
    $form['bk_diemth'] = array('#type' => 'hidden', '#default_value'=>$diemth);
    $form['bk_diemck'] = array('#type' => 'hidden', '#default_value'=>$diemck);
    $form['bk_diemhp'] = array('#type' => 'hidden', '#default_value'=>$diemhp);
    $form['bk_trangthai'] = array('#type' => 'hidden', '#default_value'=>$trangthai);
    $form['hesogk'] = array('#type' => 'hidden', '#default_value'=>$diem->heso3);
    $form['hesoth'] = array('#type' => 'hidden', '#default_value'=>$diem->heso2);
    $form['hesock'] = array('#type' => 'hidden', '#default_value'=>$diem->heso4);
    
    return $form;
}

function edit_form_submit($form, &$form_state) {
    $mahv = $form_state['values']['mahv'];
    $diemgk = (float)$form_state['values']['GK'];
    $hesogk = (float)$form_state['values']['hesogk'];
    $diemck = (float)$form_state['values']['CK'];
    $hesock = (float)$form_state['values']['hesock'];
    $diemth = (float)$form_state['values']['TH'];
    $hesoth = (float)$form_state['values']['hesoth'];
    $diemhp = (float)$form_state['values']['HP'];
    if($hesock > 0) {
        $diemhp = MROUND($diemgk*$hesogk + $diemth*$hesoth + $diemck*$hesock, 0.5);
    }
    $trangthai = (int)$form_state['values']['trangthai'];
    $thoigianketthuc = $form_state['values']['note'];
    $id = $form_state['values']['rowid'];
    //$malop = $form_state['values']['malop'];
    $mamh = $form_state['values']['mamh'];
    
    $bk_diemgk = (float)$form_state['values']['bk_diemgk'];
    $bk_diemck = (float)$form_state['values']['bk_diemck'];
    $bk_diemth = (float)$form_state['values']['bk_diemth'];
    $bk_diemhp = (float)$form_state['values']['bk_diemhp'];
    $bk_trangthai = (int)$form_state['values']['bk_trangthai'];
    
    if($trangthai != $bk_trangthai && $trangthai != 0 && $trangthai != 5) {
        drupal_set_message('Chỉ được chuyển trạng thái hủy điểm, hoãn thi, các trạng thái khác hệ thống sẽ tự cập nhật','error');
        return;
    }
    $transaction = db_transaction();
    try {
        db_update('uit_diem')
        ->fields(array(
            'diem3'=>$diemgk,
            'diem2'=>$diemth,
            'diem4'=>$diemck,
            'diem'=>$diemhp,
            'trangthai'=>$trangthai,
            'thoigianketthuc'=>$thoigianketthuc,
        ))
        ->condition('id', $id)
        ->execute();
        CapNhatTrangThai($mahv,$mamh);
    } catch (Exception $exc) {
        $transaction->rollback();
        drupal_set_message($exc->getMessage(),'error');
        drupal_goto('admin/qldiem/edit/'.$id);
        //watchdog_exception('error', $exc);
    }
    $log = 'Cập nhật điểm của hv '.$mahv.' mã môn học '.$mamh.' Nội dung cập nhật: ';
    if($diemgk != $bk_diemgk) $log .= ' điểm GK:'.$bk_diemgk.'->'.$diemgk;
    if($diemgk != $bk_diemth) $log .= ' điểm TH:'.$bk_diemth.'->'.$diemth;
    if($diemck != $bk_diemck) $log .= ' điểm CK:'.$bk_diemck.'->'.$diemck;
    if($diemhp != $bk_diemhp) $log .= ' điểm HP:'.$bk_diemhp.'->'.$diemhp;
    if($trangthai != $bk_trangthai) $log .= ' Trạng thái:'.$bk_trangthai.'->'.$trangthai;
    ghilogs($log, $mahv,'qldiem');
    drupal_set_message('Cập nhật thành công');
    return $form;
//    drupal_goto('admin/qldiem/xemdiem/'.$mamh);
}

function ajax_filter_qldiem_xemdiem_callback($form, $form_state) {
  return $form['qldiem_xemdiem_wraper'];
}

function qldiem_xemdiem_set_filters(&$form_state) {
  $filters = qldiem_xemdiem_filters();
  
  foreach ($filters as $filter => $options) {
    if (isset($form_state['values'][$filter])) {
      if( $form_state['values'][$filter] != '') {
        $_SESSION['qldiem_xemdiem_filter'][$filter] = $form_state['values'][$filter];
      }
      else {
        unset($_SESSION['qldiem_xemdiem_filter'][$filter]);
      }
    } 
  }
}

function qldiem_xemdiem_filters() {
  $filters = array();  
  $session = isset($_SESSION['qldiem_xemdiem_filter']) ? $_SESSION['qldiem_xemdiem_filter'] : array();
  $mahv = !empty($session['mahv']) ? $session['mahv'] : (isset($_REQUEST['mahv']) ? $_REQUEST['mahv']:'');
  $trangthai = !empty($session['trangthai']) ? $session['trangthai'] : NULL;
  
    $filters['mahv'] = array(
        'type' => 'textfield',
        'title' => t('Mã HV:'),
        'size' => 20,
        'default_value' => array($mahv),
    );
    $filters['trangthai'] = array(
        'title'=>t('Trạng thái:'),
        'type'=>'select',
        'options'=>array('' => 'Tất cả',0 => 'Hủy', 1 => 'Bình thường', 2 => 'Đã trả nợ', 3 => 'Đã cải thiện', 4 => 'Miễn', 5 => 'Hoãn thi',6=>'Khác'),
        'default_value'=>$trangthai,
    );
    $filters['timkiem'] = array(
        'type' => 'button',
        'value' => t('Tìm kiếm'),
        'ajax' => array(
          'event' => 'click',
          'callback' => 'ajax_filter_qldiem_xemdiem_callback',
          'wrapper' => 'table_qldiem_xemdiem',
        ),
    );
  
  return $filters;
}

function qldiem_xemdiem_filter_form() {
    $form = array();
    $filters = qldiem_xemdiem_filters();
    foreach ($filters as $key => $filter) {
        $arr = array();
        foreach ($filter as $key2 => $value2) {
            $arr['#'.$key2] = $value2;
        }
        $form['filters'][$key] = $arr;
    }
    return $form;
}

function qldiem_xemdiem_build_filter_query(SelectQueryInterface $query) {
    $filter_data = isset($_SESSION['qldiem_xemdiem_filter']) ? $_SESSION['qldiem_xemdiem_filter'] : array();
    foreach ($filter_data as $key => $value) {
        if($key == 'timkiem') continue;
        $query->condition('d.'.$key, $value);
    }
}

function qldiem_xemdiemhv($form, &$form_state, $mahv) {
    global $user, $base_url;
    if(!user_access('Xem diem')) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $q = db_select('uit_thongtinhv', 't')->fields('t');
    $q->condition('mahv', $mahv);
    $hv = $q->execute()->fetch();
    if(!$hv) return "Không tìm thấy thông tin sinh viên này.";

    $q = db_select('uit_diem', 'd');
    $q->leftJoin('uit_ctdaotao','ctdt','d.mamh=ctdt.mamh');
    $q->leftJoin('uit_monhoc','mh','d.mamh=mh.mamh');
    $q->fields('d');
    $q->fields('ctdt',array('id'));
//    $q->fields('ctdt');
    $q->fields('mh');
    $q->condition('d.mahv', $mahv);
    $q->condition('d.trangthai', array(1,4));
    $q->orderBy('d.id','ASC');
    $q->orderBy('d.sotc','ASC');
//    $q->orderBy('d.hocky','ASC');
//    $q->orderBy('d.mamh','ASC');
    $rows = $q->execute()->fetchAll();
    if(!$rows) {
        echo 'Khong co thong tin diem.';
        die;
    }
    $ctdt = db_select('uit_ctdaotao','c')->fields('c',array('mamh','sotc'))
        ->condition('hedt', $hv->hedt)->condition('nganh', $hv->chuyennganh)
        ->execute()->fetchAllKeyed();
    $lam_lv=false;
    $arrDiem = array();
    foreach ($rows as $row) {
        $arrDiem[$row->mamh] = array(
            'tenmh' => $row->tenmh,
            'sotc' => $row->sotc,
            'diem' => ($row->trangthai==4)?'Miễn':$row->diem,
            'trangthai' => $row->trangthai,
        );
        if($row->sotc==15){
            $lam_lv=true;
        }
    }
    $hoten = $user->name;
    $gioitinh = ($hv->gioitinh == 1 ? "Nam":"Nữ");
    $ngaysinh = (strlen($hv->ngaysinh) == 10 ? date("d-m-Y",strtotime($hv->ngaysinh)):$hv->ngaysinh);
    $chuyennganh=db_select('uit_nganh','ng')->fields('ng')->condition('nganh',$hv->chuyennganh)->execute()->fetchAll();
    $style_css = '<style type="text/css" media="screen">
        #print-footer {
            display: none;
        }
        #bd_header {
            display: none;
        }
        td, th {
            font-size: 10pt;
        }
        </style>
        <style type="text/css" media="print">
        #print-footer {
            width: 100%;
            display: block;
            position: fixed;
            bottom: 0;
            text-align: center;
            margin: 0px 0 10px 0;
        }
        #print-footer hr {
            margin: 0 0 10px 0;
        }
        td, th {
            font-size: 10pt;
        }
        </style>';
    $header_first = '<div id="bd_header" class="header">
		<div class="logo">
			<img width="80" alt="logo" src="'.$base_url.'/'.drupal_get_path('module', 'qldiem').'/images/logo.png">
		</div>
		<div class="logo-char">
			<p class="char-1">ĐẠI HỌC QUỐC GIA THÀNH PHỐ HỒ CHÍ MINH</p>
			<p class="char-1"><strong>TRƯỜNG ĐẠI HỌC CÔNG NGHỆ THÔNG TIN</strong></p>
			<hr>
		</div>
		<div class="logo-char divright">
			<p class="char-1"><strong>CỘNG HOÀ XÃ HỘI CHỦ NGHĨA VIỆT NAM</strong></p>
			<p class="char-1"><strong>Độc lập - Tự do - Hạnh phúc</strong></p>
			<hr>
		</div>
	</div>';
    $header_second = '<div id="bd_header" class="header">
		<div class="logo">
			<img width="80" alt="logo" src="'.$base_url.'/'.drupal_get_path('module', 'qldiem').'/images/logo.png">
		</div>
		<div class="logo-char">
			<p class="char-1">ĐẠI HỌC QUỐC GIA THÀNH PHỐ HỒ CHÍ MINH</p>
			<p class="char-1"><strong>TRƯỜNG ĐẠI HỌC CÔNG NGHỆ THÔNG TIN</strong></p>
			<hr>
		</div>
	</div>';
    $footer = '<div id="print-footer">
		<hr>
		Địa chỉ: KM 20 Xa lộ Hà Nội, Phường Linh Trung, Quận Thủ Đức<br>
		Điện thoại: (08) 372 52002 &nbsp;&nbsp;&nbsp;&nbsp; Fax: (08) 372 52148 &nbsp;&nbsp;&nbsp;&nbsp; Website: <a href="http://www.uit.edu.vn/">http://www.uit.edu.vn</a>
	</div>';
    $retdata = $style_css.$header_first;
//    $trinhdo=($hv->hedt='CH')?'THẠC SĨ':'NGHIÊN CỨU SINH';
    $chuongtrinh_daotao=($lam_lv==true)?'Giảng dạy môn học có luận văn':'Giảng dạy môn học không luận văn';
    $gioitinh=($hv->gioitinh=0)?'Nữ':'Nam';
    $retdata .= '<div id="bd_title_h2">BẢNG ĐIỂM CAO HỌC</div>';
    $retdata .= '<table cellpadding="0" cellspacing="0" border="0" bordercolor="FFFFFF" width="100%" >';
    $retdata .= '<tr><td>Họ và tên: <strong>&nbsp;'.$hv->hoten.'</strong></td><td>Giới tính : <strong>'.$gioitinh.'</strong></td></tr>';
    $retdata .= '<tr><td nowrap>Ngày sinh: <strong>&nbsp;'.$ngaysinh.'</strong></td><td>Nơi sinh: <strong></strong></td></tr>';
    $retdata .= '<tr><td>MSHV :<strong>&nbsp;'.$hv->mahv.' </strong></td><td>Khóa:&nbsp;<strong>'.$hv->khoahoc.'</strong></td></tr>';
    $retdata .= '<tr><td>Chương trình đào tạo :&nbsp;<strong>'.$chuongtrinh_daotao.'</strong></td><td></td></tr>';
    $retdata .= '<tr><td>Chuyên ngành:&nbsp;<strong>'.$chuyennganh['0']->tennganh.'</strong></td><td></td></tr>';
    $retdata .= '</table>';

    $tblheader = '<table cellpadding="2" cellspacings="0" border="1" bordercolor="#000000"  width="100%" >';
    $tblheader .= '<tr>
            <th width="5%" style="text-align:center; padding-right:0">&nbsp;Số TT</th>
            <th width="50%" style="text-align:center; padding-right:0">&nbsp;Môn học</th>
            <th width="20%" style="text-align:center; padding-right:0">&nbsp;Mã số</th>
            <th width="10%" nowrap style="text-align:center; padding-right:0">&nbsp;Số tín chỉ</th>
            <th width="15%" nowrap style="text-align:center; padding-right:0">&nbsp;Điểm môn học</th>';
    $retdata .= $tblheader;

    $tongtc = 0;
    $tongtctl = 0;
    $tongdiemtc = 0;
    $tongtc_mien=0;
    $n=1;
    foreach ($arrDiem as $mamh => $row) {
        $styletr = "";
        if($row['trangthai'] > 1) {
            $styletr = ' style="background-color:#ffbfdf;"';
        }
        $retdata .= "<tr".$styletr.">
            <td align='center'>".$n++."</td>
            <td >".$row['tenmh']."</td>
            <td align='center'>&nbsp;".$mamh."</td>
            <td align='center'>".($row['sotc']>0 ? $row['sotc']:"")."</td>
            <td align='center'>".($row['diem'])."</td>
            </tr>";
        if($n == 26 || ($n - 26)%29 == 0) {
            $retdata .= '</table>';
            $retdata .= $footer;
            $retdata .= '<p style="page-break-after: always;"></p>';
            $retdata .= $header_second;
            $retdata .= '<div id="bd_title_h2"></div>';
            $retdata .= $tblheader;
        }
//        if($row['sotc'] > 0 && $row['trangthai'] == 1) {
        if($row['sotc'] > 0 ) {
            if($row['trangthai']==4){
                $tongtc_mien += $row['sotc'];
            }else{
                $tongtc += $row['sotc'];
                $tongdiemtc += $row['diem']*$row['sotc'];
            }
            if($row['diem'] >= 5 || $row['trangthai']==4) {
                $tongtctl += $row['sotc'];
            }
        }
    }
    if($tongtc > 0) {
        $dtb=round($tongdiemtc/$tongtc,2);
        $retdata .= "<tr><td colspan=3 align='right'><strong>&nbsp;&nbsp;Tổng số tín chỉ</strong></td><td align='center'><strong>".$tongtctl."</strong></td><td>&nbsp;</td></tr>";
        $retdata .= "<tr><td colspan=4 align='right'><strong>&nbsp;&nbsp;Điểm trung bình chung các môn học</strong></td><td align='center'><strong>".$dtb."</strong></td></tr>";
    }
    $retdata .= '</table>';
    $bd_footer = '<table width="100%">';
    if($lam_lv){
        $bd_footer .= '<tr><td width="50%" >Đề tài luận văn&nbsp;</td><td width="50%" align="center"><i>TP.HCM, ngày '.date('d').' tháng '.date('m').' năm '.date('Y').'</i></td></tr>';
        $bd_footer .= '<tr><td width="50%">Hội đồng chấm luận văn gồm:&nbsp;</td><td width="50%" align="center"><strong>TL.HIỆU TRƯỞNG</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1. Chủ tịch:</td><td width="50%" align="center"><strong>TRƯỞNG PHÒNG ĐTSĐH–KHCN&QHĐN</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. Thư ký:</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3. Phản biện 1:</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4. Phản biện 2:</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5. Ủy viên:</td><td width="50%" align="center"><strong>Nguyễn Phi Khứ</strong></td></tr>';
    }else{
        $bd_footer .= '<tr><td width="50%" >&nbsp;</td><td width="50%" align="center"><i>TP.HCM, ngày '.date('d').' tháng '.date('m').' năm '.date('Y').'</i></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;</td><td width="50%" align="center"><strong>TL.HIỆU TRƯỞNG</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"><strong>TRƯỞNG PHÒNG ĐTSĐH–KHCN&QHĐN</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"><strong>Nguyễn Phi Khứ</strong></td></tr>';
    }
    $bd_footer .= '</table>';
    $retdata .= $bd_footer;
    $retdata .= $footer;
    $form['bangdiem'] = array('#markup'=>$retdata);

    return $form;
}

function qldiem_inbangdiem($form, &$form_state, $mahv) {
    global $user, $base_url;
    if(!user_access('Xem diem')) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $q = db_select('uit_thongtinhv', 't')->fields('t');
    $q->condition('mahv', $mahv);
    $hv = $q->execute()->fetch();
    if(!$hv) return "Không tìm thấy thông tin sinh viên này.";

    $q = db_select('uit_diem', 'd');
    $q->leftJoin('uit_ctdaotao','ctdt','d.mamh=ctdt.mamh');
    $q->leftJoin('uit_monhoc','mh','d.mamh=mh.mamh');
    $q->fields('d');
    $q->fields('ctdt',array('id'));
//    $q->fields('ctdt');
    $q->fields('mh');
    $q->condition('d.mahv', $mahv);
    $q->condition('d.trangthai', array(1,4));
    $q->orderBy('d.id','ASC');
    $q->orderBy('d.sotc','ASC');
//    $q->orderBy('d.hocky','ASC');
//    $q->orderBy('d.mamh','ASC');
    $rows = $q->execute()->fetchAll();
    if(!$rows) {
        echo 'Khong co thong tin diem.';
        die;
    }
    $ctdt = db_select('uit_ctdaotao','c')->fields('c',array('mamh','sotc'))
            ->condition('hedt', $hv->hedt)->condition('nganh', $hv->chuyennganh)
            ->execute()->fetchAllKeyed();
    $lam_lv=false;
    $arrDiem = array();
    foreach ($rows as $row) {
        $arrDiem[$row->mamh] = array(
            'tenmh' => $row->tenmh,
            'sotc' => $row->sotc,
            'diem' => ($row->trangthai==4)?'Miễn':$row->diem,
            'trangthai' => $row->trangthai,
        );
        if($row->sotc==15){
            $lam_lv=true;
        }
    }
    $hoten = $user->name;
    $gioitinh = ($hv->gioitinh == 1 ? "Nam":"Nữ");
    $ngaysinh = (strlen($hv->ngaysinh) == 10 ? date("d-m-Y",strtotime($hv->ngaysinh)):$hv->ngaysinh);
    $chuyennganh=db_select('uit_nganh','ng')->fields('ng')->condition('nganh',$hv->chuyennganh)->execute()->fetchAll();
    $style_css = '<style type="text/css" media="screen">
        #print-footer {
            display: none;
        }
        #bd_header {
            display: none;
        }
        td, th {
            font-size: 10pt;
        }
        </style>
        <style type="text/css" media="print">
        #print-footer {
            width: 100%;
            display: block;
            position: fixed;
            bottom: 0;
            text-align: center;
            margin: 0px 0 10px 0;
        }
        #print-footer hr {
            margin: 0 0 10px 0;
        }
        td, th {
            font-size: 10pt;
        }
        </style>';
    $header_first = '<div id="bd_header" class="header">
		<div class="logo">
			<img width="80" alt="logo" src="'.$base_url.'/'.drupal_get_path('module', 'qldiem').'/images/logo.png">
		</div>
		<div class="logo-char">
			<p class="char-1">ĐẠI HỌC QUỐC GIA THÀNH PHỐ HỒ CHÍ MINH</p>
			<p class="char-1"><strong>TRƯỜNG ĐẠI HỌC CÔNG NGHỆ THÔNG TIN</strong></p>
			<hr>
		</div>
		<div class="logo-char divright">
			<p class="char-1"><strong>CỘNG HOÀ XÃ HỘI CHỦ NGHĨA VIỆT NAM</strong></p>
			<p class="char-1"><strong>Độc lập - Tự do - Hạnh phúc</strong></p>
			<hr>
		</div>
	</div>';
    $header_second = '<div id="bd_header" class="header">
		<div class="logo">
			<img width="80" alt="logo" src="'.$base_url.'/'.drupal_get_path('module', 'qldiem').'/images/logo.png">
		</div>
		<div class="logo-char">
			<p class="char-1">ĐẠI HỌC QUỐC GIA THÀNH PHỐ HỒ CHÍ MINH</p>
			<p class="char-1"><strong>TRƯỜNG ĐẠI HỌC CÔNG NGHỆ THÔNG TIN</strong></p>
			<hr>
		</div>
	</div>';
    $footer = '<div id="print-footer">
		<hr>
		Địa chỉ: KM 20 Xa lộ Hà Nội, Phường Linh Trung, Quận Thủ Đức<br>
		Điện thoại: (08) 372 52002 &nbsp;&nbsp;&nbsp;&nbsp; Fax: (08) 372 52148 &nbsp;&nbsp;&nbsp;&nbsp; Website: <a href="http://www.uit.edu.vn/">http://www.uit.edu.vn</a>
	</div>';
    $retdata = $style_css.$header_first;
//    $trinhdo=($hv->hedt='CH')?'THẠC SĨ':'NGHIÊN CỨU SINH';
    $chuongtrinh_daotao=($lam_lv==true)?'Giảng dạy môn học có luận văn':'Giảng dạy môn học không luận văn';
    $gioitinh=($hv->gioitinh=0)?'Nữ':'Nam';
    $retdata .= '<div id="bd_title_h2">BẢNG ĐIỂM CAO HỌC</div>';
    $retdata .= '<table cellpadding="0" cellspacing="0" border="0" bordercolor="FFFFFF" width="100%" >';
    $retdata .= '<tr><td>Họ và tên: <strong>&nbsp;'.$hv->hoten.'</strong></td><td>Giới tính : <strong>'.$gioitinh.'</strong></td></tr>';
    $retdata .= '<tr><td nowrap>Ngày sinh: <strong>&nbsp;'.$ngaysinh.'</strong></td><td>Nơi sinh: <strong></strong></td></tr>';
    $retdata .= '<tr><td>MSHV :<strong>&nbsp;'.$hv->mahv.' </strong></td><td>Khóa:&nbsp;<strong>'.$hv->khoahoc.'</strong></td></tr>';
    $retdata .= '<tr><td>Chương trình đào tạo :&nbsp;<strong>'.$chuongtrinh_daotao.'</strong></td><td></td></tr>';
    $retdata .= '<tr><td>Chuyên ngành:&nbsp;<strong>'.$chuyennganh['0']->tennganh.'</strong></td><td></td></tr>';
    $retdata .= '</table>';

    $tblheader = '<table cellpadding="2" cellspacings="0" border="1" bordercolor="#000000"  width="100%" >';
    $tblheader .= '<tr>
            <th width="5%" style="text-align:center; padding-right:0">&nbsp;Số TT</th>
            <th width="50%" style="text-align:center; padding-right:0">&nbsp;Môn học</th>
            <th width="20%" style="text-align:center; padding-right:0">&nbsp;Mã số</th>
            <th width="10%" nowrap style="text-align:center; padding-right:0">&nbsp;Số tín chỉ</th>
            <th width="15%" nowrap style="text-align:center; padding-right:0">&nbsp;Điểm môn học</th>';
    $retdata .= $tblheader;

    $tongtc = 0;
    $tongtctl = 0;
    $tongdiemtc = 0;
    $tongtc_mien=0;
    $n=1;
    foreach ($arrDiem as $mamh => $row) {
        $styletr = "";
        if($row['trangthai'] > 1) {
            $styletr = ' style="background-color:#ffbfdf;"';
        }
        $retdata .= "<tr".$styletr.">
            <td align='center'>".$n++."</td>
            <td >".$row['tenmh']."</td>
            <td align='center'>&nbsp;".$mamh."</td>
            <td align='center'>".($row['sotc']>0 ? $row['sotc']:"")."</td>
            <td align='center'>".($row['diem'])."</td>
            </tr>";
        if($n == 26 || ($n - 26)%29 == 0) {
            $retdata .= '</table>';
            $retdata .= $footer;
            $retdata .= '<p style="page-break-after: always;"></p>';
            $retdata .= $header_second;
            $retdata .= '<div id="bd_title_h2"></div>';
            $retdata .= $tblheader;
        }
//        if($row['sotc'] > 0 && $row['trangthai'] == 1) {
        if($row['sotc'] > 0 ) {
            if($row['trangthai']==4){
                $tongtc_mien += $row['sotc'];
            }else{
                $tongtc += $row['sotc'];
                $tongdiemtc += $row['diem']*$row['sotc'];
            }
            if($row['diem'] >= 5 || $row['trangthai']==4) {
                $tongtctl += $row['sotc'];
            }
        }
    }
    if($tongtc > 0) {
        $dtb=round($tongdiemtc/$tongtc,2);
        $retdata .= "<tr><td colspan=3 align='right'><strong>&nbsp;&nbsp;Tổng số tín chỉ</strong></td><td align='center'><strong>".$tongtctl."</strong></td><td>&nbsp;</td></tr>";
        $retdata .= "<tr><td colspan=4 align='right'><strong>&nbsp;&nbsp;Điểm trung bình chung các môn học</strong></td><td align='center'><strong>".$dtb."</strong></td></tr>";
    }
    $retdata .= '</table>';
    $bd_footer = '<table width="100%">';
    if($lam_lv){
        $bd_footer .= '<tr><td width="50%" >Đề tài luận văn&nbsp;</td><td width="50%" align="center"><i>TP.HCM, ngày '.date('d').' tháng '.date('m').' năm '.date('Y').'</i></td></tr>';
        $bd_footer .= '<tr><td width="50%">Hội đồng chấm luận văn gồm:&nbsp;</td><td width="50%" align="center"><strong>TL.HIỆU TRƯỞNG</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1. Chủ tịch:</td><td width="50%" align="center"><strong>TRƯỞNG PHÒNG ĐTSĐH–KHCN&QHĐN</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. Thư ký:</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3. Phản biện 1:</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4. Phản biện 2:</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5. Ủy viên:</td><td width="50%" align="center"><strong>Nguyễn Phi Khứ</strong></td></tr>';
    }else{
        $bd_footer .= '<tr><td width="50%" >&nbsp;</td><td width="50%" align="center"><i>TP.HCM, ngày '.date('d').' tháng '.date('m').' năm '.date('Y').'</i></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;</td><td width="50%" align="center"><strong>TL.HIỆU TRƯỞNG</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"><strong>TRƯỞNG PHÒNG ĐTSĐH–KHCN&QHĐN</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"><strong>Nguyễn Phi Khứ</strong></td></tr>';
    }
    $bd_footer .= '</table>';
    $retdata .= $bd_footer;
    $retdata .= $footer;
    $form['bangdiem'] = array('#markup'=>$retdata);

    return $form;
}

function qldiem_inbangdiemtn($form, &$form_state, $mahv){
    global $user, $base_url;
    if(!user_access('Xem diem')) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $q = db_select('uit_thongtinhv', 't')->fields('t');
    $q->condition('mahv', $mahv);
    $hv = $q->execute()->fetch();
    if(!$hv) return "Không tìm thấy thông tin sinh viên này.";

    $q = db_select('uit_diem', 'd');
    $q->leftJoin('uit_ctdaotao', 'ctdt', "d.mamh = ctdt.mamh");
    $q->leftJoin('uit_monhoc','mh','d.mamh=mh.mamh');
    $q->fields('d')->fields('ctdt',array('id'))->fields('mh');
    $q->condition('d.mahv', $mahv);
//    $q->condition('d.diem', 5, '>=');
    $q->condition('d.trangthai', array(1,4));
    $q->orderBy('d.id','ASC');
    $q->orderBy('d.sotc','ASC');
    $q->orderBy('d.hocky','ASC');
    $q->orderBy('d.mamh','ASC');
    $rows = $q->execute()->fetchAll();
    if(!$rows) {
        echo 'Khong co thong tin diem.';
        die;
    }
    $ctdt = db_select('uit_ctdaotao','c')->fields('c',array('mamh','sotc'))
        ->condition('hedt', $hv->hedt)->condition('nganh', $hv->chuyennganh)
        ->execute()->fetchAllKeyed();

    $arrDiem = array();
    $lam_lv=false;
    foreach ($rows as $row) {
        if($row->diem<5 && $row->diem!=0){
            continue;
        }
        $arrDiem[$row->mamh] = array(
            'tenmh' => $row->tenmh,
            'sotc' => $row->sotc,
            'diem' => ($row->trangthai==4)?'Miễn':$row->diem,
            'trangthai' => $row->trangthai,
            'thoigianketthuc' => $row->thoigianketthuc,
        );
        if($row->sotc==15) $lam_lv=true;
    }
    $hoten = $user->name;
    $gioitinh = ($hv->gioitinh == 1 ? "Nam":"Nữ");
    $ngaysinh = (strlen($hv->ngaysinh) == 10 ? date("d-m-Y",strtotime($hv->ngaysinh)):$hv->ngaysinh);
    $chuyennganh=db_select('uit_nganh','ng')->fields('ng')->condition('nganh',$hv->chuyennganh)->execute()->fetchAll();
    $style_css = '<style type="text/css" media="screen">
        #print-footer {
            display: none;
        }
        #bd_header {
            display: none;
        }
        td, th {
            font-size: 10pt;
        }
        </style>
        <style type="text/css" media="print">
        #print-footer {
            width: 100%;
            display: block;
            position: fixed;
            bottom: 0;
            text-align: center;
            margin: 0px 0 10px 0;
        }
        #print-footer hr {
            margin: 0 0 10px 0;
        }
        td, th {
            font-size: 10pt;
        }
        </style>';
    $header_first = '<div id="bd_header" class="header">
		<div class="logo">
			<img width="80" alt="logo" src="'.$base_url.'/'.drupal_get_path('module', 'qldiem').'/images/logo.png">
		</div>
		<div class="logo-char">
			<p class="char-1">ĐẠI HỌC QUỐC GIA THÀNH PHỐ HỒ CHÍ MINH</p>
			<p class="char-1"><strong>TRƯỜNG ĐẠI HỌC CÔNG NGHỆ THÔNG TIN</strong></p>
			<hr>
		</div>
		<div class="logo-char divright">
			<p class="char-1"><strong>CỘNG HOÀ XÃ HỘI CHỦ NGHĨA VIỆT NAM</strong></p>
			<p class="char-1"><strong>Độc lập - Tự do - Hạnh phúc</strong></p>
			<hr>
		</div>
	</div>';
    $header_second = '<div id="bd_header" class="header">
		<div class="logo">
			<img width="80" alt="logo" src="'.$base_url.'/'.drupal_get_path('module', 'qldiem').'/images/logo.png">
		</div>
		<div class="logo-char">
			<p class="char-1">ĐẠI HỌC QUỐC GIA THÀNH PHỐ HỒ CHÍ MINH</p>
			<p class="char-1"><strong>TRƯỜNG ĐẠI HỌC CÔNG NGHỆ THÔNG TIN</strong></p>
			<hr>
		</div>
	</div>';
    $footer = '<div id="print-footer">
		<hr>
		Địa chỉ: KM 20 Xa lộ Hà Nội, Phường Linh Trung, Quận Thủ Đức<br>
		Điện thoại: (08) 372 52002 &nbsp;&nbsp;&nbsp;&nbsp; Fax: (08) 372 52148 &nbsp;&nbsp;&nbsp;&nbsp; Website: <a href="http://www.uit.edu.vn/">http://www.uit.edu.vn</a>
	</div>';
    $retdata = $style_css.$header_first;
    $trinhdo=($hv->hedt='CH')?'THẠC SĨ':'NGHIÊN CỨU SINH';
    $chuongtrinh_daotao=($lam_lv==true)?'Giảng dạy môn học có luận văn':'Giảng dạy môn học không luận văn';
    $retdata .= '<div id="bd_title_h2">BẢNG ĐIỂM ĐÀO TẠO TRÌNH ĐỘ '.$trinhdo.'</div>';
    $retdata .= '<table cellpadding="0" cellspacing="0" border="0" bordercolor="FFFFFF" width="100%" >';
    $retdata .= '<tr><td>Cấp cho ông (bà): <strong>&nbsp;'.$hv->hoten.'</strong></td><td></td></tr>';
    $retdata .= '<tr><td nowrap>Sinh ngày: <strong>&nbsp;'.$ngaysinh.'</strong></td><td></td></tr>';
    $retdata .= '<tr><td>Là học viên chương trình đào tạo '.drupal_strtolower($trinhdo).' khóa:&nbsp;<strong>'.$hv->khoahoc.'</strong></td><td></td><td>&nbsp;</td><td>&nbsp;</td></tr>';
    $retdata .= '<tr><td>Loại chương trình đào tạo :&nbsp;<strong>'.$chuongtrinh_daotao.'</strong></td><td></td></tr>';
    $retdata .= '<tr><td>Chuyên ngành:&nbsp;<strong>'.$chuyennganh['0']->tennganh.'</strong></td><td></td></tr>';
    $retdata .= '</table>';

    $tblheader = '<table cellpadding="2" cellspacings="0" border="1" bordercolor="#000000"  width="100%" >';
    $tblheader .= '<tr>
            <th width="5%" style="text-align:center; padding-right:0">&nbsp;Số TT</th>
            <th width="50%" style="text-align:center; padding-right:0">&nbsp;Môn học</th>
            <th width="20%" style="text-align:center; padding-right:0">&nbsp;Mã số</th>
            <th width="10%" nowrap style="text-align:center; padding-right:0">&nbsp;Số tín chỉ</th>
            <th width="15%" nowrap style="text-align:center; padding-right:0">&nbsp;Điểm môn học</th>';
    $retdata .= $tblheader;

    $tongtc = 0;
    $tongtctl = 0;
    $tongdiemtc = 0;
    $tongtc_mien=0;
    $n=1;
    foreach ($arrDiem as $mamh => $row) {
        $styletr = "";
        if($row['trangthai'] > 1) {
            $styletr = ' style="background-color:#ffbfdf;"';
        }
        $retdata .= "<tr".$styletr.">
            <td align='center'>".$n++."</td>
            <td >".$row['tenmh']."</td>
            <td align='center'>&nbsp;".$mamh."</td>
            <td align='center'>".($row['sotc']>0 ? $row['sotc']:"")."</td>
            <td align='center'>".($row['diem'])."</td>
            </tr>";
        if($n == 26 || ($n - 26)%29 == 0) {
            $retdata .= '</table>';
            $retdata .= $footer;
            $retdata .= '<p style="page-break-after: always;"></p>';
            $retdata .= $header_second;
            $retdata .= '<div id="bd_title_h2"></div>';
            $retdata .= $tblheader;
        }
//        if($row['sotc'] > 0 && $row['trangthai'] == 1) {
        if($row['sotc'] > 0 ) {
            if($row['trangthai']==4){
                $tongtc_mien += $row['sotc'];
            }else{
                $tongtc += $row['sotc'];
                $tongdiemtc += $row['diem']*$row['sotc'];
            }
            if($row['diem'] >= 5 || $row['trangthai']==4) {
                $tongtctl += $row['sotc'];
            }
        }
    }
    if($tongtctl > 0) {
        $dtb=round($tongdiemtc/$tongtc,2);
        $tongtc+=$tongtc_mien;
//        $retdata .= "<tr><td colspan=3><strong>&nbsp;&nbsp;Số tín chỉ đã học</strong></td><td align='center'><strong>".$tongtc."</strong></td><td>&nbsp;</td><td>&nbsp;</td></tr>";
        $retdata .= "<tr><td colspan=3 align='right'><strong>&nbsp;&nbsp;Tổng số tín chỉ</strong></td><td align='center'><strong>".$tongtctl."</strong></td><td>&nbsp;</td></tr>";
        $retdata .= "<tr><td colspan=4 align='right'><strong>&nbsp;&nbsp;Điểm trung bình chung các môn học</strong></td><td align='center'><strong>".$dtb."</strong></td></tr>";
//        $retdata .= '<tr><td colspan=2 style="border: 0px none ! important;">&nbsp;</td><td style="border: 0px none ! important;"><strong>&nbsp;&nbsp;Xếp loại tốt nghiệp</strong></td><td colspan=3 style="border: 0px none ! important;"><strong>'.  Xeploai($dtb)."</strong></td></tr>";
    }
    $retdata .= '</table>';
    $bd_footer = '<table width="100%">';
    if($lam_lv){
        $bd_footer .= '<tr><td width="50%" >Đề tài luận văn&nbsp;</td><td width="50%" align="center"><i>TP.HCM, ngày '.date('d').' tháng '.date('m').' năm '.date('Y').'</i></td></tr>';
        $bd_footer .= '<tr><td width="50%">Hội đồng chấm luận văn gồm:&nbsp;</td><td width="50%" align="center"><strong>TL.HIỆU TRƯỞNG</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1. Chủ tịch:</td><td width="50%" align="center"><strong>TRƯỞNG PHÒNG ĐTSĐH–KHCN&QHĐN</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. Thư ký:</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3. Phản biện 1:</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4. Phản biện 2:</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5. Ủy viên:</td><td width="50%" align="center"><strong>Nguyễn Phi Khứ</strong></td></tr>';
    }else{
        $bd_footer .= '<tr><td width="50%" >&nbsp;</td><td width="50%" align="center"><i>TP.HCM, ngày '.date('d').' tháng '.date('m').' năm '.date('Y').'</i></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;</td><td width="50%" align="center"><strong>TL.HIỆU TRƯỞNG</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"><strong>TRƯỞNG PHÒNG ĐTSĐH–KHCN&QHĐN</strong></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"></td></tr>';
        $bd_footer .= '<tr><td width="50%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td width="50%" align="center"><strong>Nguyễn Phi Khứ</strong></td></tr>';
    }
    $bd_footer .= '</table>';
    $retdata .= $bd_footer;
    $retdata .= $footer;
    $form['bangdiem'] = array('#markup'=>$retdata);

    return $form;
}

//function qldiem_inbangdiemtn_backup($form, &$form_state, $mahv) {
//    global $user, $base_url;
//    if(!user_access('Xem diem')) {
//        drupal_set_message('Bạn không được phép truy cập menu này','error');
//        return;
//    }
//    $q = db_select('uit_thongtinhv', 't');
////    $q->leftJoin('uit_coso','cs','cs.macs = t.coso');
////    $q->fields('t')->fields('cs',array('tencs'));
//    $q->condition('t.mahv', $mahv);
//    $hv = $q->execute()->fetch();
//    if(!$hv) return "Không tìm thấy thông tin sinh viên này.";
//
//    $q = db_select('uit_diem', 'd');
////    $q->leftJoin('uit_ctdaotao', 'mh', "d.mamh = mh.mamh and mh.nganh = '$hv->chuyennganh'");
//    $q->leftJoin('uit_ctdaotao', 'ctdt', "d.mamh = ctdt.mamh");
//    $q->leftJoin('uit_monhoc','mh','d.mamh=mh.mamh');
//    $q->fields('d')->fields('ctdt')->fields('mh');
//    $q->condition('d.mahv', $mahv);
//    $q->condition('d.diem', 5, '>=');
//    $q->condition('d.trangthai', 1);
//    $q->orderBy('d.hocky','ASC')->orderBy('d.mamh','ASC');
//    $rows = $q->execute()->fetchAll();
//    if(!$rows) {
//        echo 'Khong co thong tin diem.';
//        die;
//    }
//    $ctdt = db_select('uit_ctdaotao','c')->fields('c',array('mamh','sotc'))
//            ->condition('hedt', $hv->hedt)->condition('nganh', $hv->chuyennganh)
//            ->execute()->fetchAllKeyed();
//
//    $arrDiem = array();
//    foreach ($rows as $row) {
//        $arrDiem[$row->mamh] = array(
//            'tenmh' => $row->tenmh,
//            'sotc' => $row->sotc,
//            'diem' => $row->diem,
//            'trangthai' => $row->trangthai,
//            'thoigianketthuc' => $row->thoigianketthuc,
//        );
//    }
//
////    $hoten = db_select('field_data_field_hoten', 'f')->fields('f', array('field_hoten_value'))->condition('f.entity_id', $user->uid)->execute()->fetchField();
////    if(!$hoten) $hoten = $user->name;
//    $hoten = $user->name;
//    $gioitinh = ($hv->gioitinh == 1 ? "Nam":"Nữ");
//    $ngaysinh = (strlen($hv->ngaysinh) == 10 ? date("d-m-Y",strtotime($hv->ngaysinh)):$hv->ngaysinh);
//
//    $style_css = '<style type="text/css" media="screen">
//        #print-footer {
//            display: none;
//        }
//        #bd_header {
//            display: none;
//        }
//        td, th {
//            font-size: 10pt;
//        }
//        </style>
//        <style type="text/css" media="print">
//        #print-footer {
//            width: 100%;
//            display: none;
//            position: fixed;
//            bottom: 0;
//            text-align: center;
//            margin: 0px 0 10px 0;
//        }
//        #print-footer hr {
//            margin: 0 0 10px 0;
//        }
//        td, th {
//            font-size: 10pt;
//        }
//        </style>';
//    $header_first = '<div id="bd_header" class="header">
//		<div class="logo">
//			<img width="80" alt="logo" src="'.$base_url.'/'.drupal_get_path('module', 'qldiem').'/images/logo.png">
//		</div>
//		<div class="logo-char">
//			<p class="char-1">ĐẠI HỌC QUỐC GIA THÀNH PHỐ HỒ CHÍ MINH</p>
//			<p class="char-1"><strong>TRƯỜNG ĐẠI HỌC CÔNG NGHỆ THÔNG TIN</strong></p>
//			<hr>
//		</div>
//		<div class="logo-char divright">
//			<p class="char-1"><strong>CỘNG HOÀ XÃ HỘI CHỦ NGHĨA VIỆT NAM</strong></p>
//			<p class="char-1"><strong>Độc lập - Tự do - Hạnh phúc</strong></p>
//			<hr>
//		</div>
//	</div>';
//    $header_second = '<div id="bd_header" class="header">
//		<div class="logo">
//			<img width="80" alt="logo" src="'.$base_url.'/'.drupal_get_path('module', 'qldiem').'/images/logo.png">
//		</div>
//		<div class="logo-char">
//			<p class="char-1">ĐẠI HỌC QUỐC GIA THÀNH PHỐ HỒ CHÍ MINH</p>
//			<p class="char-1"><strong>TRƯỜNG ĐẠI HỌC CÔNG NGHỆ THÔNG TIN</strong></p>
//			<hr>
//		</div>
//	</div>';
//    $footer = '<div id="print-footer">
//		<hr>
//		Địa chỉ: KM 20 Xa lộ Hà Nội, Phường Linh Trung, Quận Thủ Đức<br>
//		Điện thoại: (08) 372 52002 &nbsp;&nbsp;&nbsp;&nbsp; Fax: (08) 372 52148 &nbsp;&nbsp;&nbsp;&nbsp; Website: <a href="http://www.uit.edu.vn/">http://www.uit.edu.vn</a>
//	</div>';
//    $retdata = $style_css.$header_first;
//
//    $retdata .= '<div id="bd_title_h2">BẢNG ĐIỂM TỐT NGHIỆP</div>';
//
//    $retdata .= '<table cellpadding="0" cellspacing="0" border="0" bordercolor="FFFFFF" width="100%" >';
//    $retdata .= '<tr><td>Mã hv:</td><td><strong>'.$hv->mahv.'</strong></td><td>Họ và tên:</td><td><strong>'.$hv->hoten.'</strong></td></tr>';
//    $retdata .= '<tr><td nowrap>Ngày sinh:</td><td><strong>'.$ngaysinh.'</strong></td><td>Nơi sinh:</td><td><strong>'.$hv->noisinh.'</strong></td></tr>';
//    $retdata .= '<tr><td>Khóa học:</td><td><strong>'.$hv->khoahoc.'</strong></td><td>Giới tính:</td><td><strong>'.$gioitinh.'</strong></td></tr>';
//    $retdata .= '<tr><td>Ngành học:</td><td><strong>Công nghệ Thông tin</strong></td><td>Hệ đào tạo:</td><td><strong>Từ xa Qua mạng</strong></td></tr>';
//    $retdata .= '</table>';
//
//    $tblheader = '<table cellpadding="2" cellspacing="0" border="1" bordercolor="#000000"  width="100%" >';
//    $tblheader .= '<tr><th width="5%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</th><th width="15%" style="text-align:center; padding-right:0">Mã MH</th><th width="50%">&nbsp;Tên môn học</th><th width="10%" nowrap style="text-align:center; padding-right:0">&nbsp;Tín chỉ</th>
//        <th width="10%" nowrap style="text-align:center; padding-right:0">&nbsp;Điểm số</th><th width="10%" nowrap style="text-align:center; padding-right:0">&nbsp;Ghi chú</th></tr>';
//    $retdata .= $tblheader;
//
//    $tongtc = 0;
//    $tongdiemtc = 0;
//    $n=1;
//    foreach ($arrDiem as $mamh => $row) {
////        if(!isset($ctdt[$mamh])) {
////            $mamhtd = MonHocTuongDuong($mamh, $hv->chuyennganh);
////            if(!$mamhtd) continue;
////            $cont = TRUE;
////            foreach ($mamhtd as $mamh2=>$value) {
////                if(isset($ctdt[$mamh2])) {
////                    if(isset($arrDiem[$mamh2])) {
////                        break;
////                    }
////                    $data = db_select('uit_ctdaotao','c')->fields('c',array('tenmh','sotc'))
////                            ->condition('hedt', $hv->hedt)->condition('nganh', $hv->chuyennganh)
////                            ->condition('mamh', $mamh2)
////                            ->execute()->fetch();
////                    if($data) {
////                        $mamh = $mamh2;
////                        $row['tenmh'] = $data->tenmh;
////                        $row['sotc'] = $data->sotc;
////                        $cont = FALSE;
////                        break;
////                    }
////                }
////            }
////            if($cont) continue;
////        }
////
//        $styletr = "";
//        if($row['trangthai'] > 1) {
//            $styletr = ' style="background-color:#ffbfdf;"';
//        }
//        $retdata .= "<tr".$styletr."><td align='center'>".$n++."</td><td align='center'>".$mamh."</td><td>&nbsp;".$row['tenmh']."</td><td align='center'>".($row['sotc']>0 ? $row['sotc']:"")."</td>
//            <td align='center'>".($row['diem'])."</td><td>&nbsp;".$row['thoigianketthuc']."</td></tr>";
//        if($n == 26 || ($n - 26)%29 == 0) {
//            $retdata .= '</table>';
//            $retdata .= $footer;
//            $retdata .= '<p style="page-break-after: always;"></p>';
//            $retdata .= $header_second;
//            $retdata .= '<div id="bd_title_h2"></div>';
//            $retdata .= $tblheader;
//        }
//        if($row['sotc'] > 0 && $row['trangthai'] == 1) {
//            $tongtc += $row['sotc'];
//            $tongdiemtc += $row['diem']*$row['sotc'];
//        }
//    }
//    if($tongtc > 0) {
//        $dtb = round($tongdiemtc/$tongtc,2);
//        $retdata .= '<tr><td colspan=2 style="border: 0px none ! important;">&nbsp;</td><td style="border: 0px none ! important;"><strong>&nbsp;&nbsp;Số tín chỉ tích lũy</strong></td><td colspan=3 style="border: 0px none ! important;"><strong>'.$tongtc."</strong></td></tr>";
//        $retdata .= '<tr><td colspan=2 style="border: 0px none ! important;">&nbsp;</td><td style="border: 0px none ! important;"><strong>&nbsp;&nbsp;ĐTB tích lũy toàn khóa</strong></td><td colspan=3 style="border: 0px none ! important;"><strong>'.$dtb."</strong></td></tr>";
//        $retdata .= '<tr><td colspan=2 style="border: 0px none ! important;">&nbsp;</td><td style="border: 0px none ! important;"><strong>&nbsp;&nbsp;Xếp loại tốt nghiệp</strong></td><td colspan=3 style="border: 0px none ! important;"><strong>'.  Xeploai($dtb)."</strong></td></tr>";
//    }
//    $retdata .= '</table>';
//
//    $bd_footer = '<table width="100%">';
//    $bd_footer .= '<tr><td width="50%" align="center">&nbsp;</td><td width="50%" align="center"><i>TP.Hồ Chí Minh, ngày '.date('d').' tháng '.date('m').' năm '.date('Y').'</i></td></tr>';
//    $bd_footer .= '<tr><td width="50%">&nbsp;</td><td width="50%" align="center"><strong>TL.HIỆU TRƯỞNG</strong></td></tr>';
//    $bd_footer .= '<tr><td width="50%" align="center">Người lập bảng điểm</td><td width="50%" align="center"><strong>KT. TRƯỞNG PHÒNG ĐÀO TẠO ĐẠI HỌC</strong></td></tr>';
//    $bd_footer .= '<tr><td width="50%" align="center">&nbsp;</td><td width="50%" align="center"><strong>PHÓ TRƯỞNG PHÒNG</strong></td></tr>';
//    $bd_footer .= '<tr><td width="50%" height="80">&nbsp;</td><td width="50%">&nbsp;</td></tr>';
//    $bd_footer .= '<tr><td width="50%" align="center"><strong>'.$hoten.'<strong></td><td width="50%" align="center"><strong>Đinh Khắc Quyền</strong></td></tr>';
//    $bd_footer .= '</table>';
//    $retdata .= $bd_footer;
//    //$retdata .= $footer;
//    $form['bangdiem'] = array('#markup'=>$retdata);
//
//    return $form;
//}