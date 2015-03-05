<?php

function qldiem_export_bangdiem($form, &$form_state, $mahv,$tn) {

    global $user;

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
    $q->fields('d')->fields('ctdt')->fields('mh');
    $q->condition('d.mahv', $mahv);
    if($tn=='tn'){
        $q->condition('d.diem', 5, '>=');
    }
    $q->condition('d.trangthai', 1);
    $q->orderBy('d.sotc','ASC');
//    $q->orderBy('d.id','ASC');
    $rows = $q->execute()->fetchAll();

    if(!$rows) {
        echo 'Không có thông tin điểm';
        die;
    }
    $size_row = sizeof($rows);

    require_once 'sites/all/libraries/PHPExcel/PHPExcel.php';
    require_once 'sites/all/libraries/PHPExcel/PHPExcel/IOFactory.php';

    $so_tt = 1;
    $cell_template = null;
    $lam_lv = FALSE;
    $hoten_export = $user->name;
    $gioitinh = ($hv->gioitinh == 1 ? 'Nam':'Nữ');
    $filename = ($tn=='tn') ? 'BangDiemTN_'.$hv->mahv.'.xlsx' : 'BangDiem_'.$hv->mahv.'.xlsx';
    $ngaysinh = (strlen($hv->ngaysinh) == 10 ? date("d-m-Y",strtotime($hv->ngaysinh)):$hv->ngaysinh);
    $chuyennganh=db_select('uit_nganh','ng')->fields('ng')->condition('nganh',$hv->chuyennganh)->execute()->fetchAll();

    $template_file = ($tn=='tn') ? drupal_get_path('module', 'qldiem').'/templates/BangDiemTN_Export.xlsx':drupal_get_path('module', 'qldiem').'/templates/BangDiem_Export.xlsx';
    $filename = variable_get('file_public_path', conf_path() . '/files') . '/exports/'.$filename;

    $objPHPExcel = PHPExcel_IOFactory::load($template_file);
    $ActiveSheet = $objPHPExcel->getActiveSheet();
    $current_date='TP.HCM, ngày '.date('d').' tháng '.date('m').' năm '.date('Y');
    $tl_hieutruong = 'TL. HIỆU TRƯỞNG ';
    $nguoi_ky = 'TRƯỞNG PHÒNG ĐT SĐH & KHCN	';
    $nguoi_ky_name = 'Nguyễn Phi Khứ';

    if($tn=='tn'){
        $cell_template = array(
            'ho_ten' => 'B4',
            'mahv' => 'B5',
            'ngay_sinh' => 'B6',
            'noisinh' => 'B7',
            'quyetdinh_trungtuyen' => 'B8',
            'chuyennganh' => 'B9',
            'khoa' => 'F9',
            'manganh' => 'D9',
            'ctdt' => 'B10',
            'col_stt' => 'A',
            'col_mamh' => 'B',
            'col_tenmh' => 'C',
            'col_sotc' => 'D',
            'col_diem' => 'E',
            'col_ghichu' => 'F',
            'data_diemlv' => '- Điểm luận văn (15TC) :',
            'data_diemtb' => '- Điểm trung bình môn học (65TC) :',
            'data_tendetai' => '- Tên đề tài luận văn :',
            'data_ngaybv' => '- Bảo vệ ngày, tháng, năm :',
            'data_hoidong' => '- Hội đồng chấm luận văn gồm :',
            'data_chutich' => '    1. Chủ tịch :',
            'data_thuky' => '    2. Thư ký :',
            'data_pb1' => '    3. Phản biện 1 :',
            'data_pb2' => '    4. Phản biện 2 :',
            'data_uyvien' => '    5. Ủy viên :',
            'start_line' => 14
        );

        /**
         * Add thông tin học viên
         */
        $start_line = $cell_template['start_line'];
        $ActiveSheet->setCellValue($cell_template['ho_ten'],$hv->hoten);
        $ActiveSheet->setCellValue($cell_template['ngay_sinh'],$ngaysinh);
        $ActiveSheet->setCellValue($cell_template['mahv'],$hv->mahv);
        $ActiveSheet->setCellValue($cell_template['chuyennganh'],$chuyennganh['0']->tennganh);
        $ActiveSheet->setCellValue($cell_template['noisinh'],$hv->noisinh);
        $ActiveSheet->setCellValue($cell_template['khoa'],$hv->khoahoc);
        $ActiveSheet->setCellValue($cell_template['manganh'],$hv->chuyennganh);

        //set Bold
        $objPHPExcel->getActiveSheet()->getStyle($cell_template['ho_ten'].':'.$cell_template['ctdt'])->getFont()->setBold(true);
        $objPHPExcel->getActiveSheet()->getStyle($cell_template['khoa'])->getFont()->setBold(true);
        $objPHPExcel->getActiveSheet()->getStyle($cell_template['manganh'])->getFont()->setBold(true);

        /**
         * Add điểm học viên
         */
        for ($i = 0; $i < $size_row; $i++) {
            $ActiveSheet->setCellValue($cell_template['col_stt'].$cell_template['start_line'],$so_tt);
            $ActiveSheet->setCellValue($cell_template['col_mamh'].$cell_template['start_line'],$rows[$i]->mamh);
            $ActiveSheet->setCellValue($cell_template['col_tenmh'].$cell_template['start_line'],$rows[$i]->tenmh);
            $ActiveSheet->setCellValue($cell_template['col_sotc'].$cell_template['start_line'],$rows[$i]->sotc);
            $ActiveSheet->setCellValue($cell_template['col_diem'].$cell_template['start_line'],$rows[$i]->diem);
            $so_tt++;
            $cell_template['start_line']++;
            if($i<$size_row-1){
                $ActiveSheet->insertNewRowBefore($cell_template['start_line'],1);
            }
            if($rows[$i]->sotc==15){
                $lam_lv = TRUE;
            }
        }

        /**
         * Tổng số tín chỉ & Điểm TB chung các môn học
         */
        $ActiveSheet->insertNewRowBefore($cell_template['start_line'],1);
        $ActiveSheet->insertNewRowBefore($cell_template['start_line'],1);

        $ActiveSheet->mergeCells($cell_template['col_stt'].$cell_template['start_line'].':C'.$cell_template['start_line']);
        $ActiveSheet->setCellValue($cell_template['col_stt'].$cell_template['start_line'],"Tổng số tín chỉ");
        $ActiveSheet->setCellValue('D'.$cell_template['start_line'],'=SUM(D'.$start_line.':D'.($cell_template['start_line']-1).')');

        $cell_template['start_line']++;
        $ActiveSheet->mergeCells($cell_template['col_stt'].$cell_template['start_line'].':D'.$cell_template['start_line']);
        $ActiveSheet->setCellValue($cell_template['col_stt'].$cell_template['start_line'],"Điểm trung bình chung các môn học");
        $ActiveSheet->setCellValue('E'.$cell_template['start_line'],'=ROUND(SUMPRODUCT(D'.$start_line.':D'.($cell_template['start_line']-2).',E'.$start_line.':E'.($cell_template['start_line']-2).')/D'.($cell_template['start_line']-1).',2)');

        $ActiveSheet->getStyle($cell_template['col_stt'].($cell_template['start_line']-1).':A'.$cell_template['start_line'])->getFont()->setBold(true);
        $ActiveSheet->getStyle('D'.($cell_template['start_line']-1))->getFont()->setBold(true);
        $ActiveSheet->getStyle('E'.$cell_template['start_line'])->getFont()->setBold(true);
        $ActiveSheet->getColumnDimension($cell_template['col_tenmh'])->setAutoSize(true);

        if($lam_lv){
            $ActiveSheet->mergeCells($cell_template['col_stt'].($cell_template['start_line']+2).':'.$cell_template['col_mamh'].($cell_template['start_line']+2));
            $ActiveSheet->mergeCells($cell_template['col_stt'].($cell_template['start_line']+3).':'.$cell_template['col_mamh'].($cell_template['start_line']+3));
            $ActiveSheet->mergeCells($cell_template['col_stt'].($cell_template['start_line']+4).':'.$cell_template['col_mamh'].($cell_template['start_line']+4));
            $ActiveSheet->mergeCells($cell_template['col_stt'].($cell_template['start_line']+5).':'.$cell_template['col_mamh'].($cell_template['start_line']+5));
            $ActiveSheet->mergeCells($cell_template['col_stt'].($cell_template['start_line']+6).':'.$cell_template['col_mamh'].($cell_template['start_line']+6));

            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+2),$cell_template['data_diemlv']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+3),$cell_template['data_diemtb']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+4),$cell_template['data_tendetai']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+5),$cell_template['data_ngaybv']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+6),$cell_template['data_hoidong']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+7),$cell_template['data_chutich']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+8),$cell_template['data_thuky']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+9),$cell_template['data_pb1']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+10),$cell_template['data_pb2']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+11),$cell_template['data_uyvien']);

            $ActiveSheet->getStyle($cell_template['col_stt'].($cell_template['start_line']+2).":".$cell_template['col_stt'].($cell_template['start_line']+6))
                ->getFont()->setBold(true);
        }

        $ActiveSheet->mergeCells($cell_template['col_sotc'].($cell_template['start_line']+11).':'.$cell_template['col_ghichu'].($cell_template['start_line']+11));
        $ActiveSheet->mergeCells($cell_template['col_sotc'].($cell_template['start_line']+12).':'.$cell_template['col_ghichu'].($cell_template['start_line']+12));
        $ActiveSheet->mergeCells($cell_template['col_sotc'].($cell_template['start_line']+13).':'.$cell_template['col_ghichu'].($cell_template['start_line']+13));
        $ActiveSheet->mergeCells($cell_template['col_sotc'].($cell_template['start_line']+16).':'.$cell_template['col_ghichu'].($cell_template['start_line']+16));

        $ActiveSheet->setCellValue($cell_template['col_sotc'].($cell_template['start_line']+11),$current_date);
        $ActiveSheet->setCellValue($cell_template['col_sotc'].($cell_template['start_line']+12),$tl_hieutruong);
        $ActiveSheet->setCellValue($cell_template['col_sotc'].($cell_template['start_line']+13),$nguoi_ky);
        $ActiveSheet->setCellValue($cell_template['col_sotc'].($cell_template['start_line']+16),$nguoi_ky_name);

        $ActiveSheet->getStyle($cell_template['col_sotc'].($cell_template['start_line']+11).":".$cell_template['col_sotc'].($cell_template['start_line']+16))
            ->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);

        $ActiveSheet->getStyle($cell_template['col_sotc'].($cell_template['start_line']+11))->getFont()->setItalic(true);
        $ActiveSheet->getStyle($cell_template['col_sotc'].($cell_template['start_line']+12).":".$cell_template['col_sotc'].($cell_template['start_line']+16))
            ->getFont()->setBold(true);
    }
    else{
        $cell_template=array(
            'ho_ten'=>'B4',
            'ngay_sinh'=>'B5',
            'mahv'=>'B6',
            'ctdt'=>'B7',
            'chuyennganh'=>'B8',
            'gioitinh'=>'E4',
            'noisinh'=>'E5',
            'khoa'=>'E6',
            'col_stt' => 'A',
            'col_mamh' => 'C',
            'col_tenmh' => 'B',
            'col_sotc' => 'D',
            'col_diem' => 'E',
            'data_tendetai' => ' Đề tài luận văn :',
            'data_ngaybv' => ' Ngày bảo vệ luận văn :',
            'data_hoidong' => ' Hội đồng chấm luận văn gồm :',
            'data_chutich' => '    1. Chủ tịch :',
            'data_thuky' => '    2. Thư ký :',
            'data_pb1' => '    3. Phản biện 1 :',
            'data_pb2' => '    4. Phản biện 2 :',
            'data_uyvien' => '    5. Ủy viên :',
            'start_line'=>11
        );

        /**
         * Add thông tin học viên
         */
        $start_line = $cell_template['start_line'];
        $ActiveSheet->setCellValue($cell_template['ho_ten'],$hv->hoten);
        $ActiveSheet->setCellValue($cell_template['ngay_sinh'],$ngaysinh);
        $ActiveSheet->setCellValue($cell_template['mahv'],$hv->mahv);
        $ActiveSheet->setCellValue($cell_template['chuyennganh'],$chuyennganh['0']->tennganh);
        $ActiveSheet->setCellValue($cell_template['gioitinh'],$gioitinh);
        $ActiveSheet->setCellValue($cell_template['noisinh'],$hv->noisinh);
        $ActiveSheet->setCellValue($cell_template['khoa'],$hv->khoahoc);

        //set Bold
        $objPHPExcel->getActiveSheet()->getStyle($cell_template['ho_ten'].':'.$cell_template['chuyennganh'])->getFont()->setBold(true);
        $objPHPExcel->getActiveSheet()->getStyle($cell_template['gioitinh'].':'.$cell_template['khoa'])->getFont()->setBold(true);

        /**
         * Add điểm học viên
         */
        for ($i = 0; $i < $size_row; $i++) {
            $ActiveSheet->setCellValue($cell_template['col_stt'].$cell_template['start_line'],$so_tt);
            $ActiveSheet->setCellValue($cell_template['col_mamh'].$cell_template['start_line'],$rows[$i]->mamh);
            $ActiveSheet->setCellValue($cell_template['col_tenmh'].$cell_template['start_line'],$rows[$i]->tenmh);
            $ActiveSheet->setCellValue($cell_template['col_sotc'].$cell_template['start_line'],$rows[$i]->sotc);
            $ActiveSheet->setCellValue($cell_template['col_diem'].$cell_template['start_line'],$rows[$i]->diem);
            $so_tt++;
            $cell_template['start_line']++;
            if($i<$size_row-1){
                $ActiveSheet->insertNewRowBefore($cell_template['start_line'],1);
            }
            if($rows[$i]->sotc==15){
                $lam_lv = TRUE;
            }
        }

        /**
         * Tổng số tín chỉ & Điểm TB chung các môn học
         */
        $ActiveSheet->insertNewRowBefore($cell_template['start_line'],1);
        $ActiveSheet->insertNewRowBefore($cell_template['start_line'],1);

        $ActiveSheet->mergeCells($cell_template['col_stt'].$cell_template['start_line'].':C'.$cell_template['start_line']);
        $ActiveSheet->setCellValue($cell_template['col_stt'].$cell_template['start_line'],"Tổng số tín chỉ");
        $ActiveSheet->setCellValue($cell_template['col_sotc'].$cell_template['start_line'],
        '=SUM('.$cell_template['col_sotc'].$start_line.':'.$cell_template['col_sotc'].($cell_template['start_line']-1).')');

        $cell_template['start_line']++;
        $ActiveSheet->mergeCells($cell_template['col_stt'].$cell_template['start_line'].':'.$cell_template['col_sotc'].$cell_template['start_line']);
        $ActiveSheet->setCellValue($cell_template['col_stt'].$cell_template['start_line'],"Điểm trung bình chung các môn học");
        $ActiveSheet->setCellValue($cell_template['col_diem'].$cell_template['start_line'],'=ROUND(SUMPRODUCT('.$cell_template['col_sotc'].$start_line.':'.$cell_template['col_sotc'].($cell_template['start_line']-2).',E'.$start_line.':E'.($cell_template['start_line']-2).')/D'.($cell_template['start_line']-1).',2)');

        $ActiveSheet->getStyle($cell_template['col_stt'].($cell_template['start_line']-1).':'.$cell_template['col_stt'].$cell_template['start_line'])->getFont()->setBold(true);
        $ActiveSheet->getStyle($cell_template['col_sotc'].($cell_template['start_line']-1))->getFont()->setBold(true);
        $ActiveSheet->getStyle($cell_template['col_diem'].$cell_template['start_line'])->getFont()->setBold(true);

        if($lam_lv){

            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+2),$cell_template['data_tendetai']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+3),$cell_template['data_ngaybv']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+4),$cell_template['data_hoidong']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+5),$cell_template['data_chutich']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+6),$cell_template['data_thuky']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+7),$cell_template['data_pb1']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+8),$cell_template['data_pb2']);
            $ActiveSheet->setCellValue($cell_template['col_stt'].($cell_template['start_line']+9),$cell_template['data_uyvien']);

            $ActiveSheet->getStyle($cell_template['col_stt'].($cell_template['start_line']+2).":".$cell_template['col_stt'].($cell_template['start_line']+4))
                ->getFont()->setBold(true);
        }

        $ActiveSheet->mergeCells($cell_template['col_mamh'].($cell_template['start_line']+9).':'.$cell_template['col_diem'].($cell_template['start_line']+9));
        $ActiveSheet->mergeCells($cell_template['col_mamh'].($cell_template['start_line']+10).':'.$cell_template['col_diem'].($cell_template['start_line']+10));
        $ActiveSheet->mergeCells($cell_template['col_mamh'].($cell_template['start_line']+11).':'.$cell_template['col_diem'].($cell_template['start_line']+11));
        $ActiveSheet->mergeCells($cell_template['col_mamh'].($cell_template['start_line']+15).':'.$cell_template['col_diem'].($cell_template['start_line']+15));
        $ActiveSheet->setCellValue($cell_template['col_mamh'].($cell_template['start_line']+9),$current_date);
        $ActiveSheet->setCellValue($cell_template['col_mamh'].($cell_template['start_line']+10),$tl_hieutruong);
        $ActiveSheet->setCellValue($cell_template['col_mamh'].($cell_template['start_line']+11),$nguoi_ky);
        $ActiveSheet->setCellValue($cell_template['col_mamh'].($cell_template['start_line']+15),$nguoi_ky_name);

        $ActiveSheet->getStyle($cell_template['col_mamh'].($cell_template['start_line']+9).":".$cell_template['col_mamh'].($cell_template['start_line']+15))
            ->getAlignment()->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
        $ActiveSheet->getStyle($cell_template['col_mamh'].($cell_template['start_line']+9))->getFont()->setItalic(true);
        $ActiveSheet->getStyle($cell_template['col_mamh'].($cell_template['start_line']+10).":".$cell_template['col_mamh'].($cell_template['start_line']+15))
            ->getFont()->setBold(true);

    }

    $ctdt = ($lam_lv)?'Giảng dạy môn học có luận văn':'Giảng dạy môn học không luận văn';
    $ActiveSheet->setCellValue($cell_template['ctdt'],$ctdt);

    /**
     * Customize excel template
     */
    $ActiveSheet->getColumnDimension($cell_template['col_tenmh'])->setAutoSize(true);

    /**
     * Save file export & return to user
     */
    $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
    ob_end_clean();
    $objWriter->save($filename);
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
    return $form;
}
