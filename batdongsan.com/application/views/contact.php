<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>Liên hệ</title>

	<link rel="stylesheet" href="<?php echo base_url();?>resource/bootstrap/css/bootstrap.min.css">
	<link rel="stylesheet" href="<?php echo base_url();?>resource/bootstrap/css/bootstrap-theme.min.css">
	<link rel="stylesheet" href="<?php echo base_url();?>resource/mycss/mystyle.css">

	<script src="<?php echo base_url();?>resource/bootstrap/jquery.min.js"></script>
	<script src="<?php echo base_url();?>resource/bootstrap/js/bootstrap.min.js"></script>
	<script src="<?php echo base_url();?>resource/myjs/myjavascript.js"></script>

	<link rel="stylesheet" href="<?php echo base_url();?>resource/bootstrap/css/font-awesome.css">
	<link rel="stylesheet" href="<?php echo base_url();?>resource/mycss/menuSideBarStyle.css">
	
</head>
<body>
	
</div>

<div class="menu" style="top: 0px;">
	<?php $this->load->view('navbar', 'nhatro'); ?>
</div>

<div class="topbarfixed" style="position: fixed; top: 0px; display: block;">
	<img id="logo2" class="btnImg" src="<?php echo base_url(); ?>resource/uphinh/logo2.png"/>
</div>

<div class="row content">
	<div class="col-md-12" style="margin: 200px 0px 100px 0px; color: #3e3e3e; font-size: 20px;">
		<center>
			Nếu bạn cần liên hệ để đặt quảng cáo, hoặc thông báo sự cố trang web,
			vui lòng liên hệ:<br>
			<i class="fa fa-phone-square"></i> Hotline: 0932 789 297<br>
			<i class="fa fa-envelope-o"></i> Email: huonghk.uit@gmail.com
		</center>
	</div>
</div>

<div class="footer row">
	<?php $this->load->view('footer'); ?>
</div>
</body>

<script>
	$(document).ready(function(){
		$('#logo2').click(function(){
			window.location.replace("<?php echo base_url(); ?>");
		});
	});
</script>