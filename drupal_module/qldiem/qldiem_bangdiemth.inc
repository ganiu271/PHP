<?php
function qldiem_form_bangdiemth($form, &$form_state) {
    if(!user_access('Quan ly diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
         
    $html = '<center><h1>QUẢN LÝ BẢNG ĐIỂM TỔNG HỢP</h1></center>';
    $form['markup'] = array('#markup' => $html);
    $form['table_filter_wraper'] = array('#type' => 'fieldset', '#title' => t('Chọn điều kiện lọc'), '#attributes' => array('class' => array('container-inline')));
    bangdiemth_set_filters($form_state);
    $form['table_filter_wraper'][] = bangdiemth_filter_form();
    
    $form['bangdiemth_data'] = array(
        '#prefix' => '<div id="table_bangdiemth">',
        '#suffix' => '</div>',);
    $form['bangdiemth_data']['bangdiemth_count'] = array('#markup' => '<strong>Tổng số hv: '.  number_format(bangdiemth_get_count_dssinhvien()).'</strong><br><br>');
    $form['bangdiemth_data']['xuatbangdiemth0'] = array(
      '#type' => 'button',
      '#value' => t('Xuất bảng điểm TH xét thi TN'),
      '#attributes' => array('title' =>'Xuất bảng điểm tổng hợp xét điều kiện thi tốt nghiệp'),
      '#executes_submit_callback' => TRUE,
      '#submit' => array('bangdiemth_xuatbangdiemth'),
    );
    $form['bangdiemth_data']['xuatbangdiemthxetTN0'] = array(
      '#type' => 'button',
      '#value' => t('Xuất bảng điểm TH xét TN'),
      '#attributes' => array('title' =>'Xuất bảng điểm tổng hợp xét điều kiện tốt nghiệp'),
      '#executes_submit_callback' => TRUE,
      '#submit' => array('bangdiemth_xuatbangdiemth_xettn'),
    );
    $form['bangdiemth_data']['bangdiemth_table'] = bangdiemth_get_table_dssinhvien();
    $tmp_q = $_GET['q']; 
    $_GET['q'] = 'admin/qldiem/bangdiemth';
    $form['bangdiemth_data']['pager'] = array('#markup' => theme('pager'));
    $_GET['q'] = $tmp_q;
    
    $form['xuatbangdiemth'] = array(
      '#type' => 'button',
      '#value' => t('Xuất bảng điểm TH xét thi TN'),
      '#attributes' => array('title' =>'Xuất bảng điểm tổng hợp xét điều kiện thi tốt nghiệp'),
      '#executes_submit_callback' => TRUE,
      '#submit' => array('bangdiemth_xuatbangdiemth'),
    );
    $form['bangdiemth_data']['xuatbangdiemthxetTN'] = array(
      '#type' => 'button',
      '#value' => t('Xuất bảng điểm TH xét TN'),
      '#attributes' => array('title' =>'Xuất bảng điểm tổng hợp xét điều kiện tốt nghiệp'),
      '#executes_submit_callback' => TRUE,
      '#submit' => array('bangdiemth_xuatbangdiemth_xettn'),
    );
    /*
    $arrHocPhan = array();
    for($i=1;$i<16;$i++) $arrHocPhan[$i] = $i;
    $form['bangdiemth_data']['xuatbangdiemth_hocky_wraper'] = array('#type' => 'fieldset', '#title' => t('Xuất bảng điểm tổng hợp theo học phần'), '#attributes' => array('class' => array('container-inline')));
    $form['bangdiemth_data']['xuatbangdiemth_hocky_wraper']['chonhocky'] = array(
        '#type' => 'select',
        '#title' => t('Học phần'),
        '#options' => $arrHocPhan,
        '#default_value' => '1',
    );
    
    $form['bangdiemth_data']['xuatbangdiemth_hocky_wraper']['xuatbangdiemhk'] = array(
        '#type' => 'button',
        '#value' => t('Xuất bảng điểm TH theo HK'),
        '#attributes' => array('title' =>'Xuất bảng điểm các sinh viên được chọn theo học kỳ, năm học'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('bangdiemth_xuatbangdiemhk'),
    );
    */
    $form['message'] = array(
      '#markup' => '<div id="view_message" class="statusmsg"></div>',
    );
    
    return $form;
}

function bangdiemth_xuatbangdiemhk($form, &$form_state) {
    if(!user_access('Quan ly diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $hocky = (int)$form_state['values']['chonhocky'];
    $namhoc = (int)$form_state['values']['chonnamhoc'];
    
    //if($hocky <= 0 || $hocky > 3) $hocky = NULL;
    if($namhoc < 2001 || $namhoc > date('Y')) $namhoc = NULL;
    
    $a_hv = array_filter($form_state['values']['bangdiemth_table']);
    if(!$a_hv) {
        $query = db_select('uit_thongtinhv', 'hv');
        $query->fields('hv', array('mahv', 'mahv'));
        $query->orderBy('hv.mahv', 'ASC');
        bangdiemth_build_filter_query( $query);
        $a_hv = $query->execute()->fetchAllKeyed();
    }
    
    $khoahoc = $form_state['values']['khoahoc'];
    if(!is_numeric($khoahoc)) {
        drupal_set_message('Khóa học không hợp lệ','error');
        return;
    }
    $khoa = $form_state['values']['khoa'];
    if($khoa == '') {
        drupal_set_message('Phải chọn Khoa để xuất bảng điểm tổng hợp','error');
        return;
    }
    $hedt = $form_state['values']['hedt'];
    if($hedt == '') {
        drupal_set_message('Phải chọn hệ đào tạo để xuất bảng điểm tổng hợp','error');
        return;
    }
    
    $filename = bangdiemth_xuatbangdiemhk_excel($a_hv, $khoahoc, $khoa, $hocky, $namhoc, $hedt);
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
    
}

function bangdiemth_get_count_dssinhvien() {
    $query = db_select('uit_thongtinhv', 'hv');
    $query->addExpression('count(mahv)', 'tonghv');
    bangdiemth_build_filter_query( $query);
    return $query->execute()->fetchField();
}

function bangdiemth_get_table_dssinhvien() {
    //$header = array('STT', 'MShv', 'Họ tên', 'Lớp', 'Khoa', 'Hệ đào tạo', 'Tình trạng');
    $header = array(
        array('data' => t('MShv'), 'field' => 'mahv'),
        array('data' => t('Họ tên'), 'field' => 'hoten'),
        array('data' => t('Lớp'), 'field' => 'lopsh'),
//        array('data' => t('Cơ sở'), 'field' => 'tenvt'),
        array('data' => t('Hệ đào tạo'), 'field' => 'hedt'),
        array('data' => t('CTĐT'), 'field' => 'chuyennganh'),
        array('data' => t('Tình trạng'), 'field' => 'ten')
    );
    $query = db_select('uit_thongtinhv', 'hv')->extend('PagerDefault')->limit($_SESSION['bangdiemth_filter']['sodong'])->extend('TableSort')->orderByHeader($header);
    $query->join('uit_tinhtrang', 'tt', 'tt.id = hv.tinhtrang');
//    $query->join('uit_coso', 'cs', 'cs.macs = hv.coso');
    $query->fields('hv', array('mahv', 'hoten', 'lopsh', 'hedt','chuyennganh'));
    $query->fields('tt', array('ten'));
//    $query->fields('cs', array('tenvt'));
    $query->orderBy('hv.mahv', 'ASC');
    bangdiemth_build_filter_query( $query);
    $rows = $query->execute();
    $options = array();
    //$i = 1;
    foreach ($rows as $row) {
        $options[$row->mahv] = array(
            l($row->mahv,'admin/qlsinhvien/xem/'.$row->mahv,array('attributes' => array('target' => '_blank'))),
            $row->hoten,
            $row->lopsh,
//            $row->tenvt,
            $row->hedt,
            $row->chuyennganh,
            $row->ten
        );
    }
    
    $form = array(
        '#title' => 'Table',
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        //'#default_value' => drupal_map_assoc($default),
        '#empty' => t('Không tìm thấy sinh viên nào.'),
        //'#sticky' => FALSE,
        //'#prefix' => '<div class="table_bangdiemth"  id="table_bangdiemth">',
        //'#suffix' => '</div>'
    );
    return $form;
}

function bangdiemth_xuatbangdiemth_xettn($form, $form_state) {
    if(!user_access('Quan ly diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $a_hv = array_filter($form_state['values']['bangdiemth_table']);
    if(!$a_hv) {
        $query = db_select('uit_thongtinhv', 'hv');
        $query->fields('hv', array('mahv', 'mahv'));
        $query->orderBy('hv.mahv', 'ASC');
        bangdiemth_build_filter_query( $query);
        $a_hv = $query->execute()->fetchAllKeyed();
    }
    
    $khoahoc = $form_state['values']['khoahoc'];
    if($khoahoc == 2000) {
        drupal_set_message('Phải chọn khóa học','error');
        return;
    }
    $coso = $form_state['values']['coso'];
    if($coso == '00') {
        drupal_set_message('Phải chọn cơ sở','error');
        return;
    }
    $chuyennganh = $form_state['values']['chuyennganh'];
    if($chuyennganh == '000') {
        drupal_set_message('Phải chọn CTĐT','error');
        return;
    }
    
    $filename = bangdiemth_xuatbangdiemth_excel($a_hv, $khoahoc, $coso, $chuyennganh, true);
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
}

function bangdiemth_xuatbangdiemth($form, $form_state) {
    if(!user_access('Quan ly diem') ) {
        drupal_set_message('Bạn không được phép truy cập menu này','error');
        return;
    }
    $a_hv = array_filter($form_state['values']['bangdiemth_table']);
    if(!$a_hv) {
        $query = db_select('uit_thongtinhv', 'hv');
        $query->fields('hv', array('mahv', 'mahv'));
        $query->orderBy('hv.mahv', 'ASC');
        bangdiemth_build_filter_query( $query);
        $a_hv = $query->execute()->fetchAllKeyed();
    }
    
    $khoahoc = $form_state['values']['khoahoc'];
    if($khoahoc == 2000) {
        drupal_set_message('Phải chọn khóa học','error');
        return;
    }
    $coso = $form_state['values']['coso'];
    if($coso == '00') {
        drupal_set_message('Phải chọn cơ sở','error');
        return;
    }
    $chuyennganh = $form_state['values']['chuyennganh'];
    if($chuyennganh == '000') {
        drupal_set_message('Phải chọn CTĐT','error');
        return;
    }
    
    $filename = bangdiemth_xuatbangdiemth_excel($a_hv, $khoahoc, $coso, $chuyennganh, false);
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
}

function bangdiemth_xuatbangdiemth_excel($a_hv, $khoahoc, $coso, $chuyennganh, $xettn = false) {
    $q = db_select('uit_diem', 'd');
    //$q->join('uit_thongtinhv','hv','hv.mahv = d.mahv');
    $q->leftJoin('uit_ctdaotao','ct','ct.mamh = d.mamh and ct.nganh = d.nganh');
    $q->fields('d',array('mahv','mamh','hocky','diem','lanthi'));
    $q->fields('ct',array('tenmh','sotc'));
    $q->condition('d.mahv', $a_hv, 'IN');
    $q->condition('d.trangthai', 1);
    //$q->condition('hv.coso', $coso);
    //$q->condition('hv.khoahoc', $khoahoc);
    //$q->condition('hv.chuyennganh', $chuyennganh);
    $q->orderBy('d.mahv');
    $q->orderBy('d.hocky');
    $q->orderBy('d.mamh');
    $rows = $q->execute()->fetchAll();
    if(!$rows) {
        drupal_set_message('Không có thông tin điểm để xuất bảng điểm tổng hợp','error');
        return;
    }
    $arr_ctdt = db_select('uit_ctdaotao','c')->fields('c',array('mamh','sotc'))
            ->condition('nganh', $chuyennganh)
            ->execute()->fetchAllKeyed();
    // Tính học kỳ cuối
    $q = db_select('uit_ctdaotao','t');
    $q->addExpression('max(hocky)',maxhocky);
    $q->condition('nganh',$chuyennganh)->condition('hocky',20,'<>');
    $sohocky = $q->execute()->fetchField();
    $arrMonHocBB = array('TH107','TH103','TH108','TH102','TH112','TH502');
    
    $ctdt = array();
    $arrhv = array();
    $arrDiem = array();
    $mahv = '';
    foreach ($rows as $row) {
        if($row->mahv != $mahv) {
            $mahv = $row->mahv;
            $hv = db_select('uit_thongtinhv', 'hv')->fields('hv', array('hoten', 'gioitinh','ngaysinh','noisinh','lopsh','chuyennganh'))->condition('mahv', $mahv)->execute()->fetch();
            $arrhv[$mahv] = array(
                'hoten' => $hv->hoten,
                'gioitinh' => ($hv->gioitinh == 1?'Nam':'Nữ'),
                'ngaysinh' => (strlen($hv->ngaysinh)==10 ? date('d-m-Y',  strtotime($hv->ngaysinh)):$hv->ngaysinh),
                'noisinh' => $hv->noisinh,
                'lopsh' => $hv->lopsh,
            );
        }
        if(!isset($arr_ctdt[$row->mamh])) {
            $mamhtd = MonHocTuongDuong($row->mamh, $hv->chuyennganh);
            if(!$mamhtd) continue;
            $cont = TRUE;
            foreach ($mamhtd as $mamh2=>$value) {
                if(isset($arr_ctdt[$mamh2])) {
                    $data = db_select('uit_ctdaotao','c')->fields('c',array('tenmh','sotc'))
                            ->condition('nganh', $hv->chuyennganh)
                            ->condition('mamh', $mamh2)
                            ->execute()->fetch();
                    if($data) {
                        $row->mamh = $mamh2;
                        $row->tenmh = $data->tenmh;
                        $row->sotc = $data->sotc;
                        $cont = FALSE;
                        break;
                    }
                }
            }
            if($cont) continue;
        }
        if(!isset($ctdt[$row->hocky][$row->mamh])) {
            //$arrPos[$row->mamh] = $col++;
            $ctdt[$row->hocky][$row->mamh] = array(
                'tenmh' => $row->tenmh,
                'sotc' => $row->sotc
            );
        }
        $arrDiem[$mahv][$row->mamh] = array('diem' => $row->diem, 'sotc' => $row->sotc, 'lanthi' => $row->lanthi);
    }
    $tencoso = db_select('uit_coso','t')->fields('t',array('tencs'))->condition('macs',$coso)->execute()->fetchField();
    $hedt = db_select('uit_nganh','t')->fields('t',array('hedt'))->condition('nganh',$chuyennganh)->execute()->fetchField();
    
    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    $filename = 'BangDiemTH_'.$coso.'_'.$khoahoc.'_'.$chuyennganh.'.xlsx';
    $path = variable_get('file_public_path', conf_path() . '/files') . '/exports/'.$filename;
    $templatefile = drupal_get_path('module', 'qldiem').'/templates/Mau_BDTH.xlsx';
    $objPHPExcel = PHPExcel_IOFactory::load($templatefile);
    
    //$styleBold = array('font' => array('bold' => true));
    $style_cell = array('fill' => array('type' => PHPExcel_Style_Fill::FILL_SOLID,'color' => array('rgb'=>'ffaed7')));
    $style_cell2 = array('fill' => array('type' => PHPExcel_Style_Fill::FILL_SOLID,'color' => array('rgb'=>'999966')));
    $style_cell3 = array('fill' => array('type' => PHPExcel_Style_Fill::FILL_SOLID,'color' => array('rgb'=>'cc9966')));
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
    
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    $ActiveSheet->setCellValue('A2','Cơ sở: '.$tencoso);
    $ActiveSheet->setCellValue('A3',"Bảng điểm hệ $hedt - Khóa $khoahoc-".($khoahoc+4));
    
    $arrPos = array();
    $col = $startcol = 'H';
    $colhp9 = $colhp10 = $startcol;
    $r = 4;
    foreach ($ctdt as $hocky => $mh) {
        if($hocky == 20) {
            $ActiveSheet->setCellValue($col.$r,'HỌC PHẦN TỰ CHỌN');
        } else {
            $ActiveSheet->setCellValue($col.$r,'HỌC PHẦN '.$hocky);
        }
        $ActiveSheet->getStyle($col.$r)->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
        if($hocky%2 == 0) {
            $ActiveSheet->getStyle($col.$r)->applyFromArray( $style_cell2 );
        } else {
            $ActiveSheet->getStyle($col.$r)->applyFromArray( $style_cell3 );
        }
        $col2 = $col;
        foreach ($mh as $mamh => $monhoc) {
            $ActiveSheet->setCellValue($col.($r+1),$monhoc['sotc']);
            $ActiveSheet->setCellValue($col.($r+2),$monhoc['tenmh']);
            $ActiveSheet->setCellValue($col.($r+3),$mamh);
            if(in_array($mamh,$arrMonHocBB)) {
                $ActiveSheet->getStyle($col.($r+1).':'.$col.($r+3))->applyFromArray( $style_cell );
            }
            $colhp10 = $col;
            if($hocky < $sohocky) $colhp9 = $col;
            $arrPos[$mamh] = $col++;
        }
        $ActiveSheet->mergeCells($col2.$r.':'.$colhp10.$r);
    }
    $ActiveSheet->setCellValue($col.'5','=SUM('.$startcol.'5:'.$colhp10.'5)');
    
    $ActiveSheet->mergeCells($col.'6:'.$col.'7');
    $ActiveSheet->setCellValue($col.'6','SỐ MÔN NỢ ĐẾN HỌC PHẦN 9');
    $arrPos['NoHP9'] = $col;
    $col++;
    $ActiveSheet->mergeCells($col.'6:'.$col.'7');
    $ActiveSheet->setCellValue($col.'6','ĐIỂM TRUNG BÌNH ĐẾN HỌC PHẦN 9');
    $arrPos['DTBHP9'] = $col;
    $col++;
    $ActiveSheet->mergeCells($col.'6:'.$col.'7');
    $ActiveSheet->setCellValue($col.'6','SỐ MÔN NỢ TRÙNG MÔN THI TỐT NGHIỆP');
    $arrPos['NoMonBB'] = $col;
    $col++;
    $ActiveSheet->mergeCells($col.'6:'.$col.'7');
    $ActiveSheet->setCellValue($col.'6','SỐ MÔN NỢ TOÀN KHÓA');
    $arrPos['NoToanKhoa'] = $col;
    $col++;
    
    $ActiveSheet->mergeCells($col.'6:'.$col.'7');
    $ActiveSheet->setCellValue($col.'6','SỐ TÍN CHỈ TÍCH LŨY TOÀN KHÓA');
    $arrPos['TCTL'] = $col;
    $col++;
    
    $ActiveSheet->mergeCells($col.'6:'.$col.'7');
    $ActiveSheet->setCellValue($col.'6','ĐIỂM TRUNG BÌNH TOÀN KHÓA');
    $arrPos['DTB'] = $col;
    $col++;
    
    $ActiveSheet->mergeCells($col.'6:'.$col.'7');
    $ActiveSheet->setCellValue($col.'6','XẾP LOẠI KẾT QUẢ HỌC TẬP');
    $arrPos['XepLoai'] = $col;
    $col++;
    
    $ActiveSheet->mergeCells($col.'6:'.$col.'7');
    $ActiveSheet->setCellValue($col.'6','GHI CHÚ');
    $arrPos['GhiChu'] = $col;
    
    $r = 8;
    $stt = 1;
    $NoMonBB = 0;
    
    foreach ($arrhv as $mahv => $thongtinhv) {
        $col = 'A';
        $ActiveSheet->setCellValue($col++.$r,$stt);
        $ActiveSheet->setCellValueExplicit($col++.$r,$mahv, PHPExcel_Cell_DataType::TYPE_STRING);
        $ActiveSheet->setCellValue($col++.$r,$thongtinhv['hoten']);
        $ActiveSheet->setCellValue($col++.$r,$thongtinhv['gioitinh']);
        $ActiveSheet->setCellValueExplicit($col++.$r,$thongtinhv['ngaysinh'], PHPExcel_Cell_DataType::TYPE_STRING);
        $ActiveSheet->setCellValue($col++.$r,$thongtinhv['noisinh']);
        $ActiveSheet->setCellValue($col++.$r,$thongtinhv['lopsh']);
        
        $tctichluy = 0;
        $tctl = 0;
        $diemtl = 0;
        //$diemtl2 = 0;
        foreach ($arrDiem[$mahv] as $mamh => $thongtinmh) {
            $ActiveSheet->setCellValue($arrPos[$mamh].$r,$thongtinmh['diem']);
            if($thongtinmh['diem'] < 5) {
                if(in_array($mamh,$arrMonHocBB)) $NoMonBB++;
                $ActiveSheet->getStyle($arrPos[$mamh].$r)->applyFromArray( $style_cell );
            }
            $tctichluy += $thongtinmh['sotc'];
            $diemtl += $thongtinmh['diem']*$thongtinmh['sotc'];
            if($thongtinmh['diem'] >= 5) {
                $tctl += $thongtinmh['sotc'];
                //$diemtl2 += $thongtinmh['diem']*$thongtinmh['sotc'];
            }
            if($thongtinmh['lanthi'] == 2) $ActiveSheet->getStyle($arrPos[$mamh].$r)->getFont()->getColor()->setRGB('0000ff');
            if($thongtinmh['lanthi'] >= 3) $ActiveSheet->getStyle($arrPos[$mamh].$r)->getFont()->getColor()->setRGB('ff0000');
        }
        $dtb = round($diemtl/$tctichluy, 2);
        // Set công thức các cột tổng hợp
        $ActiveSheet->setCellValue($arrPos['NoHP9'].$r,'=COUNTIF('.$startcol.$r.':'.$colhp9.$r.',"<5")');
        $ActiveSheet->setCellValue($arrPos['DTBHP9'].$r,'=ROUND(SUMPRODUCT('.$startcol.$r.':'.$colhp9.$r.','.$startcol.'5:'.$colhp9.'5)/SUM('.$startcol.'5:'.$colhp9.'5),2)');
        $ActiveSheet->setCellValue($arrPos['NoMonBB'].$r,$NoMonBB);
        $ActiveSheet->setCellValue($arrPos['NoToanKhoa'].$r,'=COUNTIF('.$startcol.$r.':'.$colhp10.$r.',"<5")');
        $ActiveSheet->setCellValue($arrPos['TCTL'].$r,$tctl);
        //$ActiveSheet->setCellValue($arrPos['DTB'].$r,'=ROUND(SUMPRODUCT('.$startcol.$r.':'.$colhp10.$r.','.$startcol.'5:'.$colhp10.'5)/SUM('.$startcol.'5:'.$colhp10.'5),2)');
        $ActiveSheet->setCellValue($arrPos['DTB'].$r,$dtb);
        $ActiveSheet->setCellValue($arrPos['XepLoai'].$r,  Xeploai($dtb));
        if($xettn) {
            if(!XetDieuKienTN($mahv)) {
                $ActiveSheet->setCellValue($arrPos['GhiChu'].$r,'Không đủ ĐK TN');
            }
        } else {
            if(!XetDieuKienThiTN($mahv, $sohocky)) {
                $ActiveSheet->setCellValue($arrPos['GhiChu'].$r,'Không đủ ĐK thi TN');
            }
        }
        
        $stt++;
        $r++;
    }
    $ActiveSheet->getStyle('A5:'.$arrPos['GhiChu'].($r-1))->applyFromArray($styleBorder);
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
    ob_end_clean();
    $objWriter->save($path);

    return $path;  
}

function bangdiemth_xuatbangdiemhk_excel($a_hv, $khoahoc, $khoa, $hocky, $namhoc, $hedt) {
    $q = db_select('uit_diem', 'd')->distinct();
    $q->join('uit_monhoc', 'mh', 'mh.mamh = d.mamh');
    $q->fields('mh', array('tenmh','mamh'));
    $q->fields('d', array('sotc'));
    $q->condition('d.mahv', $a_hv, 'IN');
    $q->condition('d.trangthai', 1);
    $q->condition('d.diem', 0, '>');
    if($hocky) {
        $q->condition('d.hocky', $hocky);
    }
    $q->condition('d.namhoc', $namhoc);
    $q->orderBy('mh.mamh');
    
    $q->orderBy('mh.mamh');
    $a_mh = $q->execute()->fetchAll();
    if(!$a_mh) {
        drupal_set_message('Không có thông tin điểm để xuất.','error');
        return;
    }
    
    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';
    if($hocky) {
        $filename = 'BangDiemTH_'.$hocky.'_'.$namhoc.'.xlsx';
    } else {
        $filename = 'BangDiemTH_'.$namhoc.'.xlsx';
    }
    
    $path = variable_get('file_public_path', conf_path() . '/files') . '/exports/'.$filename;
    $templatefile = variable_get('file_public_path', conf_path() . '/files') . '/templates/qldiem/BangDiemTH_HocKy.xlsx';
    $objPHPExcel = PHPExcel_IOFactory::load($templatefile);
    $style_cell = array('fill' => array('type' => PHPExcel_Style_Fill::FILL_SOLID,'color' => array('rgb'=>'ffaed7')));
    //$style_cell3 = array('fill' => array('type' => PHPExcel_Style_Fill::FILL_SOLID,'color' => array('rgb'=>'cc9966')));
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    if($hocky) {
        $ActiveSheet->setCellValue('C4','BẢNG ĐIỂM TỔNG HỢP HK '.$hocky.' - NĂM HỌC: '.$namhoc.'-'.($namhoc+1));
    } else {
        $ActiveSheet->setCellValue('C4','BẢNG ĐIỂM TỔNG HỢP NĂM HỌC: '.$namhoc.'-'.($namhoc+1));
    }
    $col1 = 'E';
    $r = 6;
    $arrPos = array();
    //$mhtd = db_select('uit_monhoctd','td')->fields('td', array('mamh1', 'mamh2'))
    //        ->condition('hedt', $hedt)
    //        ->orderBy('mamh1')->execute()->fetchAllKeyed();
    $ctdt = db_select('uit_ctdaotao','c')->fields('c',array('mamh','sotc'))
            ->condition('hedt', $hedt)->condition('makhoa', $khoa)
            ->condition('khoadt', $khoahoc)
            ->execute()->fetchAllKeyed();
    foreach ($a_mh as $mh) {
        if(!isset($arrPos[$mh->mamh])) {
            $mamhtd = MonHocTuongDuong($mh->mamh, $hedt);
            $cont = FALSE;
            foreach ($mamhtd as $mamh2=>$value) {
                if(isset($arrPos[$mamh2])) {
                    $arrPos[$mh->mamh] = $arrPos[$mamh2];
                    $cont = TRUE;
                    break;
                }
            }
            if($cont) continue;
            
            $arrPos[$mh->mamh] = $col1;
            $ActiveSheet->setCellValue($col1.($r),$mh->tenmh);
            //$ActiveSheet->getStyle($col1.($r))->getAlignment()->getVertical();
            $ActiveSheet->setCellValue($col1.($r+1),$mh->mamh);
            $ActiveSheet->setCellValue($col1.($r+2),(isset($ctdt[$mh->mamh]) ? $ctdt[$mh->mamh]:$mh->sotc));
            if($mh->sotc == 0) {
                $ActiveSheet->getStyle($col1.($r).':'.$col1.($r+2))->applyFromArray( $style_cell );
            }
            $col1++;
        }
    }
    /*
    $colTCTL = $col2;
    $ActiveSheet->setCellValue($col2++.'5','Số TC đã học');
    $colTCTL2 = $col2;
    $ActiveSheet->setCellValue($col2++.'5','Số TC tích lũy');
    $ActiveSheet->setCellValue($col2.'5','Điểm TB tích lũy');
    $colDTB = $col2;
    */
    $r = 9;
    $stt = 1;
    $arrTCTL = array(); //Số tín chỉ tích lũy đã học
    $arrDTB = array(); // Điểm TB HK
    $arrDTBTL = array(); // Điểm TB tích lũy đến HK đang xét
    $arrTCPass = array(); // Số tín chỉ tích lũy đã pass đến HK đang xét
    
    foreach ($a_hv as $mahv) {
        $hv = db_select('uit_thongtinhv', 'hv')->fields('hv', array('hoten', 'khoa'))->condition('mahv', $mahv)->execute()->fetch();
        $q = db_select('uit_diem', 'd');
        $q->join('uit_monhoc', 'mh', 'mh.mamh = d.mamh');
        $q->fields('d');
        $q->fields('mh', array('tenmh'));
        $q->condition('d.mahv', $mahv);
        if($hocky) {
            $q->condition('d.hocky', $hocky);
        }
        $q->condition('d.namhoc', $namhoc);
        $q->condition('d.trangthai', 1);
        
        $diem = $q->execute()->fetchAll();
        $col = 'A';
        $ActiveSheet->setCellValue($col++.$r,$stt);
        $ActiveSheet->setCellValue($col++.$r,$hv->khoa);
        $ActiveSheet->setCellValueExplicit($col++.$r,$mahv, PHPExcel_Cell_DataType::TYPE_STRING);
        $ActiveSheet->setCellValue($col++.$r,$hv->hoten);
        $tctichluy = 0;
        $diemtl = 0;
        foreach ($diem as $mh) {
            if(isset($arrPos[$mh->mamh])) {
                $ActiveSheet->setCellValue($arrPos[$mh->mamh].$r,$mh->diem);
                if($mh->diem < 5) {
                    $ActiveSheet->getStyle($arrPos[$mh->mamh].$r)->applyFromArray( $style_cell );
                }
            }
            if($mh->sotc > 0) {
                $tctichluy += $mh->sotc;
                $diemtl += $mh->diem*$mh->sotc;
            }
        }
        $arrTCTL[$r] = $tctichluy;
        $arrDTB[$r] = 0;
        if($tctichluy > 0) $arrDTB[$r] = round($diemtl / $tctichluy,2);
        
        //Tính số TCTL và ĐTBTL đến HK hiện tại
        $q = db_select('uit_diem', 'd');
        $q->fields('d', array('mamh','sotc','diem'));
        $q->condition('d.mahv', $mahv);
        $q->condition('d.trangthai', 1);
        if($hocky) {
            $q->where('(d.namhoc < '.$namhoc.' OR (d.namhoc = '.$namhoc.' AND d.hocky <= '.$hocky.'))');
        } else {
            $q->condition('d.namhoc',$namhoc, '<');
        }
        $q->orderBy('d.namhoc');
        $q->orderBy('d.hocky');
        $q->orderBy('d.mamh');
        $arrdiem = $q->execute()->fetchAll();
        $tctichluy = 0;
        $diemtl = 0;
        $tctl = 0;
        foreach ($arrdiem as $mh) {
            if($mh->sotc > 0) {
                $tctichluy += $mh->sotc;
                $diemtl += $mh->diem*$mh->sotc;
                if($mh->diem >= 5) {
                    $tctl += $mh->sotc;
                }
            }
            
        }
        $arrTCPass[$r] = $tctl;
        $arrDTBTL[$r] = 0;
        if($tctichluy > 0) $arrDTBTL[$r] = round($diemtl / $tctichluy,2);
        
        $stt++;
        $r++;
    }
    
    $col2 = $col3 = $col1;
    $col3++;
    $col4 = $col3;
    $col4++;
    $col5 = $col4;
    $col5++;
    $col6 = $col5;
    $col6++;
    $str1 = 'học kỳ';
    if(!$hocky) $str1 = 'năm học';
    $ActiveSheet->setCellValue($col1.'6','Số TC '.$str1);
    $ActiveSheet->mergeCells($col1.'6:'.$col1.'8');
    $objCommentRichText = $ActiveSheet->getComment($col1.'6')->getText()->createTextRun('PĐT: Tổng số TC sinh viên học của HK đang xét');
    $objCommentRichText->getFont()->setBold(true);
    $col1++;
    $ActiveSheet->setCellValue($col1.'6','Điểm TB '.$str1);
    $ActiveSheet->mergeCells($col1.'6:'.$col1.'8');
    $objCommentRichText = $ActiveSheet->getComment($col1.'6')->getText()->createTextRun('PĐT: Điểm TB của tất cả các môn mà hv đăng ký học trong HK đang xét');
    $objCommentRichText->getFont()->setBold(true);
    $col1++;
    $ActiveSheet->setCellValue($col1.'6','Số TC tích lũy');
    $ActiveSheet->mergeCells($col1.'6:'.$col1.'8');
    $objCommentRichText = $ActiveSheet->getComment($col1.'6')->getText()->createTextRun('PĐT: Số tín chỉ của các học phần mã hv đã đậu (>=5đ) từ khi bắt đầu học đến thời điểm xét.');
    $objCommentRichText->getFont()->setBold(true);
    $col1++;
    $ActiveSheet->setCellValue($col1.'6','Điểm TB chung tích lũy');
    $ActiveSheet->mergeCells($col1.'6:'.$col1.'8');
    $objCommentRichText = $ActiveSheet->getComment($col1.'6')->getText()->createTextRun('PĐT: ĐTB tất cả các học phần mà hv đã học (kể cả học phần chưa đậu) từ khi bắt đầu học đến thời điểm xét');
    $objCommentRichText->getFont()->setBold(true);
    if(!$hocky) {
        $col1++;
        $ActiveSheet->setCellValue($col1.'6','Xếp loại');
        $ActiveSheet->mergeCells($col1.'6:'.$col1.'8');
        $objCommentRichText->getFont()->setBold(true);
    }
    
    foreach ($arrTCTL as $row => $value) {
        $ActiveSheet->setCellValue($col2.$row,$value);
        $ActiveSheet->setCellValue($col3.$row,$arrDTB[$row]);
        $ActiveSheet->setCellValue($col4.$row,$arrTCPass[$row]);
        $ActiveSheet->setCellValue($col5.$row,$arrDTBTL[$row]);
        if(!$hocky) {
            $ActiveSheet->setCellValue($col6.$row,  Xeploai($arrDTB[$row]));
        }
    }   
    
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
    ob_end_clean();
    $objWriter->save($path);

    return $path;  
}

function VarCharPlus($varchar,$num) {
    for($i=0;$i<$num;$i++) $varchar++;
    return $varchar;
}

function bangdiemth_build_filter_query(SelectQueryInterface $query) {
    $filter_data = isset($_SESSION['bangdiemth_filter']) ? $_SESSION['bangdiemth_filter'] : array();
    foreach ($filter_data as $key => $value) {
        switch ($key) {
            case 'coso':break;
            case 'timkiem':
            case 'sodong':
                break;
            case 'mahv':
            case 'lopsh':
                $arrmahv = explode(',', $value);
                $query->condition('hv.'.$key, $arrmahv, 'IN');
                break;
            default:
                $query->condition('hv.'.$key, $value);
                break;
        }
    }
}

function bangdiemth_set_filters(&$form_state) {
  $filters = bangdiemth_filters();
  //Apply every filter that has a choice selected other than 'any'.
  foreach ($filters as $filter => $options) {
    if (isset($form_state['values'][$filter])) {
      if( $form_state['values'][$filter] != '') {
        $_SESSION['bangdiemth_filter'][$filter] = $form_state['values'][$filter];
      }
      else {
        unset($_SESSION['bangdiemth_filter'][$filter]);
      }
    } 
  }
}

function bangdiemth_filters() {
  $filters = array();  
  if(!isset($_SESSION['bangdiemth_filter'])) {
      $_SESSION['bangdiemth_filter']['sodong'] = 50;
      $_SESSION['bangdiemth_filter']['tinhtrang'] = 1;
      $_SESSION['bangdiemth_filter']['coso'] = '00';
      $_SESSION['bangdiemth_filter']['khoahoc'] = 2000;
      $_SESSION['bangdiemth_filter']['chuyennganh'] = '000';
  }
  //if(!isset($_SESSION['bangdiemth_filter']['sodong'])) $_SESSION['bangdiemth_filter']['sodong'] = 50;
  $session = isset($_SESSION['bangdiemth_filter']) ? $_SESSION['bangdiemth_filter'] : array();
  $mahv = !empty($session['mahv']) ? $session['mahv'] : NULL;
  $khoahoc = !empty($session['khoahoc']) ? $session['khoahoc'] : '06';
  $coso = !empty($session['coso']) ? $session['coso'] : NULL;
  $lopsh = !empty($session['lopsh']) ? $session['lopsh'] : NULL;
  $chuyennganh = !empty($session['chuyennganh']) ? $session['chuyennganh'] : NULL;
  $tinhtrang = !empty($session['tinhtrang']) ? $session['tinhtrang'] : '';
  $sodong = !empty($session['sodong']) ? $session['sodong'] : 50;
  
  $skhoahoc = array('2000' => t('Chọn khóa học'));
  for($i=2001;$i<=date('Y');$i++) $skhoahoc[$i] = $i;
  
    $filters['mahv'] = array(
      'type' => 'textfield',
      'title' => t('Danh sách MShv:'),
      'size' => 90,
      'maxlength' => 1000,
      'default_value' => $mahv,    
      'attributes' => array('title' =>t('Mỗi MShv cách nhau bởi dấu phẩy (,)')),
    );
    $filters['coso'] = array(
      'type' => 'select',
      'title' => t('Cơ sở'),
      'options' => (array('00' => t('Chọn cơ sở')) + db_select('uit_coso', 'k')->orderBy('macs')->fields('k', array('macs', 'tenvt'))->execute()->fetchAllKeyed()),
      'default_value' => array($coso),
    );
    $filters['khoahoc'] = array(
      'type' => 'select',
      'title' => t('<br>Khóa'),
      'options' => $skhoahoc,
      'default_value' => array($khoahoc),
    );
    $filters['lopsh'] = array(
      'type' => 'textfield',
      'title' => t('Lớp:'),
      'size' => 30,
      'default_value' => $lopsh,
      'attributes' => array('title' =>t('Mỗi lớp cách nhau bởi dấu phẩy (,)')),
    );
    $filters['chuyennganh'] = array(
      'type' => 'select',
      'title' => t('CTĐT'),
      'options' => (array('000' => t('Chọn CTĐT')) + db_select('uit_nganh', 'k')->orderBy('nganh')->fields('k', array('nganh', 'nganh'))->execute()->fetchAllKeyed()),
      'default_value' => array($chuyennganh),
    );
    $filters['tinhtrang'] = array(
      'type' => 'select',
      'title' => t('Tình trạng'),
      'options' => (array('' => t('Tất cả')) + db_select('uit_tinhtrang', 'k')->orderBy('ten')->fields('k', array('id', 'ten'))->condition('k.doituong','hv')->execute()->fetchAllKeyed()),
      'default_value' => array($tinhtrang),
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
          'callback' => 'ajax_filter_bangdiemth_callback',
          'wrapper' => 'table_bangdiemth',
        ),
    );
  
  return $filters;
}

function bangdiemth_filter_form() {
    $form = array();
    $filters = bangdiemth_filters();
    foreach ($filters as $key => $filter) {
        $arr = array();
        foreach ($filter as $key2 => $value2) {
            $arr['#'.$key2] = $value2;
        }
        $form['filters'][$key] = $arr;
    }
    return $form;
}

function ajax_filter_bangdiemth_callback($form, $form_state) {
  return $form['bangdiemth_data'];
}