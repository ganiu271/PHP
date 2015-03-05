<?php
function qlhocvien_form($form, &$form_state) {
    if(!user_access('UIT qlhocvien admin') && !user_access('UIT qlhocvien') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $form = array();
    $form['#prefix'] = '<div id="form-qlhocvien-wrapper">';
    $form['#suffix'] = '</div>';
    $form['information'] = array(
      '#markup' =>
      '<center><h1><font color="blue"><strong>QUẢN LÝ HỌC VIÊN</strong></font></h1></center><br>');
    
    $form['table_filter_wraper'] = array('#type' => 'fieldset', '#title' => t('Chọn điều kiện lọc'), 
        '#attributes' => array('class' => array('container-inline')), '#collapsible' => TRUE, '#collapsed' => FALSE);
    qlhv_set_filters($form_state);
    $form['table_filter_wraper'][] = qlhv_filter_form();
    $form[] = cmdXuLy('0');
    $form['qlhv_data'] = array(
        '#prefix' => '<div id="table_dshv">',
        '#suffix' => '</div>',);
    if ($_GET['q'] == 'system/ajax') {
        $q = 'admin/qlhocvien' ;
        $_GET['q'] = $q;
    }
    $form['qlhv_data']['info'] = qlhv_info_form();
    $form['qlhv_data']['dshocvien_table'] = _qlhocvien_get_table_dshocvien();
    $form['qlhv_data']['pager'] = array('#markup' => theme('pager'));
    
    $form[] = cmdXuLy();
    return $form;
}

function cmdXuLy($num='') {
    global $user, $base_url;
    $form = array();
    $form['xuatexcel'.$num] = array(
        '#type' => 'button',
        '#value' => t('Xuất excel'),
        '#attributes' => array('title' =>'Xuất excel'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlhocvien_xuatexcel'),
    );
    /*
    $form['xuatexcel_img'.$num] = array(
        '#type' => 'button',
        '#value' => t('Xuất excel kèm hình'),
        '#attributes' => array('title' =>'Xuất excel kèm hình hv'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('qlhocvien_xuatexcel_img'),
    );
     * 
     */
    if(user_access('UIT qlhocvien admin')) {
        $form['xoahv'.$num] = array(
            '#type' => 'button',
            '#value' => t('Xóa'),
            '#attributes' => array('onclick'=>'return confirm(\'Bạn có chắc xóa các học viên được chọn (không phục hồi được) ?\');', 'title' =>'Xóa các HV được chọn'),
            '#executes_submit_callback' => TRUE,
            '#submit' => array('qlhocvien_xoa'),
        );
        $form['importhv'.$num] = array( 
            '#type' => 'button',
            '#value' => t('Import HV'),
//            '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/admin/qlhocvien/import/hocvien\');', 'title' =>'Import hv từ file excel'),
            '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/?q=admin/qlhocvien/import/hocvien\');', 'title' =>'Import HV từ file excel'),
            '#executes_submit_callback' => FALSE,
        );
        $form['importtt'.$num] = array( 
            '#type' => 'button',
            '#value' => t('Import tình trạng'),
//            '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/admin/qlhocvien/import/thongtin\');', 'title' =>'Import cập nhật tình trạng hv từ file excel'),
            '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/?q=admin/qlhocvien/import/thongtin\');', 'title' =>'Import cập nhật tình trạng HV từ file excel'),
            '#executes_submit_callback' => FALSE,
        );  
        $form['uploadhinhhv'.$num] = array( 
            '#type' => 'button',
            '#value' => t('Upload hình HV'),
//            '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/admin/qlhocvien/import/hinhanh\');', 'title' =>'Upload hình hv'),
            '#attributes' => array('onclick'=>'window.open(\''.$base_url.'/?q=admin/qlhocvien/import/hinhanh\');', 'title' =>'Upload hình HV'),
            '#executes_submit_callback' => FALSE,
        );
    }
    /*
    if(in_array("administrator", $user->roles)) {
        $form['themhv'.$num] = array( 
            '#type' => 'button',
            '#value' => t('Thêm hv'),
            '#attributes' => array('title' =>'Thêm 1 hv'),
            '#executes_submit_callback' => TRUE,
            '#submit' => array('qlhocvien_themhv'),
        );
        $form['taopass'.$num] = array( 
            '#type' => 'button',
            '#value' => t('Tạo mật khẩu'),
            '#attributes' => array('title' =>'Tạo mật khẩu ngẫu nhiên theo danh sách lọc'),
            '#executes_submit_callback' => TRUE,
            '#submit' => array('qlhocvien_taopass'),
        );
        $form['taofilead'.$num] = array( 
            '#type' => 'button',
            '#value' => t('File import AD'),
            '#attributes' => array('title' =>'Tạo file import AD'),
            '#executes_submit_callback' => TRUE,
            '#submit' => array('qlhocvien_taofilead'),
        );
        $form['taofileemail'.$num] = array( 
            '#type' => 'button',
            '#value' => t('File import Email'),
            '#attributes' => array('title' =>'Tạo file import Email'),
            '#executes_submit_callback' => TRUE,
            '#submit' => array('qlhocvien_taofileemail'),
        );

    }
     * 
     */
    return $form;
}

function qlhocvien_themhv() {
    drupal_goto('admin/qlhocvien/them');
}

function _qlhocvien_get_table_dshocvien() {
    $header = array(
        //array('data' => t('STT'), 'field' => ''),
        array('data' => t('MSHV'), 'field' => 'mahv'),
        array('data' => t('Họ tên'), 'field' => 'hoten'),
        array('data' => t('Ngày sinh'), 'field' => 'ngaysinh'),
//        array('data' => t('Cơ sở'), 'field' => 'tenvt'),
        array('data' => t('Giới tính'), 'field' => 'gioitinh'),
        array('data' => t('Lớp'), 'field' => 'lopsh'),
        array('data' => t('Khóa'), 'field' => 'khoahoc'),
        array('data' => t('Tình trạng'), 'field' => 'ten'),
        array('data' => t('Thao tác'), 'field' => '')
    );
    $query = db_select('uit_thongtinhv', 'hv')->extend('PagerDefault')->extend('TableSort')->orderByHeader($header);
    $query->join('uit_tinhtrang', 'tt', 'tt.id = hv.tinhtrang');
//    $query->leftJoin('uit_coso', 'cs', 'cs.macs = hv.coso');
    $query->fields('hv', array('mahv', 'hoten', 'lopsh', 'hedt', 'chuyennganh','khoahoc','ngaysinh','gioitinh'));
    $query->fields('tt', array('ten'));
//    $query->fields('cs', array('tenvt'));
    $query->orderBy('hv.mahv', 'ASC');
    qlhv_build_filter_query( $query);
    $sodong = isset($_SESSION['qlhv_filter']['sodong']) ? $_SESSION['qlhv_filter']['sodong'] : 50;
    $rows = $query->limit($sodong)->execute();
    $options = array();
    //$i = 1;
    foreach ($rows as $row) { 
        $options[$row->mahv] = array(
            //$i++,
            l($row->mahv,'admin/qlhocvien/xem/'.$row->mahv,array('attributes' => array('target' => '_blank'))),
            $row->hoten,
            strlen($row->ngaysinh) == 10 ? date('d/m/Y',  strtotime($row->ngaysinh)) : $row->ngaysinh,
//            $row->tenvt,
            $row->gioitinh == 1 ? 'Nam' : 'Nữ',
            $row->lopsh,
            $row->khoahoc,
            $row->ten,
            l('Sửa','admin/qlhocvien/capnhat/'.$row->mahv,array('attributes' => array('target' => '_blank'))),
        );
    }
    
    $form = array(
        '#title' => 'Table',
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        //'#default_value' => drupal_map_assoc($default),
        '#empty' => t('Không tìm thấy học viên nào.'),
        //'#sticky' => FALSE,
        '#prefix' => '<div class="table_dshocvien"  id="table_dshocvien">',
        '#suffix' => '</div>'
    );
    return $form;
}

function qlhocvien_xuathvnghihoc($form, &$form_state) {
    $hocky = variable_get('SYSTEM_HOCKY');
    $namhoc = variable_get('SYSTEM_NAMHOC');
    if($hocky == 1) {
        $hocky = 2;
        $namhoc = $namhoc - 1;
    } else {
        $hocky = $hocky -1;
        $namhoc = $namhoc;
    }
    if($hocky == 1) {
        $hocky_truoc = 2;
        $namhoc_truoc = $namhoc - 1;
    } else {
        $hocky_truoc = $hocky -1;
        $namhoc_truoc = $namhoc;
    }
    $dshv = db_select('uit_thongtinhv', 't')->fields('t', array('mahv', 'hoten', 'lopsh', 'tinhtrang'))->condition('tinhtrang', array(1), 'IN')->execute();
    //->condition('mahv', '1352%', 'NOT LIKE')
    $arr_dshv = array();
     foreach ($dshv as $hv) {
        $mahv = $hv->mahv;
        //$checkhp = db_select('uit_hocphi', 't')->fields('t', array('id'))->condition('dadong', 0, '>')
        //        ->condition('mahv', $mahv)->where("((hocky = '$hocky' and namhoc = '$namhoc') OR (hocky = '$hocky_truoc' and namhoc = '$namhoc_truoc'))")
        //        ->execute()->fetch();
        //$checkhp = db_select('uit_hocphi', 't')->fields('t', array('id'))->condition('dadong', 0, '>')->where("((hocky = 2 and namhoc = 2012) OR (hocky = 1 and namhoc = 2013))")
        //        ->condition('mahv', $mahv)->condition('trangthai', 1)->execute()->fetch();
        //$checkhp = db_select('uit_hocphi', 't')->fields('t', array('id'))->condition('dadong', 0, '>')->condition('namhoc', 2012)
        //        ->condition('mahv', $mahv)->condition('trangthai', 1)->execute()->fetch();
        $checkhp = db_select('uit_dkhp', 't')->fields('t', array('mahv'))->where("((hocky = 2 and namhoc = 2012) OR (hocky = 1 and namhoc = 2013))")
                ->condition('mahv', $mahv)->execute()->fetch();
        if($checkhp)            continue;
        $arr_dshv[$mahv] = $hv->tinhtrang;
    }
    return $arr_dshv;
}

function qlhocvien_xoa($form, &$form_state) {
    $arrmahv = array_filter($form_state['values']['dshocvien_table']);
    $strmahv = NULL;
    foreach ($arrmahv as $mahv) {
        if(qlhocvien_checkxoa($mahv)) {
            try {
                db_delete('uit_thongtinhv')->condition('mahv',$mahv)->execute();
                drupal_set_message('Đã xóa thành công hv: '.$mahv);
                if(!$strmahv) $strmahv = $mahv; else $strmahv .= '; '.$mahv;
            } catch (Exception $exc){
                drupal_set_message('Xóa không thành công hv: '.$mahv.' ('.$exc->getMessage().')', 'error');
            }
        } else {
            drupal_set_message('Không thể xóa hv '.$mahv.' vì còn dữ liệu liên quan', 'error');
        }
    }
    if($strmahv) ghilogs('Xóa các hv: '.$strmahv, '', 'qlhocvien');
}

function qlhocvien_checkxoa($mahv){
    $uid = db_select('uit_thongtinhv', 'd')->fields('d', array('uid'))->condition('mahv', $mahv)->execute()->fetchField();
    if($uid != '') return false;
    //$check = db_select('uit_dkhp', 'd')->fields('d', array('mahv'))->condition('mahv', $mahv)->execute()->fetchField();
    //if($check) return false;
    $check = db_select('uit_diem', 'd')->fields('d', array('mahv'))->condition('mahv', $mahv)->execute()->fetchField();
    if($check) return false;
    
    return true;
}


function qlhocvien_xuatexcel($form, &$form_state) {
    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    $filename = 'dshocvien_'.date('Ymd_His').'.xlsx';
    $path = variable_get('file_public_path', conf_path() . '/files') . '/exports';
    if(!file_exists($path)) mkdir($path);
    $path .= '/'.$filename;
    
    $objPHPExcel = new PHPExcel();
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    $r = 1;
    $col_start = 'A';
    
    $query = db_select('uit_thongtinhv', 'hv');
    $query->join('uit_tinhtrang', 'tt', 'tt.id = hv.tinhtrang');
    $query->fields('hv');
    //$query->fields('tt', array('ten'));
    $query->orderBy('hv.mahv', 'ASC');
    qlhv_build_filter_query( $query);
    $rows = $query->execute()->fetchAll();
    if(!$rows) {
        drupal_set_message('Không tìm thấy học viên để xuất', 'error');
        return;
    }
    set_time_limit(0);
    ini_set('memory_limit', '512M');
    $arrDanhmuc['tinhtrang'] = db_select('uit_tinhtrang', 't')->fields('t', array('id', 'ten'))->condition('doituong', 'sv')->execute()->fetchAllKeyed();
    $arrDanhmuc['dantoc'] = db_select('uit_dantoc', 't')->fields('t', array('id', 'ten'))->execute()->fetchAllKeyed();
    $arrDanhmuc['tongiao'] = db_select('uit_tongiao', 't')->fields('t', array('id', 'ten'))->execute()->fetchAllKeyed();
    $arrDanhmuc['xuatthan'] = db_select('uit_xuatthan', 't')->fields('t', array('id', 'ten'))->execute()->fetchAllKeyed();
    $arrDanhmuc['gioitinh'] = array(1 => 'Nam',0 => 'Nữ');
    //Headers
    $c = $col_start;
    $ActiveSheet->setCellValue($c++.$r, 'STT');
    foreach ($rows[0] as $key=>$value) {
        if($key != 'id' && $key != 'uid') {
            $ActiveSheet->setCellValue($c++.$r, $key);
        }
    }
    
    //Rows
    $r++;
    $stt = 1;
    foreach ($rows as $row) {
        $c = $col_start;
        $ActiveSheet->setCellValue($c++.$r, $stt++);
        foreach ($row as $key => $value) {
            switch ($key) {
                case 'id':
                case 'uid':
                    break;
                case 'mahv':
                case 'cmnd':
                case 'dienthoai':
                case 'cha_dienthoai':
                case 'me_dienthoai':
                case 'baoho_dienthoai':
                case 'baotin_dienthoai':
                    $ActiveSheet->setCellValueExplicit($c++.$r, $value, PHPExcel_Cell_DataType::TYPE_STRING);
                    break;
                case 'tinhtrang':
                case 'dantoc':
                case 'tongiao':
                case 'xuatthan':
                case 'gioitinh':
                    if(isset($arrDanhmuc[$key][$value])) {
                        $ActiveSheet->setCellValue($c++.$r, $arrDanhmuc[$key][$value]);
                    } else {
                        $c++;
                    }
                    break;

                default:
                    $ActiveSheet->setCellValue($c++.$r, $value);
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

function qlhocvien_xuatexcel_img($form, &$form_state) {
    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    $filename = 'dshocviencohinh_'.date('Ymd_His').'.xlsx';
    $path = variable_get('file_public_path', conf_path() . '/files') . '/exports';
    if(!file_exists($path)) mkdir($path);
    $path .= '/'.$filename;
    
    $objPHPExcel = new PHPExcel();
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    $r = 1;
    $col_start = 'A';
    
    $query = db_select('uit_thongtinhv', 'hv');
    $query->join('uit_tinhtrang', 'tt', 'tt.id = hv.tinhtrang');
    $query->fields('hv');
    //$query->fields('tt', array('ten'));
    $query->orderBy('hv.mahv', 'ASC');
    qlhv_build_filter_query( $query);
    $rows = $query->execute()->fetchAll();
    if(!$rows) {
        drupal_set_message('Không tìm thấy học viên để xuất', 'error');
        return;
    }
    $arrTinhTrang = db_select('uit_tinhtrang', 't')->fields('t', array('id', 'ten'))->condition('doituong', 'hv')->execute()->fetchAllKeyed();
    $arrDanToc = db_select('uit_dantoc', 't')->fields('t', array('id', 'ten'))->execute()->fetchAllKeyed();
    $arrTonGiao = db_select('uit_tongiao', 't')->fields('t', array('id', 'ten'))->execute()->fetchAllKeyed();
    $arrXuatThan = db_select('uit_xuatthan', 't')->fields('t', array('id', 'ten'))->execute()->fetchAllKeyed();
    
    //Headers
    $c = $col_start;
    $ActiveSheet->setCellValue($c++.$r, 'STT');
    foreach ($rows[0] as $key=>$value) {
        if($key != 'id' && $key != 'uid') {
            $ActiveSheet->setCellValue($c++.$r, $key);
        }
    }
    $ActiveSheet->setCellValue($c.$r, 'Hình');
    
    //Rows
    $r++;
    $stt = 1;
    $img_path = variable_get('QLhv_THUMUCANHhv');
    
    foreach ($rows as $row) {
        $c = $col_start;
        $ActiveSheet->getRowDimension($r)->setRowHeight(115);
        $ActiveSheet->setCellValue($c++.$r, $stt++);
        foreach ($row as $key => $value) {
            switch ($key) {
                case 'id':
                case 'uid':
                    break;
                case 'mahv':
                case 'cmnd':
                case 'dienthoai':
                case 'cha_dienthoai':
                case 'me_dienthoai':
                case 'baoho_dienthoai':
                case 'baotin_dienthoai':
                    $ActiveSheet->setCellValueExplicit($c++.$r, $value, PHPExcel_Cell_DataType::TYPE_STRING);
                    break;
                case 'tinhtrang':
                    $ActiveSheet->setCellValue($c++.$r, $arrTinhTrang[$value]);
                    break;
                case 'dantoc':
                    $ActiveSheet->setCellValue($c++.$r, $arrDanToc[$value]);
                    break;
                case 'tongiao':
                    $ActiveSheet->setCellValue($c++.$r, $arrTonGiao[$value]);
                    break;
                case 'xuatthan':
                    $ActiveSheet->setCellValue($c++.$r, $arrXuatThan[$value]);
                    break;
                case 'gioitinh':
                    if($value) {
                        $ActiveSheet->setCellValue($c++.$r, 'Nam');
                    } else {
                        $ActiveSheet->setCellValue($c++.$r, 'Nữ');
                    }
                    break;

                default:
                    $ActiveSheet->setCellValue($c++.$r, $value);
            }
        }
        //Add hình hv
        try {
            $path_img_hv = $img_path.'/20'.substr($row->mahv, 0, 2).'/'.$row->mahv.'.jpg';
            if(file_exists($path_img_hv)) {
                $objDrawing = new PHPExcel_Worksheet_Drawing();
                $objDrawing->setWorksheet($ActiveSheet, FALSE);
                $objDrawing->setPath($img_path.'/20'.substr($row->mahv, 0, 2).'/'.$row->mahv.'.jpg');
                $objDrawing->setCoordinates($c.$r);
                $objDrawing->setWidth(114);
                $objDrawing->setHeight(152);
            }
        } catch (Exception $exc) {
            drupal_set_message('Không tìm thấy hình hv '.$row->mahv, 'error');
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

function qlhocvien_taofilead($form, &$form_state) {
    $template = variable_get('QLhv_MAUIMPORTAD', '');
    if($template == '') {
        drupal_set_message('Chưa có thông tin mẫu tạo file (admin/qlhocvien/settings)','error');
        return;
    }
    $query = db_select('uit_thongtinhv', 'hv');
    $query->fields('hv', array('mahv', 'hoten', 'matkhau'));
    $query->orderBy('hv.mahv', 'ASC');
    qlhv_build_filter_query( $query);
    $rows = $query->execute()->fetchAll();
    if(!$rows) {
        drupal_set_message('Không có thông tin để xử lý');
        return;
    }
    $strcontent = '';
    foreach ($rows as $row) {
         $hoten = TachHoTen($row->hoten);
         $str = str_replace('<mahv>', $row->mahv, $template);
         $str = str_replace('<hoten>', $row->hoten, $str);
         $str = str_replace('<password>', $row->matkhau, $str);
         $str = str_replace('<ho>', $hoten['ho'], $str);
         $str = str_replace('<ten>', $hoten['ten'], $str);
         if($strcontent == '') {
             $strcontent = $str;
         } else {
             $strcontent .= PHP_EOL.PHP_EOL.$str;
         }
    }
    $length = strlen($strcontent);

    header('Content-Description: File Transfer');
    header('Content-Type: text/plain');
    header('Content-Disposition: attachment; filename=import_ad.ldf');
    header('Content-Transfer-Encoding: binary');
    header('Content-Length: ' . $length);
    header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
    header('Expires: 0');
    header('Pragma: public');

    echo $strcontent;
    exit;
}

function qlhocvien_taopass($form, $form_state) {
    $query = db_select('uit_thongtinhv', 'hv');
    $query->fields('hv', array('mahv'));
    $query->orderBy('hv.mahv', 'ASC');
    qlhv_build_filter_query( $query);
    $rows = $query->execute()->fetchAll();
    if(!$rows) {
        drupal_set_message('Không có thông tin để xử lý');
        return;
    }
    $succ = 0;
    $pass = rand(1000000000, 9999999999);
    foreach ($rows as $row) {
        try {
            $ret = db_update('uit_thongtinhv')->fields(array('matkhau'=>$pass))->condition('mahv',$row->mahv)->where("matkhau is null OR matkhau=''")->execute();
            if($ret) $succ++;
        } catch (Exception $exc) {
            drupal_set_message('Có lỗi xãy ra: '.$exc->getMessage(),'error');
        }
    }
    drupal_set_message('Đã cập nhật thành công '.$succ.' hv');
    return;
}


function qlhv_build_filter_query(SelectQueryInterface $query) {
    $filter_data = isset($_SESSION['qlhv_filter']) ? $_SESSION['qlhv_filter'] : array();
    foreach ($filter_data as $key => $value) {
        switch ($key) {
            case 'timkiem':
                break;
            case 'mahv':
                //$arrmahv = explode(',', $value);
                $arrmahv = preg_split('/,| /', $value, -1, PREG_SPLIT_NO_EMPTY);
                $query->condition('hv.'.$key, $arrmahv, 'IN');
                break;
            case 'lopsh':
                $arrmalop = explode(',', $value);
                $query->condition('hv.'.$key, $arrmalop, 'IN');
                break;
            case 'hoten':
                $query->where('(hv.hoten like \'%'.$value.'\' OR hv.hoten like \''.$value.'%\')');
                break;
            case 'tinhtrang':
                if(!array_key_exists('',$value)) $query->condition('hv.'.$key, $value, 'IN');
                break;
            case 'coso': break;
            case 'sodong': break;
            case 'khoahoc':
                if(!array_key_exists('',$value)) $query->condition('hv.'.$key, $value);
                break;
            case 'chinhsach': break;
            case 'khuvuc': break;
            case 'vungmien': break;
            case 'dienthoai':
                $query->condition('hv.'.$key, '%'.$value.'%','LIKE');
                break;
            case 'noisinh':
                $query->condition('hv.'.$key, '%'.$value.'%','LIKE');
                break;
            case 'diachi_tamtru':
                $query->condition('hv.'.$key, '%'.$value.'%','LIKE');
                break;
            default:
                $query->condition('hv.'.$key, $value);
        }
    }
}

function qlhv_set_filters(&$form_state) {
  $filters = qlhv_filters();
  //Apply every filter that has a choice selected other than 'any'.
  foreach ($filters as $filter => $options) {
    if (isset($form_state['values'][$filter])) {
      if( $form_state['values'][$filter] != '') {
        // Flatten the options array to accommodate hierarchical/nested options.
        //$flat_options = form_options_flatten($filters[$filter]['options']);
        $_SESSION['qlhv_filter'][$filter] = $form_state['values'][$filter];
        // Only accept valid selections offered on the dropdown, block bad input.
        //if (isset($flat_options[$form_state['values'][$filter]])) {
        //  $_SESSION['qlhv_filter'][$filter] = $form_state['values'][$filter];
        //}
      }
      else {
        unset($_SESSION['qlhv_filter'][$filter]);
      }
    } 
  }
}

function qlhv_filters() {
  $filters = array();
  if(!isset($_SESSION['qlhv_filter']['tinhtrang'])) $_SESSION['qlhv_filter']['tinhtrang'] = array(1);
  $session = isset($_SESSION['qlhv_filter']) ? $_SESSION['qlhv_filter'] : array();
  $mahv = !empty($session['mahv']) ? $session['mahv'] : NULL;
  $hoten = !empty($session['hoten']) ? $session['hoten'] : NULL;
  $chuyennganh = !empty($session['chuyennganh']) ? $session['chuyennganh'] : NULL;
  $dienthoai = !empty($session['dienthoai']) ? $session['dienthoai'] : NULL;
  $noisinh = !empty($session['noisinh']) ? $session['noisinh'] : NULL;
  //$diachi_tamtru = !empty($session['diachi_tamtru']) ? $session['diachi_tamtru'] : NULL;
  $hedt = !empty($session['hedt']) ? $session['hedt'] : NULL;
  //$lopsh = !empty($session['lopsh']) ? $session['lopsh'] : NULL;
  $khoahoc = !empty($session['khoahoc']) ? $session['khoahoc'] : array('');
  $gioitinh = !empty($session['gioitinh']) ? $session['gioitinh'] : null;
  $tinhtrang = !empty($session['tinhtrang']) ? $session['tinhtrang'] : array('');
  //$khuvuc = !empty($session['khuvuc']) ? $session['khuvuc'] : array('');
  //$chinhsach = !empty($session['chinhsach']) ? $session['chinhsach'] : array('');
  //$vungmien = !empty($session['vungmien']) ? $session['vungmien'] : array('');
  $sodong = !empty($session['sodong']) ? $session['sodong'] : 50;
  //$query = db_select('uit_thongtinhv', 'hv')->distinct()->orderBy('hv.lopsh');
  //$query->fields('hv',array('lopsh','lopsh'));
//  $arrkhoahoc = array(''=>'Tất cả');

//  for($i = 2001;$i < date('Y');$i++) {
//      $arrkhoahoc[$i] = $i;
//  }
     
  $filters['mahv'] = array(
      'type' => 'textfield',
      'title' => t('MSHV:'),
      'size' => 80,
      'maxlength' => 4000,
      'default_value' => $mahv,
      'attributes' => array('title' =>t('Mỗi MSHV cách nhau bởi dấu phẩy (,)')),
  );
  $filters['hoten'] = array(
      'type' => 'textfield',
      'title' => t('Họ tên:'),
      'size' => 30,
      'default_value' => $hoten,
      'attributes' => array('title' =>t('Nhập họ hoặc tên hoặc cả 2')),
  );
  /*
  $filters['lopsh'] = array(
      'type' => 'textfield',
      'title' => t('Lớp:'),
      'size' => 30,
      'default_value' => $lopsh,
      'attributes' => array('title' =>t('Mỗi lớp cách nhau bởi dấu phẩy (,)')),
  );
   * 
   */
  $filters['dienthoai'] = array(
      'type' => 'textfield',
      'title' => t('Điện thoại:'),
      'size' => 15,
      'default_value' => $dienthoai,
      'attributes' => array('title' =>t('Nhập số điện thoại cần tìm (gần đúng)')),
  );
  $filters['noisinh'] = array(
      'type' => 'textfield',
      'title' => t('Nơi sinh:'),
      'size' => 20,
      'default_value' => array($noisinh),
      'attributes' => array('title' =>t('Nhập nơi sinh cần tìm (gần đúng)')),
  );
  /*
  $filters['diachi_tamtru'] = array(
      'type' => 'textfield',
      'title' => t('Nơi ở:'),
      'size' => 33,
      'default_value' => array($diachi_tamtru),
      'attributes' => array('title' =>t('Nhập địa chỉ tạm trú (nơi ở) cần tìm (gần đúng)')),
  );
   * 
   */
  $filters['chuyennganh'] = array(
    'type' => 'select',
    'title' => t('Ngành:'),
    'options' => (array('' => t('Tất cả')) + db_select('uit_nganh', 'k')->orderBy('nganh')->fields('k', array('nganh', 'nganh'))->execute()->fetchAllKeyed()),
    'default_value' => $chuyennganh,
  );
  $filters['hedt'] = array(
    'type' => 'select',
    'title' => t('Hệ ĐT'),
    'options' => (array('' => t('Tất cả')) + db_select('uit_hedt', 'k')->orderBy('kyhieu')->fields('k', array('kyhieu', 'tenhedt'))->execute()->fetchAllKeyed()),
    'default_value' => array($hedt),
  );
  $filters['gioitinh'] = array(
        'type' => 'select',
        'title' => t('Giới tính'),
        'options' =>(array('' => t('Tất cả'),'0'=>t('Nữ'),'1'=>t('Nam'))),
        'default_value' => $gioitinh,
  );
//  $filters['coso'] = array(
//    'type' => 'select',
//    'multiple' => TRUE,
//    'title' => t('Cơ sở'),
//    'options' => (array('' => t('Tất cả')) + db_select('uit_coso', 'k')->orderBy('macs')->fields('k', array('macs', 'tenvt'))->execute()->fetchAllKeyed()),
//    'default_value' => $coso,
//  );
  $filters['khoahoc'] = array(
    'type' => 'select',
    'multiple' => TRUE,
    'title' => t('Khóa'),
    'options' =>(array('' => t('Tất cả')) + db_select('uit_thongtinhv', 'k')->distinct()->orderBy('khoahoc')->fields('k', array('khoahoc','khoahoc'))->execute()->fetchAllKeyed()),
    'default_value' => $khoahoc,
  );
  $filters['tinhtrang'] = array(
    'type' => 'select',
    'multiple' => TRUE,
    'title' => t('Tình trạng'),
    'options' => (array('' => t('Tất cả')) + db_select('uit_tinhtrang', 'k')->orderBy('ten')->fields('k', array('id', 'ten'))->condition('k.doituong','SV')->execute()->fetchAllKeyed()),
    'default_value' => $tinhtrang,
    'attributes' => array('title' =>t('Nhấn giữ Ctrl để chọn nhiều tình trạng')),
  );
    $filters['sodong'] = array(
        'type' => 'select',
        'title' => t('Số dòng'),
        'options' => (array('50' => t('50'),'100'=>t('100'),'200'=>t('200'),'500'=>t('500'))),
        'default_value' => $sodong,
    );
//  $filters['khuvuc'] = array(
//    'type' => 'select',
//    'multiple' => TRUE,
//    'title' => t('Khu vực'),
//    'options' => array('' => t('Tất cả'),'1'=>'KV1','2'=>'KV2','2NT'=>'KV2-NT','3'=>'KV3'),
//    'default_value' => $khuvuc,
//    'attributes' => array('title' =>t('Nhấn giữ Ctrl để chọn nhiều khu vực')),
//  );
//  $filters['vungmien'] = array(
//    'type' => 'select',
//    'multiple' => TRUE,
//    'title' => t('Vùng miền'),
//    'options' => array('' => t('Tất cả'),'MN'=>'Miền núi','VS'=>'Vùng sâu','VC'=>'Vùng cao','NT'=>'Nông thôn','HĐ'=>'Hải đảo','NN'=>'hv nước ngoài'),
//    'default_value' => $vungmien,
//    'attributes' => array('title' =>t('Nhấn giữ Ctrl để chọn nhiều vùng miền')),
//  );
//  $filters['chinhsach'] = array(
//    'type' => 'select',
//    'multiple' => TRUE,
//    'title' => t('Chính sách'),
//    'options' => array('' => t('Tất cả'),'TB'=>'Thương binh','BB'=>'Bệnh binh','LS'=>'Liệt sĩ'),
//    'default_value' => $chinhsach,
//    'attributes' => array('title' =>t('Nhấn giữ Ctrl để chọn nhiều mục')),
//  );
//
  $filters['timkiem'] = array(
    'type' => 'button',
    'value' => t('Tìm kiếm'),
    'ajax' => array(
      'event' => 'click',
      'callback' => 'ajax_filter_qlhocvien_callback',
      'wrapper' => 'table_dshv',
    ),
);
  
  return $filters;
}

function qlhv_filter_form() {
    $form = array();
    $filters = qlhv_filters();
    foreach ($filters as $key => $filter) {
        $arr = array();
        foreach ($filter as $key2 => $value2) {
            $arr['#'.$key2] = $value2;
        }
        $form['filters'][$key] = $arr;
    }
    return $form;
}

function qlhv_info_form() {
    $query = db_select('uit_thongtinhv','hv');
    $query->addExpression('count(hv.mahv)', 'tonghv');
    qlhv_build_filter_query( $query);
    $tonghv = $query->execute()->fetchField();
    $form['info'] = array(array('#markup' =>'<br><strong>Tổng số học viên: '.number_format($tonghv).'</strong>'));
    return $form;
}

function ajax_filter_qlhocvien_callback($form, $form_state) {
  return $form['qlhv_data'];
}

?>