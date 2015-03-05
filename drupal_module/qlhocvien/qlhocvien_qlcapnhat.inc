<?php
function qlcapnhat_form($form, &$form_state) {
    if(!user_access('UIT qlhocvien admin')) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $form = array();
    $html = '<center><h1>QUẢN LÝ CẬP NHẬT THÔNG TIN HỌC VIÊN</h1></center>';
    
    $form['markup'] = array('#markup' => $html);
    $form['qlcapnhat_filter_wraper'] = array('#type' => 'fieldset', '#title' => t('Chọn điều kiện lọc'), '#attributes' => array('class' => array('container-inline')));
    qlcapnhat_set_filters($form_state);
    $form['qlcapnhat_filter_wraper'][] = qlcapnhat_filter_form();
    
    $headers = array(
        array('data' => 'Mã HV', 'field' => 'mahv'),
        array('data' => 'Họ tên', 'field' => 'hoten'),
        array('data' => 'HK', 'field' => 'hocky'),
        array('data' => 'Năm học', 'field' => 'namhoc'),
        array('data' => 'Tình Trạng', 'field' => 'ten'),
        array('data' => 'Số QĐ', 'field' => 'soqd'),
        array('data' => 'Ngày QĐ', 'field' => 'ngayqd'),
        array('data' => 'Ghi chú', 'field' => 'ghichu'),
        array('data' => 'Giá trị cũ', 'field' => 'giatricu'),
        array('data' => 'Giá trị mới', 'field' => 'giatrimoi'),
    );
    $query = db_select('uit_lichsu_tinhtrang_hocvien','t')->extend('PagerDefault')->limit($_SESSION['qlcapnhat_filter']['sodong'])->extend('TableSort');
    $query->join('uit_thongtinhv', 'sv', 'sv.mahv = t.mahv');
    $query->leftJoin('uit_tinhtrang', 'tt', "tt.id = t.tinhtrang and tt.doituong='SV'");
    $query->fields('t');
    $query->fields('tt', array('ten'));
    $query->fields('sv', array('hoten'));
    $query->orderBy('t.ngayqd','DESC');
    $query->orderBy('t.soqd','DESC');
    $query->orderBy('t.mahv','ASC');
    qlcapnhat_build_filter_query( $query);
    $data = $query->execute();
    
    $rows = array();
    //$i=1;
    foreach ($data as $row) {
        $rows[$row->id.';'.$row->mahv] = array(
                            $row->mahv,
                            $row->hoten,
                            $row->hocky,
                            $row->namhoc,
                            $row->ten,
                            $row->soqd,
                            date('d-m-Y',  strtotime($row->ngayqd)),
                            $row->ghichu,
                            $row->giatricu,
                            $row->giatrimoi,
                            );
    }
    
    $form['qlcapnhat_wraper'] = array(
        '#prefix' => '<div id="table_qlcapnhat">',
        '#suffix' => '</div>',);
    $form['qlcapnhat_wraper']['info'] = qlcapnhat_info_form();
    if ($_GET['q'] == 'system/ajax') {
        $q = 'admin/qlhocvien/qlcapnhat' ;
        $_GET['q'] = $q;
    }
    $form['qlcapnhat_wraper']['table_data'] = array(
        '#type' => 'tableselect',
        '#caption' => 'DANH SÁCH HỌC VIÊN CẬP NHẬT',
        '#header' => $headers,
        '#options' => $rows,
        '#empty' => 'Không tìm thấy HV nào'
    );
    $tmp_q = $_GET['q']; 
    $_GET['q'] = 'admin/qlhocvien/qlcapnhat';
    $form['qlcapnhat_wraper']['pager'] = array('#markup' => theme('pager'));
    $_GET['q'] = $tmp_q;
    
    $form['xuat'] = array( '#type' => 'button',
                            '#value' => t('Export'),
                            '#attributes' => array('title' =>'Export danh sách cập nhật ra file excel'),
                            '#executes_submit_callback' => TRUE,
                            '#submit' => array('qlcapnhat_xuat'),
                           );
    $form['xoa'] = array( '#type' => 'button',
                             '#value' => t('Xóa'),
                             '#attributes' => array('onclick'=>'return confirm(\'Bạn có chắc xóa các dòng được chọn ?\');','title' =>'Xóa các dòng được chọn'),
                             '#executes_submit_callback' => TRUE,
                             '#submit' => array('qlcapnhat_xoa'),
                            );
    
    return $form;
}

function qlcapnhat_info_form() {
    $query = db_select('uit_lichsu_tinhtrang_hocvien','t');
    $query->join('uit_thongtinhv', 'sv', 'sv.mahv = t.mahv');
    $query->addExpression('count(t.id)', 'tong');
    qlcapnhat_build_filter_query( $query);
    $tong = $query->execute()->fetchField();
    $form['info'] = array(array('#markup' =>'<strong>Tổng cộng: '.number_format($tong).'</strong>'));
    return $form;
}

function qlcapnhat_xuat($form, &$form_state) {
    $query = db_select('uit_lichsu_tinhtrang_hocvien','t');
    $query->join('uit_thongtinhv', 'sv', 'sv.mahv = t.mahv');
    $query->leftJoin('uit_tinhtrang', 'tt', "tt.id = t.tinhtrang and tt.doituong='SV'");
    $query->fields('t');
    $query->fields('tt', array('ten'));
    $query->fields('sv', array('hoten'));
    $query->orderBy('t.ngayqd','DESC');
    $query->orderBy('t.soqd','DESC');
    $query->orderBy('t.mahv','ASC');
    qlcapnhat_build_filter_query( $query);
    $rows = $query->execute()->fetchAll();
    if(!$rows) {
        drupal_set_message('Không có thông tin để xuất');
        return;
    }
    
    $filename = 'DSCapNhat_'.date('dmhis').'.xlsx';
    $path = variable_get('file_public_path', conf_path() . '/files') . '/exports';
    if(!file_exists($path)) mkdir($path);
    $path .= '/'.$filename;
    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    $objPHPExcel = new PHPExcel();
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    
    $c = 'A';
    $r = 1;
    $ActiveSheet->setCellValue($c++.$r, 'STT');
    $ActiveSheet->setCellValue($c++.$r, 'Mã SV');
    $ActiveSheet->setCellValue($c++.$r, 'Họ tên');
    $ActiveSheet->setCellValue($c++.$r, 'Học kỳ');
    $ActiveSheet->setCellValue($c++.$r, 'Năm học');
    $ActiveSheet->setCellValue($c++.$r, 'Tình Trạng');
    $ActiveSheet->setCellValue($c++.$r, 'Số QĐ');
    $ActiveSheet->setCellValue($c++.$r, 'Ngày QĐ');
    $ActiveSheet->setCellValue($c++.$r, 'Ghi chú');
    $ActiveSheet->setCellValue($c++.$r, 'Giá trị cũ');
    $ActiveSheet->setCellValue($c++.$r, 'Giá trị mới');
    $r = 2;
    //$ActiveSheet->getStyle('H')->getNumberFormat()->setFormatCode("#,##0");
    foreach($rows as $row){
        $c = 'A';
        $ActiveSheet->setCellValue($c++.$r, $r-1);
        $ActiveSheet->setCellValueExplicit($c++.$r, $row->mahv, PHPExcel_Cell_DataType::TYPE_STRING);
        $ActiveSheet->setCellValue($c++.$r, $row->hoten);
        $ActiveSheet->setCellValue($c++.$r, $row->hocky);
        $ActiveSheet->setCellValue($c++.$r, $row->namhoc);
        $ActiveSheet->setCellValue($c++.$r, $row->ten);
        $ActiveSheet->setCellValue($c++.$r, $row->soqd);
        $ActiveSheet->setCellValue($c++.$r, $row->ngayqd);
        $ActiveSheet->setCellValue($c++.$r, $row->ghichu);
        $ActiveSheet->setCellValue($c++.$r, $row->giatricu);
        $ActiveSheet->setCellValue($c++.$r, $row->giatrimoi);
        $r++;
    }
    $objWriter = new PHPExcel_Writer_Excel2007($objPHPExcel);
    ob_end_clean();
    $objWriter->save($path);
    
    if(!file_exists($path)) {
        drupal_set_message ('Có lỗi xảy ra, không xuất được file.', 'error');
        return;
    }
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
}

function qlcapnhat_xoa($form, &$form_state) {
    if(!user_access('UIT qlhocvien admin')) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $arrData = array_filter($form_state['values']['table_data']);
    $strmahv = '';
    foreach ($arrData as $item) {
        $arr = explode(';', $item);
        $id = $arr[0];
        $mahv = $arr[1];
        if(XoaID($id)) $strmahv .= $mahv.'; ';
    }
    if($strmahv != '') drupal_set_message ('Đã xóa thành công đơn các SV: '.$strmahv);
    ghilogs('Xóa cập nhật thông tin SV', $strmahv, 'qlhocvien');
    return;
}

function XoaID($id) {
    try {
        $row = db_select('uit_lichsu_tinhtrang_hocvien','t')->fields('t')->condition('id',$id)->execute()->fetch();
        if(!$row) return FALSE;
        if(in_array($row->tinhtrang, array(4,5,6)) && $row->tinhtrang != $row->giatricu ) {
            //rollback dữ liệu DKHP nếu có
            $query = db_select('uit_dkhp_backup','t');
            $query->fields('t', array('mahv','malop','mamh','thuchanh','ngaydk','hocky','namhoc','ghichu'));
            $query->condition('mahv', $row->mahv);
            $query->condition('hocky', $row->hocky);
            $query->condition('namhoc', $row->namhoc);
            $check = $query->execute()->fetch();
            if($check) {
                db_insert('uit_dkhp')->from($query)->execute();
                //Xóa DKHP
                db_delete('uit_dkhp_backup')
                    ->condition('mahv', $row->mahv)
                    ->condition('hocky', $row->hocky)
                    ->condition('namhoc', $row->namhoc)
                    ->execute();
                //tính lại HP
                TinhHocPhiSV($row->mahv, $row->hocky, $row->namhoc);
            }
        }
        db_delete('uit_lichsu_tinhtrang_hocvien')->condition('id',$id)->execute();
        $tinhtrang = db_select('uit_lichsu_tinhtrang_hocvien','t')->fields('t',array('tinhtrang'))->condition('mahv',$row->mahv)
                ->where("(namhoc*5+hocky) > ".($row->namhoc*5+$row->hocky))->orderBy('namhoc','DESC')->orderBy('hocky','DESC')
                ->execute()->fetchField();
        if(!$tinhtrang) {
            db_update('uit_thongtinhv')->fields(array('tinhtrang' => ($row->giatricu ? $row->giatricu : 1)))->condition('mahv', $row->mahv)->execute();
        }
    } catch (Exception $exc){
        drupal_set_message('Phát sinh lỗi: '.$exc->getMessage(),'error');
        return FALSE;
    }
    return TRUE;
}


function ajax_filter_qlcapnhat_callback($form, $form_state) {
  return $form['qlcapnhat_wraper'];
}

function qlcapnhat_set_filters(&$form_state) {
  $filters = qlcapnhat_filters();
  foreach ($filters as $filter => $options) {
    if (isset($form_state['values'][$filter])) {
      if( $form_state['values'][$filter] != '') {
        $_SESSION['qlcapnhat_filter'][$filter] = $form_state['values'][$filter];
      }
      else {
        unset($_SESSION['qlcapnhat_filter'][$filter]);
      }
    } 
  }
}

function qlcapnhat_filters() {
  $filters = array();
  //unset($_SESSION['qlcapnhat_filter']);
  if(!isset($_SESSION['qlcapnhat_filter']['sodong'])) $_SESSION['qlcapnhat_filter']['sodong'] = 50;
  
  $session = isset($_SESSION['qlcapnhat_filter']) ? $_SESSION['qlcapnhat_filter'] : array();
  $mahv = !empty($session['mahv']) ? $session['mahv'] : NULL;
  $hoten = !empty($session['hoten']) ? $session['hoten'] : NULL;
  $soqd = !empty($session['soqd']) ? $session['soqd'] : NULL;
  $ngayqd = !empty($session['ngayqd']) ? $session['ngayqd'] : NULL;
  $tinhtrang = !empty($session['tinhtrang']) ? $session['tinhtrang'] : array('');
  $namhoc = !empty($session['namhoc']) ? $session['namhoc'] : array('');
  $hocky = !empty($session['hocky']) ? $session['hocky'] : array('');
  $sodong = !empty($session['sodong']) ? $session['sodong'] : 50;
  
    $filters['mahv'] = array(
        'type' => 'textfield',
        'title' => t('Mã SV'),
        'size' => 30,
        'default_value' => array($mahv),
        'attributes' => array('title' =>'Có thể nhập nhiều mahv cách nhau bởi dấu phẩy (,).'),
    );
    $filters['hoten'] = array(
        'type' => 'textfield',
        'title' => t('Họ tên'),
        'size' => 20,
        'default_value' => array($hoten),
    );
    $filters['soqd'] = array(
        'type' => 'textfield',
        'title' => t('Số QĐ'),
        'size' => 20,
        'default_value' => array($soqd),
    );
    $filters['ngayqd'] = array(
        'type' => 'textfield',
        'title' => t('Ngày QĐ'),
        'size' => 10,
        'default_value' => array($ngayqd),
        'attributes' => array('title' =>'Nhập dd/mm/yyyy'),
    );
    $filters['namhoc'] = array(
        'type' => 'select',
        'multiple' => TRUE,
        'title' => t('<br>Năm học'),
        'options' => (array(''=>'Tất cả') + db_select('uit_lichsu_tinhtrang_hocvien', 'd')->distinct()->orderBy('d.namhoc')->fields('d', array('namhoc', 'namhoc'))->execute()->fetchAllKeyed()),
        'default_value' => $namhoc,
    );
    $filters['hocky'] = array(
        'type' => 'select',
        'multiple' => TRUE,
        'title' => t('Học kỳ'),
        'options' => (array(''=>'Tất cả','1'=>'1','2'=>'2','3'=>'3')),
        'default_value' => $hocky,
    );
    $filters['tinhtrang'] = array(
        'type' => 'select',
        'multiple' => TRUE,
        'title' => t('Tình trạng'),
        'options' => (array(''=>'Tất cả') + db_select('uit_tinhtrang', 'd')->distinct()->orderBy('d.id')->fields('d', array('id', 'ten'))->condition('doituong','SV')->execute()->fetchAllKeyed()),
        'default_value' => $tinhtrang,
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
          'callback' => 'ajax_filter_qlcapnhat_callback',
          'wrapper' => 'table_qlcapnhat',
        ),
    );
  
  return $filters;
}

function qlcapnhat_filter_form() {
    $form = array();
    $filters = qlcapnhat_filters();
    foreach ($filters as $key => $filter) {
        $arr = array();
        foreach ($filter as $key2 => $value2) {
            $arr['#'.$key2] = $value2;
        }
        $form['filters'][$key] = $arr;
    }
    return $form;
}

function qlcapnhat_build_filter_query(SelectQueryInterface $query) {
    $filter_data = isset($_SESSION['qlcapnhat_filter']) ? $_SESSION['qlcapnhat_filter'] : array();
    foreach ($filter_data as $key => $value) {
        switch ($key) {
            case 'timkiem':
            case 'sodong':
                break;
            case 'mahv':
                $arr = explode(',', $value);
                $str = '';
                foreach ($arr as $item) {
                    if($str == '') {
                        $str = "(t.mahv = '".trim($item)."')";
                    } else {
                        $str .= " OR (t.mahv = '".trim($item)."')";
                    }
                }
                $query->where($str);
                break;
            case 'hoten':
                $query->condition('sv.'.$key, '%'.$value.'%', 'LIKE');
                break;
            case 'soqd':
                $query->condition('t.'.$key, $value.'%', 'LIKE');
                break;
            case 'ngayqd':
                $query->condition('t.'.$key, ConvertToDate($value));
                break;
            case 'hocky':
            case 'namhoc':
            case 'tinhtrang':
                if(!array_key_exists('',$value)) $query->condition('t.'.$key, $value);
                break;
            
            default:
                $query->condition('t.'.$key, $value);
        }
    }
}