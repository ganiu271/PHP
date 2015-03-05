<?php
function qldiem_form_bangdiem($form, &$form_state) {
    if(!user_access('Quan ly diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $html = '<center><h1>QUẢN LÝ ĐIỂM HỌC VIÊN</h1></center>';
    $form['markup'] = array('#markup' => $html);
    $form['table_filter_wraper'] = array('#type' => 'fieldset', '#title' => t('Chọn điều kiện lọc'), '#attributes' => array('class' => array('container-inline')));
    qldiemhv_set_filters($form_state);
    $form['table_filter_wraper'][] = qldiemhv_filter_form();
    
    $form['qldiemhv_data'] = array(
        '#prefix' => '<div id="table_qldiemhv">',
        '#suffix' => '</div>',);
    $form['qldiemhv_data']['qldiemhv_count'] = array('#markup' => '<br><strong>Tổng số HV: '.number_format(qldiemhv_get_count_dssinhvien()).'</strong><br>');
    
    $form['qldiemhv_data']['qldiemhv_table'] = qldiemhv_get_table_dssinhvien();
    $tmp_q = $_GET['q']; 
    $_GET['q'] = 'admin/qldiem/bangdiem';
    $form['qldiemhv_data']['pager'] = array('#markup' => theme('pager'));
    $_GET['q'] = $tmp_q;
    
    return $form;
}

function qldiemhv_get_count_dssinhvien() {
    $query = db_select('uit_thongtinhv', 'hv');
    $query->addExpression('count(mahv)', 'tonghv');
    qldiemhv_build_filter_query( $query);
    return $query->execute()->fetchField();
}

function qldiemhv_get_table_dssinhvien() {
    $header = array(
        array('data' => t('MSHV'), 'field' => 'mahv'),
        array('data' => t('Họ tên'), 'field' => 'hoten'),
        array('data' => t('Ngành'), 'field' => 'chuyennganh'),
        array('data' => t('Khóa học'), 'field' => 'khoahoc'),
        array('data' => t('Hệ đào tạo'), 'field' => 'hedt'),
        //array('data' => t('Lớp'), 'field' => 'lopsh'),
        array('data' => t('Tình trạng'), 'field' => 'ten'),
        array('data' => t('Chức năng'), 'field' => ''),
    );
    $query = db_select('uit_thongtinhv', 'hv')->extend('PagerDefault')->limit($_SESSION['qldiemhv_filter']['sodong'])->extend('TableSort')->orderByHeader($header);
    $query->join('uit_tinhtrang', 'tt', 'tt.id = hv.tinhtrang');
//    $query->leftJoin('uit_coso', 'cs', 'cs.macs = hv.coso');
    $query->fields('hv', array('mahv', 'hoten', 'chuyennganh', 'hedt','khoahoc'));
    $query->fields('tt', array('ten'));
//    $query->fields('cs', array('tenvt'));
    $query->orderBy('hv.mahv', 'ASC');
    qldiemhv_build_filter_query( $query);
    $rows = $query->execute();
    $options = array();
    //$i = 1;
    foreach ($rows as $row) {
        $options[$row->mahv] = array(
            l($row->mahv,'admin/qlhocvien/xem/'.$row->mahv,array('attributes' => array('target' => '_blank'))),
            $row->hoten,
            $row->chuyennganh,
            $row->khoahoc,
            $row->hedt,
            //$row->lopsh,
            $row->ten,
            l('Xem điểm','admin/qldiem/xem/'.$row->mahv,array('attributes' => array('target' => '_blank'))).' | '
            .l('In','print/admin/qldiem/in/'.$row->mahv,array('attributes' => array('target' => '_blank'))).' | '
            .l('BĐTN','print/admin/qldiem/bangdiemtn/'.$row->mahv,array('attributes' => array('target' => '_blank'))).' | '
            .l('Export excel','admin/qldiem/xuat/bangdiem/'.$row->mahv.'/nm',array('attributes' => array('target' => '_blank'))).' | '
            .l('Export excel TN','admin/qldiem/xuat/bangdiem/'.$row->mahv.'/tn',array('attributes' => array('target' => '_blank')))
        );
    }
    $form = array(
        '#title' => 'Table',
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#empty' => t('Không tìm thấy sinh viên nào.'),
    );
    return $form;
}

function qldiemhv_capnhattrangthai($form, &$form_state) {
    //global $user;
    if(!user_access('Quan ly diem') ) {
        //if($user->uid == 0) drupal_goto('user/login', array('query' => drupal_get_destination()));
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $a_hv = array_filter($form_state['values']['qldiemhv_table']);
    if(!$a_hv) {
        $query = db_select('uit_thongtinhv', 'hv');
        $query->fields('hv', array('mahv', 'mahv'));
        $query->orderBy('hv.mahv', 'ASC');
        qldiemhv_build_filter_query( $query);
        $a_hv = $query->execute()->fetchAllKeyed();
    }
    $n=0;
    foreach ($a_hv as $mahv) {
        if(CapNhatTrangThai($mahv)) $n++;
    }
    drupal_set_message('Tổng số hv đã cập nhật thành công ('.$n.')');
}

function qldiemhv_xuatbangdiemth($form, &$form_state) {
    if(!user_access('Quan ly diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $arrID = array_filter($form_state['values']['qldiemhv_table']);
    if(count($arrID) == 0) {
        drupal_set_message('Cần phải chọn hv để xuất bảng điểm','error');
        return;
    }
    if(count($arrID) == 1) {
        $filename = qldiemhv_export_excel(key($arrID));
        if($filename){
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
    } else {
        set_time_limit(0);
        $path = variable_get('file_public_path', conf_path() . '/files') . '/exports/BangDiem_'.date('y-m-d_H-i-s').'.zip';
        $zip = new ZipArchive();
        // open archive 
        if ($zip->open($path, ZIPARCHIVE::CREATE) !== TRUE) {
            die ("Could not open archive");
        }
        $fnames = array();
        foreach ($arrID as $mahv) {
            $fn = qldiemhv_export_excel($mahv);
            if($fn) {
                $zip->addFile ($fn, basename($fn));
                $fnames[] = $fn;
            }
        }
        $zip->close();
        foreach ($fnames as $fname) {
            unlink($fname);
        }
        if(filesize($path)){
          drupal_add_http_header('Pragma', 'public');
          drupal_add_http_header('Expires', '0');
          drupal_add_http_header('Cache-Control', 'must-revalidate, post-check=0, pre-check=0');
          drupal_add_http_header('Content-Type', 'application/application/x-zip');
          drupal_add_http_header('Content-Disposition', 'attachment; filename=' . basename($path) . ';');
          drupal_add_http_header('Content-Transfer-Encoding', 'binary');
          drupal_add_http_header('Content-Length', filesize($path));
          readfile($path);
          unlink($path);
          drupal_exit();
        }
    }
}

function qldiemhv_xuatbangdiemth_new($form, &$form_state) {
    if(!user_access('Quan ly diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $arrID = array_filter($form_state['values']['qldiemhv_table']);
    if(count($arrID) == 0) {
        drupal_set_message('Cần phải chọn hv để xuất bảng điểm','error');
        return;
    }
    if(count($arrID) == 1) {
        $filename = qldiemhv_export_excel_1(key($arrID));
        if($filename){
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
    } else {
        set_time_limit(0);
        $path = variable_get('file_public_path', conf_path() . '/files') . '/exports/BangDiem_'.date('y-m-d_H-i-s').'.zip';
        $zip = new ZipArchive();
        // open archive 
        if ($zip->open($path, ZIPARCHIVE::CREATE) !== TRUE) {
            die ("Could not open archive");
        }
        $fnames = array();
        foreach ($arrID as $mahv) {
            $fn = qldiemhv_export_excel_1($mahv);
            if($fn) {
                $zip->addFile ($fn, basename($fn));
                $fnames[] = $fn;
            }
        }
        $zip->close();
        foreach ($fnames as $fname) {
            unlink($fname);
        }
        if(filesize($path)){
          drupal_add_http_header('Pragma', 'public');
          drupal_add_http_header('Expires', '0');
          drupal_add_http_header('Cache-Control', 'must-revalidate, post-check=0, pre-check=0');
          drupal_add_http_header('Content-Type', 'application/application/x-zip');
          drupal_add_http_header('Content-Disposition', 'attachment; filename=' . basename($path) . ';');
          drupal_add_http_header('Content-Transfer-Encoding', 'binary');
          drupal_add_http_header('Content-Length', filesize($path));
          readfile($path);
          unlink($path);
          drupal_exit();
        }
    }
}

function qldiemhv_xuatbangdiemhknh($form, &$form_state) {
    //global $user;
    if(!user_access('Quan ly diem') ) {
        //if($user->uid == 0) drupal_goto('user/login', array('query' => drupal_get_destination()));
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $hocky = (int)$form_state['values']['chonhocky'];
    $namhoc = (int)$form_state['values']['chonnamhoc'];
    if($hocky <= 0 || $hocky > 3) $hocky = NULL;
    if($namhoc < 2006 || $namhoc > date('Y')) $namhoc = NULL;
    $arrID = array_filter($form_state['values']['qldiemhv_table']);
    if(count($arrID) == 0) {
        drupal_set_message('Cần phải chọn hv để xuất bảng điểm','error');
        return;
    }
    if(count($arrID) == 1) {
        $filename = qldiemhv_export_excel(key($arrID), $hocky, $namhoc);
        if($filename){
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
    } else {
        set_time_limit(0);
        $path = variable_get('file_public_path', conf_path() . '/files') . '/exports/BangDiem_'.date('y-m-d_H-i-s').'.zip';
        $zip = new ZipArchive();
        // open archive 
        if ($zip->open($path, ZIPARCHIVE::CREATE) !== TRUE) {
            die ("Could not open archive");
        }
        foreach ($arrID as $mahv) {
            $fn = qldiemhv_export_excel($mahv, $hocky, $namhoc);
            if($fn) $zip->addFile ($fn, basename($fn));
        }
        $zip->close();
        if(filesize($path)){
          drupal_add_http_header('Pragma', 'public');
          drupal_add_http_header('Expires', '0');
          drupal_add_http_header('Cache-Control', 'must-revalidate, post-check=0, pre-check=0');
          drupal_add_http_header('Content-Type', 'application/application/x-zip');
          drupal_add_http_header('Content-Disposition', 'attachment; filename=' . basename($path) . ';');
          drupal_add_http_header('Content-Transfer-Encoding', 'binary');
          drupal_add_http_header('Content-Length', filesize($path));
          readfile($path);
          unlink($path);
          drupal_exit();
        }
    }
}

function qldiemhv_export_excel_1($mahv, $hocky=NULL) {
    global $user;
    if(!$mahv) return;
    $q = db_select('uit_thongtinhv', 'hv');
    $q->leftJoin('uit_nganh', 'n', 'n.nganh = hv.chuyennganh');
    $q->fields ('hv', array('hoten', 'ngaysinh', 'noisinh', 'gioitinh', 'lopsh', 'khoahoc','hedt','chuyennganh'));
//    $q->fields('n', array('tennganh','tgdt'));
    $q->condition('hv.mahv',$mahv);
    $hv = $q->execute()->fetch();
    
    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $filename = 'BangDiem_'.$mahv.'.xlsx';
    $path = variable_get('file_public_path', conf_path() . '/files') . '/exports/'.$filename;
    $templatefile = drupal_get_path('module', 'qldiem').'/templates/MauBangDiem.xlsx';
    $objPHPExcel = PHPExcel_IOFactory::load($templatefile);
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    $ActiveSheet->setCellValue('D2', $hv->hoten)
              ->setCellValue('G2', (strlen($hv->ngaysinh) == 10 ? date('d-m-Y',  strtotime($hv->ngaysinh)):''))
              ->setCellValue('D3', $hv->noisinh)
              ->setCellValue('D4', $hv->khoahoc.'-'.($hv->khoahoc + (int)$hv->tgdt)) // ??????
              ->setCellValue('F3', ($hv->gioitinh == 1 ? 'Nam':'Nữ'))
              ->setCellValue('G4', $mahv.($hv->lopsh != '' ? ', '.$hv->lopsh : ' '))
              ->setCellValue('D5', 'Ngành: Công nghệ thông tin - '.$hv->tennganh);

    $q = db_select('uit_diem', 'd');
//    $q->fields ('d')->fields('mh',array('tenmh'));
//    $q->leftJoin('uit_ctdaotao', 'mh', 'd.mamh = mh.mamh and mh.nganh = d.nganh');
    $q->leftJoin('uit_ctdaotao', 'ctdt', 'd.mamh = ctdt.mamh');
    $q->leftJoin('uit_monhoc','mh','d.mamh=mh.mamh');
    $q->fields ('d')->fields('mh')->fields('ctdt');
    $q->condition('d.mahv', $mahv);
    $q->condition('d.trangthai', 1);
    //$q->condition('d.trangthai', 0, '>');
    //$q->condition('d.trangthai', 5, '<>');
    if($hocky) $q->condition('d.hocky', $hocky);
    $q->orderBy('d.hocky','ASC')->orderBy('d.mamh','ASC');
    $rows = $q->execute()->fetchAll();
    
    $ctdt = db_select('uit_ctdaotao','c')->fields('c',array('mamh','sotc'))
            ->condition('hedt', $hv->hedt)->condition('nganh', $hv->chuyennganh)
            ->execute()->fetchAllKeyed();
    
    $arrDiem = array();
    $arrCheck = array();
    foreach($rows as $row) {
//        if(!isset($ctdt[$row->mamh])) {
//            $mamhtd = MonHocTuongDuong($row->mamh, $hv->chuyennganh);
//            if(!$mamhtd) continue;
//            $cont = TRUE;
//            foreach ($mamhtd as $mamh2=>$value) {
//                if(isset($ctdt[$mamh2])) {
//                    if(isset($arrDiem[$mamh2])) {
//                        break;
//                    }
//                    $data = db_select('uit_ctdaotao','c')->fields('c',array('tenmh','sotc'))
//                            ->condition('hedt', $hv->hedt)->condition('nganh', $hv->chuyennganh)
//                            ->condition('mamh', $mamh2)
//                            ->execute()->fetch();
//                    if($data) {
//                        $row->mamh = $mamh2;
//                        $row->tenmh = $data->tenmh;
//                        $row->sotc = $data->sotc;
//                        $cont = FALSE;
//                        break;
//                    }
//                }
//            }
//            if($cont) continue;
//        }
        
        $mamh = $row->mamh;
        $diem = $row->diem;
        $sotc = $row->sotc;
        $ind1 = $row->hocky;
        $ind2 = $ind1.'-'.$mamh;
        
        $arrDiem[$ind1][] = array(
            'mamh' => $mamh,
            'tenmh' => $row->tenmh,
            'sotc' => $sotc,
            'diem' => $diem,
            'ghichu' => $row->ghichu,
        );
        if($row->trangthai == 1) {
            $arrCheck[$ind2] = 0;
        } else {
            $arrCheck[$ind2] = 1;
        }
    }
    $sohk = count($arrDiem);
    $somon = 0;
    foreach ($arrDiem as $hknh => $hp) {
        $somon += count($hp);
    }
    $styleArray = array('font' => array('bold' => true));
    $style_row = array('fill' => array('type' => PHPExcel_Style_Fill::FILL_SOLID,'color' => array('rgb'=>'c0c0c0')));
    
    $r = 7;
    $tctichluy = 0;
    $tctl = 0;
    $diemtichluy = 0;
    //$n = 1;
    
    $ActiveSheet->insertNewRowBefore($r+1, (2*$sohk)+$somon-1);
    foreach ($arrDiem as $hknh => $hp) {
        $ActiveSheet->mergeCells('C'.$r.':G'.$r);
        $ActiveSheet->setCellValue('C'.$r, 'Nhóm học phần '.$hknh);
        $ActiveSheet->getStyle('C'.$r)->applyFromArray($styleArray);
        $ActiveSheet->getStyle('C'.$r)->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_LEFT);
        $n=1;
        $r++;
        $tongtc = 0;
        $tongdiem = 0;
        foreach ($hp as $mh) {
            $mamh = $mh['mamh'];
            $tenmh = $mh['tenmh'];
            $sotc = $mh['sotc'];
            $diem = $mh['diem'];
            $ghichu = $mh['ghichu'];
            if($sotc > 0) {
                $tongtc += $sotc;
                $tongdiem += $diem*$sotc;
            }
            $ActiveSheet->setCellValue('B'.$r, $n++)
                ->setCellValue('C'.$r, $mamh)
                ->setCellValue('D'.$r, $tenmh)
                ->setCellValue('E'.$r, $sotc)
                ->setCellValue('F'.$r, $diem)
                ->setCellValue('G'.$r, $ghichu);
            if($sotc > 0) {
                $tctichluy += $sotc;
                $diemtichluy += $diem*$sotc;
                if($diem >= 5) {
                    $tctl +=$sotc;
                }
            }
            $r++;
        }
        if(count($hp) > 0) {
            $ActiveSheet
                ->setCellValue('B'.$r, '')
                ->setCellValue('C'.$r, '')
                ->setCellValue('D'.$r, 'Trung bình học phần')
                ->setCellValue('E'.$r, $tongtc)
                ->setCellValue('F'.$r, round($tongdiem/$tongtc,2))
                ->setCellValue('G'.$r, '');
            $ActiveSheet->getStyle('D'.$r.':F'.$r)->applyFromArray($styleArray);
            $r++;
        }
    }
  
    if($tctichluy > 0 && (!$hocky || !$namhoc)) {
        $ActiveSheet->setCellValue('E'.$r, $tctichluy);
        $ActiveSheet->setCellValue('E'.($r+1), $tctl);
        $ActiveSheet->setCellValue('F'.($r+2), round($diemtichluy/$tctichluy,2));
    }
    $ActiveSheet->setCellValue('E'.($r+3), 'TP.HCM, ngày '.date('d').' tháng '.date('m').' năm '.date('Y'));
//    $hoten = db_select('field_data_field_hoten', 'f')->fields('f', array('field_hoten_value'))->condition('f.entity_id', $user->uid)->execute()->fetchField();
//    if($hoten == '')
    $hoten = $user->name;
    $ActiveSheet->setCellValue('B'.($r+8), $hoten);
  
    $objPageSetup = new PHPExcel_Worksheet_PageSetup();
    $objPageSetup->setPaperSize(PHPExcel_Worksheet_PageSetup::PAPERSIZE_LETTER);
    $objPageSetup->setOrientation(PHPExcel_Worksheet_PageSetup::ORIENTATION_PORTRAIT);
    $objPageSetup->setPrintArea("A1:G".($r+8));
    $objPageSetup->setRowsToRepeatAtTopByStartAndEnd(6,6);
    $ActiveSheet->setPageSetup($objPageSetup);

    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
    ob_end_clean();
    $objWriter->save($path);
  
  //return;
  return $path;  
}

function qldiemhv_export_excel($mahv, $hocky=NULL) {
    global $user;
    if(!$mahv) return;
    $q = db_select('uit_thongtinhv', 'hv');
    $q->leftJoin('uit_nganh', 'n', 'n.nganh = hv.chuyennganh');
    $q->fields ('hv', array('hoten', 'ngaysinh', 'noisinh', 'gioitinh', 'lopsh', 'khoahoc','hedt','chuyennganh'));
    $q->fields('n', array('tennganh'));
    $q->condition('hv.mahv',$mahv);
    $hv = $q->execute()->fetch();
    
    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $filename = 'BangDiem_'.$mahv.'.xlsx';
    $path = variable_get('file_public_path', conf_path() . '/files') . '/exports/'.$filename;
    $templatefile = drupal_get_path('module', 'qldiem').'/templates/BangDiem.xlsx';
    $objPHPExcel = PHPExcel_IOFactory::load($templatefile);
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    $ActiveSheet->setCellValue('B2', 'Họ và tên: '.mb_strtoupper($hv->hoten))
              ->setCellValue('B3', "Mã số: $mahv            Khóa: $hv->khoahoc")
              ->setCellValue('B4', "Nơi sinh: $hv->noisinh")
              ->setCellValue('E2', 'Giới tính: '.($hv->gioitinh ? 'Nam':'Nữ'))
              ->setCellValue('I2', 'Ngày sinh: '.($hv->ngaysinh && strlen($hv->ngaysinh) == 10 ? date('d-m-Y',  strtotime($hv->ngaysinh)):$hv->ngaysinh))            
              ->setCellValue('E3', "Lớp: $hv->lopsh")
              ->setCellValue('E4', 'Ngành: Công nghệ thông tin - '.$hv->tennganh);

    $q = db_select('uit_diem', 'd');
//    $q->leftJoin('uit_ctdaotao', 'ctdt', 'd.mamh = ctdt.mamh and mh.nganh = d.nganh');
    $q->leftJoin('uit_ctdaotao', 'ctdt', 'd.mamh = ctdt.mamh');
    $q->leftJoin('uit_monhoc','mh','d.mamh=mh.mamh');
    $q->condition('d.mahv', $mahv);
    $q->condition('d.trangthai', 1);
    //$q->condition('d.trangthai', 0, '>');
    //$q->condition('d.trangthai', 5, '<>');
    if($hocky) $q->condition('d.hocky', $hocky);
    $q->orderBy('d.hocky','ASC')->orderBy('d.mamh','ASC');
    $q->fields ('d')->fields('mh')->fields('ctdt');
    $rows = $q->execute()->fetchAll();
    
    $ctdt = db_select('uit_ctdaotao','c')->fields('c',array('mamh','sotc'))
            ->condition('hedt', $hv->hedt)->condition('nganh', $hv->chuyennganh)
            ->execute()->fetchAllKeyed();

    $arrDiem = array();
    $arrCheck = array();
    foreach($rows as $row) {
//        if(!isset($ctdt[$row->mamh])) {
//            $mamhtd = MonHocTuongDuong($row->mamh, $hv->chuyennganh);
//            if(!$mamhtd) continue;
//            $cont = TRUE;
//            foreach ($mamhtd as $mamh2=>$value) {
//                if(isset($ctdt[$mamh2])) {
//                    if(isset($arrDiem[$mamh2])) {
//                        break;
//                    }
//                    $data = db_select('uit_ctdaotao','c')->fields('c',array('tenmh','sotc'))
//                            ->condition('hedt', $hv->hedt)->condition('nganh', $hv->chuyennganh)
//                            ->condition('mamh', $mamh2)
//                            ->execute()->fetch();
//                    if($data) {
//                        $row->mamh = $mamh2;
//                        $row->tenmh = $data->tenmh;
//                        $row->sotc = $data->sotc;
//                        $cont = FALSE;
//                        break;
//                    }
//                }
//            }
//            if($cont) continue;
//        }
        
        $mamh = $row->mamh;
        $diem = $row->diem;
        $sotc = $row->sotc;
        $ind1 = $row->hocky;
        $ind2 = $ind1.'-'.$mamh;
        
        $arrDiem[$ind1][] = array(
            'mamh' => $mamh,
            'tenmh' => $row->tenmh,
            'sotc' => $sotc,
            'diem' => $diem,
            'ghichu' => $row->ghichu,
        );
        if($row->trangthai == 1) {
            $arrCheck[$ind2] = 0;
        } else {
            $arrCheck[$ind2] = 1;
        }
    }
    //$styleArray = array('font' => array('bold' => true));
    //$style_row = array('fill' => array('type' => PHPExcel_Style_Fill::FILL_SOLID,'color' => array('rgb'=>'c0c0c0')));
    $styleBorder = array(
        'borders' => array(
          'inside'     => array(
            'style' => PHPExcel_Style_Border::BORDER_THIN,
          ),
          'outline'     => array(
            'style' => PHPExcel_Style_Border::BORDER_THIN,
          )
        )
    );
    
    $r = 7;
    $tctichluy = 0;
    $tctl = 0;
    $diemtichluy = 0;
    $n = 1;
    foreach ($arrDiem as $hknh => $hp) {
        $tongtc = 0;
        $tongdiem = 0;
        foreach ($hp as $mh) {
            $mamh = $mh['mamh'];
            $tenmh = $mh['tenmh'];
            $sotc = $mh['sotc'];
            $diem = $mh['diem'];
            //$ghichu = $mh['ghichu'];
            if($sotc > 0) {
                $tongtc += $sotc;
                $tongdiem += $diem*$sotc;
            }
            if($n <= 30) {
                $ActiveSheet
                    ->setCellValue('A'.$r, $n++)
                    ->setCellValue('B'.$r, $mamh)
                    ->setCellValue('C'.$r, $tenmh)
                    ->setCellValue('D'.$r, $sotc)
                    ->setCellValue('E'.$r, $diem);
            } else {
                if($n == 31) $r = 7;
                $ActiveSheet
                    ->setCellValue('G'.$r, $n++)
                    ->setCellValue('H'.$r, $mamh)
                    ->setCellValue('I'.$r, $tenmh)
                    ->setCellValue('J'.$r, $sotc)
                    ->setCellValue('K'.$r, $diem);
            }
            if($sotc > 0) {
                $tctichluy += $sotc;
                $diemtichluy += $diem*$sotc;
                if($diem >= 5) {
                    $tctl +=$sotc;
                }
            }
            $r++;
        }
  }
  
    if($n <= 30) {
        $col1 = 'A';
        $col2 = 'C';
        $col3 = 'D';
        $col4 = 'E';
    } else {
        $col1 = 'G';
        $col2 = 'I';
        $col3 = 'J';
        $col4 = 'K';
    }
    $dtb = round($diemtichluy/$tctichluy,2);
    $ActiveSheet->mergeCells("$col1$r:$col2$r");
    $ActiveSheet->getStyle("$col1$r")->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_RIGHT);
    $ActiveSheet->mergeCells("$col3$r:$col4$r");
    $ActiveSheet->setCellValue("$col1$r",'Số tín chỉ đã học');
    $ActiveSheet->setCellValue("$col3$r",$tctichluy);
    $ActiveSheet->getStyle("$col3$r")->getFont()->setBold(TRUE);
    $r++;
    $ActiveSheet->mergeCells("$col1$r:$col2$r");
    $ActiveSheet->getStyle("$col1$r")->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_RIGHT);
    $ActiveSheet->mergeCells("$col3$r:$col4$r");
    $ActiveSheet->setCellValue("$col1$r",'Số tín chỉ đã đạt');
    $ActiveSheet->setCellValue("$col3$r",$tctl);
    $ActiveSheet->getStyle("$col3$r")->getFont()->setBold(TRUE);
    $r++;
    $ActiveSheet->mergeCells("$col1$r:$col2$r");
    $ActiveSheet->getStyle("$col1$r")->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_RIGHT);
    $ActiveSheet->mergeCells("$col3$r:$col4$r");
    $ActiveSheet->setCellValue("$col1$r",'Điểm trung bình toàn khóa');
    $ActiveSheet->setCellValue("$col3$r",$dtb);
    $ActiveSheet->getStyle("$col3$r")->getFont()->setBold(TRUE);
    $r++;
    $ActiveSheet->mergeCells("$col1$r:$col4$r");
    $ActiveSheet->getStyle("$col1$r")->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
    $ActiveSheet->setCellValue("$col1$r",'Xếp loại: '.  Xeploai($dtb));
    $ActiveSheet->getStyle($col1.'7:'.$col4.$r)->applyFromArray($styleBorder);
    $ActiveSheet->getStyle("$col1$r")->getFont()->setBold(TRUE);
    
    $r += 2;
    $ActiveSheet->getStyle($col1.$r.':'.$col4.($r+10))->getFont()->setSize(10);
    
    $ActiveSheet->mergeCells("$col2$r:$col4$r");
    $ActiveSheet->getStyle("$col2$r")->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
    $ActiveSheet->setCellValue("$col2$r",'TP.HCM, ngày '.date('d').' tháng '.date('m').' năm '.date('Y'));
    $r++;
    $ActiveSheet->mergeCells("$col2$r:$col4$r");
    $ActiveSheet->getStyle("$col2$r")->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
    $ActiveSheet->setCellValue("$col2$r",'TL. HIỆU TRƯỞNG');
    $r++;
    $ActiveSheet->mergeCells("$col2$r:$col4$r");
    $ActiveSheet->getStyle("$col2$r")->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
    $ActiveSheet->setCellValue("$col2$r",'KT. TRƯỞNG PHÒNG ĐÀO TẠO ĐẠI HỌC');
    $r++;
    $ActiveSheet->setCellValue("$col1$r",'   Người lập bảng                       PHÓ TRƯỞNG PHÒNG');
    $r += 5;
//    $hoten = db_select('field_data_field_hoten', 'f')->fields('f', array('field_hoten_value'))->condition('f.entity_id', $user->uid)->execute()->fetchField();
//    if($hoten == '')
    $hoten = $user->name;
    $ActiveSheet->setCellValue("$col1$r",$hoten);
    $ActiveSheet->getStyle("$col3$r")->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_RIGHT);
    $ActiveSheet->setCellValue("$col3$r",'Đinh Khắc Quyền                 ');
  /*
  if($tctichluy > 0 && !$hocky) {
      $ActiveSheet->setCellValue('E'.$r, $tctichluy);
      $ActiveSheet->setCellValue('E'.($r+1), $tctl);
      $ActiveSheet->setCellValue('F'.($r+2), round($diemtichluy/$tctichluy,2));
  }
  $ActiveSheet->setCellValue('E'.($r+3), 'TP.HCM, ngày '.date('d').' tháng '.date('m').' năm '.date('Y'));
  $hoten = db_select('field_data_field_hoten', 'f')->fields('f', array('field_hoten_value'))->condition('f.entity_id', $user->uid)->execute()->fetchField();
  if($hoten == '') $hoten = $user->name;
  $ActiveSheet->setCellValue('B'.($r+8), $hoten);
  
    $objPageSetup = new PHPExcel_Worksheet_PageSetup();
    $objPageSetup->setPaperSize(PHPExcel_Worksheet_PageSetup::PAPERSIZE_LETTER);
    $objPageSetup->setOrientation(PHPExcel_Worksheet_PageSetup::ORIENTATION_PORTRAIT);
    $objPageSetup->setPrintArea("A1:G".($r+8));
    $objPageSetup->setRowsToRepeatAtTopByStartAndEnd(6,6);
    $ActiveSheet->setPageSetup($objPageSetup);
*/
  $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
  ob_end_clean();
  $objWriter->save($path);
  
  //return;
  return $path;  
}

function bangdiemth_xuatbangdiemtn($form, &$form_state) {
    if(!user_access('Quan ly diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    
    $arrID = array_filter($form_state['values']['qldiemhv_table']);
    if(count($arrID) == 0) {
        drupal_set_message('Cần phải chọn hv để xuất bảng điểm','error');
        drupal_exit();
    }
    if(count($arrID) == 1) {
        $filename = bangdiemth_xuatbangdiemTN_excel(key($arrID));
        if($filename){
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
    } else {
        set_time_limit(0);
        $path = variable_get('file_public_path', conf_path() . '/files') . '/exports/BangDiem_'.date('y-m-d_H-i-s').'.zip';
        $zip = new ZipArchive();
        // open archive 
        if ($zip->open($path, ZIPARCHIVE::CREATE) !== TRUE) {
            die ("Could not open archive");
        }
        foreach ($arrID as $mahv) {
            $fn = bangdiemth_xuatbangdiemTN_excel($mahv);
            if($fn) $zip->addFile ($fn, basename($fn));
        }
        $zip->close();
        if(filesize($path)){
          drupal_add_http_header('Pragma', 'public');
          drupal_add_http_header('Expires', '0');
          drupal_add_http_header('Cache-Control', 'must-revalidate, post-check=0, pre-check=0');
          drupal_add_http_header('Content-Type', 'application/application/x-zip');
          drupal_add_http_header('Content-Disposition', 'attachment; filename=' . basename($path) . ';');
          drupal_add_http_header('Content-Transfer-Encoding', 'binary');
          drupal_add_http_header('Content-Length', filesize($path));
          readfile($path);
          unlink($path);
          drupal_exit();
        }
    }
    
}

function bangdiemth_xuatbangdiemTN_excel($mahv) {
    if(!$mahv) return;
    
    $q = db_select('uit_thongtinhv', 'hv');
    $q->leftJoin('uit_sinhvien_totnghiep', 'tn', 'tn.mahv = hv.mahv');
    $q->leftJoin('uit_nganh', 'n', 'n.nganh = hv.chuyennganh');
    $q->leftJoin('uit_vanbang', 'vb', 'vb.mahv = hv.mahv');
    $q->fields ('hv');
    $q->fields ('n', array('tennganh','tgdt'));
    $q->fields ('vb', array('sohieu', 'ngaycapvb'));
    $q->fields ('tn', array('noisinh', 'xeploai'));
    
    $q->condition('hv.mahv',$mahv);
    $hv = $q->execute()->fetch();
    if(!$hv) return "Không tìm thấy thông tin sinh viên này.";
    //$arrHeDT = array('CN1'=>'Cử nhân 1','CN2'=>'Cừ nhân 2','HC'=>'Hoàn chỉnh đại học');
    
    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $filename = 'BangDiem_TN_'.$mahv.'.xlsx';
    $path = variable_get('file_public_path', conf_path() . '/files') . '/exports/'.$filename;
    $templatefile = drupal_get_path('module', 'qldiem').'/templates/MauBangDiemTN.xlsx';
    $objPHPExcel = PHPExcel_IOFactory::load($templatefile);
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    
    //$objPHPExcel->getSheet(0)->setTitle($mahv);
    $ActiveSheet
            ->setCellValue('D2', $hv->hoten)
            ->setCellValue('D3', (strlen($hv->ngaysinh) == 10 ? date('d-m-Y',  strtotime($hv->ngaysinh)):$hv->ngaysinh))
            ->setCellValueExplicit('D4', $hv->mahv, PHPExcel_Cell_DataType::TYPE_STRING)
            ->setCellValue('D6', $hv->tennganh)
            ->setCellValue('F2', ($hv->gioitinh == 1 ? 'Nam':'Nữ'))
            ->setCellValue('F3', ($hv->noisinh != '' ? $hv->noisinh:''))
            ->setCellValue('F4', $hv->khoahoc.'-'.($hv->khoahoc + (int)$hv->tgdt));
    
    $q = db_select('uit_diem', 'd');
    $q->leftJoin('uit_ctdaotao', 'mh', 'd.mamh = mh.mamh and mh.nganh = d.nganh');
    $q->fields ('d')->fields('mh',array('tenmh'));
    $q->condition('d.mahv', $mahv);
    $q->condition('d.diem', 5, '>=');
    $q->condition('d.trangthai', 1);
    $q->orderBy('d.mamh','ASC');
            
    $rows = $q->execute()->fetchAll();
    if(!$rows) {
        drupal_set_message('hv '.$mahv.' không có thông tin điểm','error');
        return;
    }
    $ctdt = db_select('uit_ctdaotao','c')->fields('c',array('mamh','sotc'))
            ->condition('hedt', $hv->hedt)->condition('nganh', $hv->chuyennganh)
            ->execute()->fetchAllKeyed();
    $arrInfos = array();
    foreach ($rows as $infos) {
        $sotc = $infos->sotc;
        if($sotc > 0){ //chỉ lấy những môn có TC > 0
            $arrInfos[$infos->mamh] = array(
                'tenmh'=>$infos->tenmh, 
                'diem'=>$infos->diem,
                'sotc'=>$sotc,
                'ghichu'=>$infos->ghichu);
        }
    }
    
    $r = 9;
    $stt = 1;
    $tctichluy = 0;
    $diemtichluy = 0;
    
    $ActiveSheet->insertNewRowBefore($r+1, count($arrInfos)- 2);
    foreach ($arrInfos as $mamh=>$info) {
        if(!isset($ctdt[$mamh])) {
            $mamhtd = MonHocTuongDuong($mamh, $hv->chuyennganh);
            if(!$mamhtd) continue;
            $cont = TRUE;
            foreach ($mamhtd as $mamh2=>$value) {
                if(isset($ctdt[$mamh2])) {
                    if(isset($arrInfos[$mamh2])) {
                        break;
                    }
                    $data = db_select('uit_ctdaotao','c')->fields('c',array('tenmh','sotc'))
                            ->condition('hedt', $hv->hedt)->condition('nganh', $hv->chuyennganh)
                            ->condition('mamh', $mamh2)
                            ->execute()->fetch();
                    if($data) {
                        $mamh = $mamh2;
                        $info['tenmh'] = $data->tenmh;
                        $info['sotc'] = $data->sotc;
                        $cont = FALSE;
                        break;
                    }
                }
            }
            if($cont) continue;
        }
        
        $tctichluy += $info['sotc'];
        $diemtichluy += $info['diem']*$info['sotc'];
        $ActiveSheet
        ->setCellValue('B'.$r, $stt++)
        ->setCellValue('C'.$r, $mamh)
        ->setCellValue('D'.$r, $info['tenmh'])
        ->setCellValue('E'.$r, $info['sotc'])
        ->setCellValue('F'.$r, $info['diem']);
        //->setCellValue('G'.$r, $info['ghichu']);
        $r++;
    }
    $r++;
    if($tctichluy > 0) {        
        $ActiveSheet->setCellValue('E'.$r, $tctichluy);
        $ActiveSheet->setCellValue('E'.($r+1), round($diemtichluy/$tctichluy,2));
        //$ActiveSheet->setCellValue('E'.($r+2), ($hv->diemrl ? diemth_xeploai_drl($hv->diemrl):''));
        $ActiveSheet->setCellValue('E'.($r+2), ($hv->xeploai ? $hv->xeploai:''));
    }
    
    $ActiveSheet->setCellValue('E'.($r+3), 'TP. Hồ Chí Minh, ngày '.date('d').' tháng '.date('m').' năm '.date('Y'));
    
    $objPageSetup = new PHPExcel_Worksheet_PageSetup();
    $objPageSetup->setPaperSize(PHPExcel_Worksheet_PageSetup::PAPERSIZE_LETTER);
    $objPageSetup->setOrientation(PHPExcel_Worksheet_PageSetup::ORIENTATION_PORTRAIT);
    $objPageSetup->setPrintArea("A1:G".($r+8));
    $objPageSetup->setRowsToRepeatAtTopByStartAndEnd(9,9);
    $ActiveSheet->setPageSetup($objPageSetup);
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
    ob_end_clean();
    $objWriter->save($path);
  
  return $path;
}

function qldiemhv_build_filter_query(SelectQueryInterface $query) {
    $filter_data = isset($_SESSION['qldiemhv_filter']) ? $_SESSION['qldiemhv_filter'] : array();
    foreach ($filter_data as $key => $value) {
        switch ($key) {
            case 'timkiem':
            case 'sodong':
                break;
            case 'mahv':
            case 'lopsh':
                $arrmahv = explode(',', $value);
                $query->condition('hv.'.$key, $arrmahv, 'IN'); 
                break;
            case 'tinhtrang':
            case 'khoahoc':
                if(!array_key_exists('',$value)) $query->condition('hv.'.$key, $value, 'IN'); 
                break;
                
            default:
                $query->condition('hv.'.$key, $value);
        }
    }
}

function qldiemhv_set_filters(&$form_state) {
    $filters = qldiemhv_filters();
    //Apply every filter that has a choice selected other than 'any'.
    foreach ($filters as $filter => $options) {
      if (isset($form_state['values'][$filter])) {
        if( $form_state['values'][$filter] != '') {
          $_SESSION['qldiemhv_filter'][$filter] = $form_state['values'][$filter];
        }
        else {
          unset($_SESSION['qldiemhv_filter'][$filter]);
        }
      } 
    }
}

function qldiemhv_filters() {
  $filters = array();  
  $session = isset($_SESSION['qldiemhv_filter']) ? $_SESSION['qldiemhv_filter'] : array();
  if(!isset($_SESSION['qldiemhv_filter']['sodong'])) $_SESSION['qldiemhv_filter']['sodong'] = 50;
  $mahv = !empty($session['mahv']) ? $session['mahv'] : NULL;
  //$coso = !empty($session['coso']) ? $session['coso'] : NULL;
  $hedt = !empty($session['hedt']) ? $session['hedt'] : NULL;
  //$lopsh = !empty($session['lopsh']) ? $session['lopsh'] : NULL;
//  $chuyennganh = !empty($session['chuyennganh']) ? $session['chuyennganh'] : NULL;
  //$ctdt=!empty($session['ctdt'])? $session['ctdt'] : NULL;
  $tinhtrang = !empty($session['tinhtrang']) ? $session['tinhtrang'] : array('');
  $khoahoc = !empty($session['khoahoc']) ? $session['khoahoc'] : array('');
  $sodong = !empty($session['sodong']) ? $session['sodong'] : 50;
  
  $arrKhoaHoc = array('' => t('Tất cả'));
  for($i=2001;$i<=date('Y');$i++) $arrKhoaHoc[$i] = $i;
     
    $filters['mahv'] = array(
        'type' => 'textfield',
        'title' => t('Danh sách MSHV:'),
        'size' => 80,
        'maxlength' => 1000,
        'default_value' => array($mahv),
        'attributes' => array('title' =>t('Mỗi MSHV cách nhau bởi dấu phẩy (,)')),
    );
    $filters['hedt'] = array(
      'type' => 'select',
      'title' => t('Hệ ĐT'),
      'options' => (array('' => t('Tất cả')) + db_select('uit_hedt', 'k')->orderBy('kyhieu')->fields('k', array('kyhieu', 'kyhieu'))->execute()->fetchAllKeyed()),
      'default_value' => array($hedt),
    );
    $filters['tinhtrang'] = array(
      'type' => 'select',
      'multiple' => TRUE,
      'title' => t('Tình trạng'),
      'options' => (array('' => t('Tất cả')) + db_select('uit_tinhtrang', 'k')->orderBy('ten')->fields('k', array('id', 'ten'))->condition('k.doituong','SV')->execute()->fetchAllKeyed()),
      'default_value' => $tinhtrang,
    );
    $filters['khoahoc'] = array(
      'type' => 'select',
      'multiple' => TRUE,
      'title' => t('Khóa'),
      'options' => (array('' => t('Tất cả')) + db_select('uit_thongtinhv', 'k')->distinct()->orderBy('khoahoc')->fields('k', array('khoahoc', 'khoahoc'))->execute()->fetchAllKeyed()),
      'default_value' => $khoahoc,
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
          'callback' => 'ajax_filter_qldiemhv_callback',
          'wrapper' => 'table_qldiemhv',
        ),
    );
  
  return $filters;
}

function qldiemhv_filter_form() {
    $form = array();
    $filters = qldiemhv_filters();
    foreach ($filters as $key => $filter) {
        $arr = array();
        foreach ($filter as $key2 => $value2) {
            $arr['#'.$key2] = $value2;
        }
        $form['filters'][$key] = $arr;
    }
    return $form;
}

function ajax_filter_qldiemhv_callback($form, $form_state) {
  return $form['qldiemhv_data'];
}
