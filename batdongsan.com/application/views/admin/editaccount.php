<html lang="en">
<!-- BEGIN HEAD -->
<head>
<meta charset="utf-8"/>
<title>Edit Account</title>
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
				Add Accounts
				</h3>
				<div class="page-bar">
					<ul class="page-breadcrumb">
						<li>
							<i class="fa fa-home"></i>
							<a href="<?php echo base_url(); ?>admin">Home</a>
							<i class="fa fa-angle-right"></i>
						</li>
						<li>
							<a href="<?php echo base_url(); ?>admin/manage_account">Accounts</a>
							<i class="fa fa-angle-right"></i>
						</li>
						<li>
							Edit Account
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
									<i class="fa fa-pencil"></i>Edit Account
								</div>
								<div class="tools">
								</div>
							</div>
							<div class="portlet-body form">
								<!-- BEGIN FORM-->
								<form action="<?php echo base_url(); ?>admin/update_account" method="post" class="form-horizontal">
									<div class="form-body">
										<div class="form-group">
											<label class="col-md-3 control-label">Username</label>
											<div class="col-md-4" id="divuser">
												<input type="text" class="form-control input-circle" placeholder="Enter text" id="username" name="username" value="<?php echo $accountinfo->USERNAME; ?>" readonly>
											</div>
										</div>
										<div class="form-group">
											<label class="col-md-3 control-label">Email Address</label>
											<div class="col-md-4">
												<div class="input-group">
													<span class="input-group-addon input-circle-left">
													<i class="fa fa-envelope"></i>
													</span>
													<input type="email" class="form-control input-circle-right" value="<?php echo $accountinfo->EMAIL; ?>" name="email" placeholder="Email Address">
												</div>
											</div>
										</div>
										<div class="form-group">
											<label class="col-md-3 control-label">Full name</label>
											<div class="col-md-4">
												<input type="text" class="form-control input-circle" placeholder="Enter text" value="<?php echo $accountinfo->FULLNAME; ?>" name="fullname">
											</div>
										</div>
										<div class="form-group">
											<label class="col-md-3 control-label">Sex</label>
											<div class="col-md-4 radio-list">
												<label class="radio-inline">
													<span class="checked">
														<input type="radio" <?php if($accountinfo->SEX == "Male") echo "checked"; ?> name="sex" id="optionsRadios4" value="Male" checked="">
													</span>
													Male
												</label>
												<label class="radio-inline">
													<span>
														<input type="radio" name="sex" id="optionsRadios5" <?php if($accountinfo->SEX == "Female") echo "checked"; ?> value="Female">
													</span>
													Female
												</label>
											</div>
										</div>
										<div class="form-group">
											<label class="col-md-3 control-label">Role</label>
											<div class="col-md-4">
												<select name="role" class="form-control" required>
													<?php if(is_array($roles)): ?>
														<?php foreach($roles as $role): ?>
															<option <?php if($accountinfo->ROLEID == $role->ROLEID) echo "selected"; ?> value="<?php echo $role->ROLEID; ?>"><?php echo $role->ROLENAME; ?></option>
														<?php endforeach; ?>
													<?php endif; ?>
												</select>
											</div>
										</div>

									</div>
									<div class="form-actions">
										<div class="row">
											<div class="col-md-offset-3 col-md-9">
												<input type="hidden" name="accountid" value="<?php echo $accountinfo->ACCOUNTID; ?>" >
												<button type="submit" class="btn btn-circle blue">Submit</button>
												<a href="<?php echo base_url(); ?>admin/manage_account" ><button type="button" class="btn btn-circle default">Cancel</button></a>
											</div>
										</div>
									</div>
								</form>
								<!-- END FORM-->
							</div>
						</div>
						<div class="portlet box green">
							<div class="portlet-title">
								<div class="caption">
									<i class="fa fa-pencil"></i>Change Password
								</div>
								<div class="tools">
								</div>
							</div>
							<div class="portlet-body form">
								<!-- BEGIN FORM-->
								<form action="<?php echo base_url(); ?>admin/changepassword" method="post" class="form-horizontal">
									<div class="form-body">
										<div class="form-group">
											<label class="col-md-3 control-label">Password</label>
											<div class="col-md-4">
												<div class="input-group">
													<input type="password" class="form-control input-circle-left" placeholder="Password" id="pw1" required>
													<span class="input-group-addon input-circle-right">
													<i class="fa fa-key"></i>
													</span>
												</div>
											</div>
										</div>
										<div class="form-group">
											<label class="col-md-3 control-label">Re-Password</label>
											<div class="col-md-4">
												<div class="input-group">
													<input type="password" class="form-control input-circle-left" name="password" id="pw2" placeholder="Re-Password" required>
													<span class="input-group-addon input-circle-right">
													<i class="fa fa-key"></i>
													</span>
												</div>
												<span style="font-weight:bold;" class='help-block' id="pwstt"></span>
											</div>
										</div>

										<div class="form-actions">
											<div class="row">
												<div class="col-md-offset-3 col-md-9">
													<input type="hidden" name="accountid" value="<?php echo $accountinfo->ACCOUNTID; ?>" >
													<button type="submit" id="regacc" class="btn btn-circle blue">Change Password</button>
												</div>
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