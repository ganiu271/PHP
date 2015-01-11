<html lang="en">
<!-- BEGIN HEAD -->
<head>
	<meta charset="utf-8"/>
	<title>Add News</title>
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta content="width=device-width, initial-scale=1.0" name="viewport"/>
	<meta http-equiv="Content-type" content="text/html; charset=utf-8">
	<meta content="" name="description"/>
	<meta content="" name="author"/>
	<!-- BEGIN GLOBAL MANDATORY STYLES -->
	<link href="http://fonts.googleapis.com/css?family=Open+Sans:400,300,600,700&subset=all" rel="stylesheet" type="text/css">
	<link href="<?php echo base_url(); ?>resource/admin/assets/global/plugins/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
	<link href="<?php echo base_url(); ?>resource/admin/assets/global/plugins/simple-line-icons/simple-line-icons.min.css" rel="stylesheet" type="text/css">
	<link href="<?php echo base_url(); ?>resource/admin/assets/global/plugins/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css">
	<link href="<?php echo base_url(); ?>resource/admin/assets/global/plugins/uniform/css/uniform.default.css" rel="stylesheet" type="text/css">
	<link href="<?php echo base_url(); ?>resource/admin/assets/global/plugins/bootstrap-switch/css/bootstrap-switch.min.css" rel="stylesheet" type="text/css"/>
	<!-- END GLOBAL MANDATORY STYLES -->
	<!-- BEGIN PAGE LEVEL STYLES -->
	<link rel="stylesheet" type="text/css" href="<?php echo base_url(); ?>resource/admin/assets/global/plugins/select2/select2.css"/>
	<!-- END PAGE LEVEL SCRIPTS -->
	<!-- BEGIN THEME STYLES -->
	<link href="<?php echo base_url(); ?>resource/admin/assets/global/css/components.css" rel="stylesheet" type="text/css"/>
	<link href="<?php echo base_url(); ?>resource/admin/assets/global/css/plugins.css" rel="stylesheet" type="text/css"/>
	<link href="<?php echo base_url(); ?>resource/admin/assets/admin/layout2/css/layout.css" rel="stylesheet" type="text/css"/>
	<link id="style_color" href="<?php echo base_url(); ?>resource/admin/assets/admin/layout2/css/themes/default.css" rel="stylesheet" type="text/css"/>
	<link href="<?php echo base_url(); ?>resource/admin/assets/admin/layout2/css/custom.css" rel="stylesheet" type="text/css"/>
	<!-- END THEME STYLES -->
	<link rel="shortcut icon" href="favicon.ico"/>
</head>
<!-- END HEAD -->
<!-- BEGIN BODY -->
<body class="page-boxed page-header-fixed page-container-bg-solid page-sidebar-closed-hide-logo ">
	<!-- BEGIN HEADER -->
		<?php $this->load->view('admin/menuheader'); ?>
	<!-- END HEADER -->
	<div class="clearfix">
	</div>
	<div class="container">
		<!-- BEGIN CONTAINER -->
		<div class="page-container">
			<!-- BEGIN SIDEBAR -->
				<?php $this->load->view('admin/menubar',$menuactive); ?>
			<!-- END SIDEBAR -->
			<!-- BEGIN CONTENT -->
			<div class="page-content-wrapper">
				<div class="page-content">
					<!-- BEGIN PAGE HEADER-->
					<h3 class="page-title">
					Add News
					</h3>
					<div class="page-bar">
						<ul class="page-breadcrumb">
							<li>
								<i class="fa fa-home"></i>
								<a href="<?php echo base_url(); ?>admin">Home</a>
								<i class="fa fa-angle-right"></i>
							</li>
							<li>
								<a href="<?php echo base_url(); ?>admin/manage_news">News</a>
								<i class="fa fa-angle-right"></i>
							</li>
							<li>
								<a href="<?php echo base_url(); ?>admin/add_news">Add News</a>
							</li>
						</ul>
					</div>
					<!-- END PAGE HEADER-->
					<!-- BEGIN PAGE CONTENT-->
					<div class="row">
						<div class="col-md-12">

							<div class="portlet box green">
								<div class="portlet-title">
									<div class="caption">
										<i class="fa fa-newspaper-o"></i>Add News
									</div>
									<div class="tools">
									</div>
								</div>
								<div class="portlet-body form">
									<!-- BEGIN FORM-->
									<form action="<?php echo base_url(); ?>admin/insert_news" method="post" enctype="multipart/form-data" class="form-horizontal">
										<div class="form-body">
											<div class="form-group">
												<label class="col-md-3 control-label">Title</label>
												<div class="col-md-4">
													<input type="text" class="form-control " placeholder="Enter text" name="title" required>
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Kind</label>
												<div class="col-md-4">
													<select id="kind" name="kind" class="form-control">
														<option value="Nhà trọ">Nhà trọ</option>
														<option value="Căn hộ">Căn hộ</option>
														<option value="Văn phòng/Mặt bằng">Văn phòng / Mặt bằng</option>
														<option value="Nhà đất">Nhà đất</option>
													</select>
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Type</label>
												<div class="col-md-4">
													<select id="type" name="type" class="form-control">
														<option value'Nhà trọ cho thuê'>Nhà trọ cho thuê</option>
														<option value'Tìm người ở ghép'>Tìm người ở ghép</option>
													</select>
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Price</label>
												<div class="col-md-4">
													<input type="text" class="form-control " required placeholder="Enter text" name="price">
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Size</label>
												<div class="col-md-4">
													<input type="text" class="form-control " required placeholder="Enter text" name="size">
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Address</label>
												<div class="col-md-4">
													<input type="text" class="form-control " required placeholder="Enter text" name="address">
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">District</label>
												<div class="col-md-4">
													<select id="districtaddnews" name="district" class="form-control">
														<?php $this->load->view('admin/ajaxdistrict', $data); ?>
													</select>
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Province</label>
												<div class="col-md-4">
													<select id="provinceaddnews" name="province" class="form-control">
														<?php if(is_array($provinces)): ?>
															<?php foreach($provinces as $province): ?>
																<option value="<?php echo $province->PROVINCEID; ?>"><?php echo $province->PROVINCENAME; ?></option>
															<?php endforeach; ?>
														<?php endif; ?>
													</select>
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Description</label>
												<div class="col-md-4">
													<input type="text" class="form-control " required placeholder="Enter text" name="description">
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Contact Name</label>
												<div class="col-md-4">
													<input type="text" class="form-control " required placeholder="Enter text" name="contactname">
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Contact  Phone</label>
												<div class="col-md-4">
													<input type="text" class="form-control " required placeholder="Enter text" name="contactnumber">
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Contact Email</label>
												<div class="col-md-4">
													<input type="text" class="form-control " required placeholder="Enter text" name="contactemail">
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Contact Address</label>
												<div class="col-md-4">
													<input type="text" class="form-control " required placeholder="Enter text" name="contactaddress">
												</div>
											</div>
											<div class="form-group">
												<label class="col-md-3 control-label">Image</label>
												<div class="col-md-4">
													<input type="file" id="userfile" name="userfile[]" multiple>
												</div>
											</div>
											
										</div>
										<div class="form-actions">
											<div class="row">
												<div class="col-md-offset-3 col-md-9">
													<button type="submit" id="regacc" class="btn btn-circle blue">Submit</button>
													<a href="<?php echo base_url(); ?>admin/manage_news" ><button type="button" class="btn btn-circle default">Cancel</button></a>
												</div>
											</div>
										</div>
									</form>
									<!-- END FORM-->
								</div>
							</div>
						</div>
					</div>
					<!-- END PAGE CONTENT-->
				</div>
			</div>
			<!-- END CONTENT -->
			<!-- BEGIN QUICK SIDEBAR -->
			<!--Cooming Soon...-->
			<!-- END QUICK SIDEBAR -->
		</div>
		<!-- END CONTAINER -->
		<!-- BEGIN FOOTER -->
		<div class="page-footer">
			<div class="page-footer-inner">
				 2014 &copy; Metronic.
			</div>
			<div class="scroll-to-top">
				<i class="icon-arrow-up"></i>
			</div>
		</div>
		<!-- END FOOTER -->
	</div>
	<!-- BEGIN JAVASCRIPTS(Load javascripts at bottom, this will reduce page load time) -->
	<!-- BEGIN CORE PLUGINS -->
	<!--[if lt IE 9]>
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/respond.min.js"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/excanvas.min.js"></script> 
	<![endif]-->
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/jquery.min.js" type="text/javascript"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/jquery-migrate.min.js" type="text/javascript"></script>
	<!-- IMPORTANT! Load jquery-ui-1.10.3.custom.min.js before bootstrap.min.js to fix bootstrap tooltip conflict with jquery ui tooltip -->
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/jquery-ui/jquery-ui-1.10.3.custom.min.js" type="text/javascript"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/bootstrap/js/bootstrap.min.js" type="text/javascript"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/bootstrap-hover-dropdown/bootstrap-hover-dropdown.min.js" type="text/javascript"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/jquery-slimscroll/jquery.slimscroll.min.js" type="text/javascript"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/jquery.blockui.min.js" type="text/javascript"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/jquery.cokie.min.js" type="text/javascript"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/uniform/jquery.uniform.min.js" type="text/javascript"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/bootstrap-switch/js/bootstrap-switch.min.js" type="text/javascript"></script>
	<!-- END CORE PLUGINS -->
	<!-- BEGIN PAGE LEVEL PLUGINS -->
	<script type="text/javascript" src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/select2/select2.min.js"></script>
	<!-- END PAGE LEVEL PLUGINS -->
	<!-- BEGIN PAGE LEVEL SCRIPTS -->
	<script src="<?php echo base_url(); ?>resource/admin/assets/global/scripts/metronic.js" type="text/javascript"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/admin/layout2/scripts/layout.js" type="text/javascript"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/admin/layout2/scripts/demo.js" type="text/javascript"></script>
	<script src="<?php echo base_url(); ?>resource/admin/assets/admin/pages/scripts/form-samples.js"></script>
	<!-- END PAGE LEVEL SCRIPTS -->
	<script>
	jQuery(document).ready(function() {    
	   	// initiate layout and plugins
	   	Metronic.init(); // init metronic core components
		Layout.init(); // init current layout
		Demo.init(); // init demo features
	   	FormSamples.init();
	});
	</script>
	<script src="<?php echo base_url(); ?>resource/myjs/myscript.js"></script>
	<!-- END JAVASCRIPTS -->
</body>
<!-- END BODY -->
</html>