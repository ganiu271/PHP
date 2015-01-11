<html lang="en">
<!-- BEGIN HEAD -->
<head>
<meta charset="utf-8"/>
<title>Dashboard</title>
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
						Dashboard
					</h3>
					<div class="page-bar">
						<ul class="page-breadcrumb">
							<li>
								<i class="fa fa-home"></i>
								<a href="<?php echo base_url(); ?>admin">Home</a>
							</li>
						</ul>
					</div>
				<!-- END PAGE HEADER-->
				<!-- BEGIN PAGE CONTENT-->
				<div class="row">
					<div class="col-lg-4 col-md-4 col-sm-6 col-xs-12 margin-bottom-10">
						<div class="dashboard-stat blue-madison">
							<div class="visual">
								<i class="fa fa-briefcase fa-icon-medium"></i>
							</div>
							<div class="details">
								<div class="number">
									<?php echo $totalaccounts; ?>
								</div>
								<div class="desc">
									Total Accounts
								</div>
							</div>
							<a class="more" href="<?php echo base_url(); ?>admin/manage_account">
							View more <i class="m-icon-swapright m-icon-white"></i>
							</a>
						</div>
					</div>
					<div class="col-lg-4 col-md-4 col-sm-6 col-xs-12">
						<div class="dashboard-stat red-intense">
							<div class="visual">
								<i class="fa fa-shopping-cart"></i>
							</div>
							<div class="details">
								<div class="number">
									<?php echo $totalnews; ?>
								</div>
								<div class="desc">
									Total News
								</div>
							</div>
							<a class="more" href="<?php echo base_url(); ?>admin/manage_news">
							View more <i class="m-icon-swapright m-icon-white"></i>
							</a>
						</div>
					</div>
					<div class="col-lg-4 col-md-4 col-sm-6 col-xs-12">
						<div class="dashboard-stat green-haze">
							<div class="visual">
								<i class="fa fa-group fa-icon-medium"></i>
							</div>
							<div class="details">
								<div class="number">
									<?php echo round(($totalnews/$totalaccounts),2); ?>
								</div>
								<div class="desc">
									Average News
								</div>
							</div>
							<a class="more" href="#">
							View more <i class="m-icon-swapright m-icon-white"></i>
							</a>
						</div>
					</div>
				</div>

				<div class="row">
					<div class="col-md-12">
						<!-- Begin: life time stats -->
						<div class="portlet light">
							<div class="portlet-title">
								<div class="caption">
									<i class="icon-bar-chart font-green-sharp"></i>
									<span class="caption-subject font-green-sharp bold uppercase">Overview</span>
								</div>
								<div class="tools">
									<a href="javascript:;" class="collapse">
									</a>
									<a href="javascript:;" class="fullscreen">
									</a>
								</div>
							</div>
							<div class="portlet-body">
								<div class="tabbable-line">
									<ul class="nav nav-tabs">
										<li class="active">
											<a href="#overview_1" data-toggle="tab">
											Top 5 new Users </a>
										</li>
										<li class="">
											<a href="#overview_2" data-toggle="tab">
											Top 5 new News </a>
										</li>
										<li class="">
											<a href="#overview_3" data-toggle="tab">
											Users has highest news </a>
										</li>
									</ul>
									<div class="tab-content">
										<div class="tab-pane active" id="overview_1">
											<div class="table-responsive">
												<table class="table table-striped table-hover table-bordered">
													<thead>
														<tr>
															<th>
																User ID
															</th>
															<th>
																Username
															</th>
															<th>
																Fullname
															</th>
														</tr>
													</thead>
													<tbody>
													<?php if(is_array($top5newusers)): ?>
														<?php foreach($top5newusers as $a): ?>
														<tr>
															<td>
																<?php echo $a->ACCOUNTID; ?>
															</td>
															<td>
																<a href="<?php echo base_url(); ?>admin/view_account/<?php echo $a->ACCOUNTID; ?>"><?php echo $a->USERNAME; ?></a>
															</td>
															<td>
																<?php echo $a->FULLNAME; ?>
															</td>
														</tr>
														<?php endforeach; ?>
													<?php endif; ?>
													</tbody>
												</table>
											</div>
										</div>
										<div class="tab-pane" id="overview_2">
											<div class="table-responsive">
												<table class="table table-striped table-hover table-bordered">
													<thead>
														<tr>
															<th>
																News ID
															</th>
															<th>
																Title
															</th>
															<th>
																Author
															</th>
															<th>
																Kind
															</th>
															<th>
																Type
															</th>
															<th>
																Price
															</th>
														</tr>
													</thead>
													<tbody>
														<?php if(is_array($top5newnews)): ?>
															<?php foreach($top5newnews as $b): ?>
															<tr>
																<td>
																	<?php echo $b->REALTYID; ?>
																</td>
																<td>
																	<a href="<?php echo base_url(); ?>admin/edit_news/<?php echo $b->REALTYID; ?>"><?php echo $b->TITLE; ?></a>
																</td>
																<td>
																	<a href="<?php echo base_url(); ?>admin/view_account/<?php echo $b->ACCOUNTID; ?>"><?php echo $b->USERNAME; ?></a>
																</td>
																<td>
																	<?php echo $b->KIND; ?>
																</td>
																<td>
																	<?php echo $b->TYPE; ?>
																</td>
																<td>
																	<?php echo $b->PRICE; ?>
																</td>
															</tr>
															<?php endforeach; ?>
														<?php endif; ?>
													</tbody>
												</table>
											</div>
										</div>
										<div class="tab-pane" id="overview_3">
											<div class="table-responsive">
												<table class="table table-striped table-hover table-bordered">
													<thead>
														<tr>
															<th>
																User ID
															</th>
															<th>
																Username
															</th>
															<th>
																Fullname
															</th>
															<th>
																Total News
															</th>
														</tr>
													</thead>
													<tbody>
													<?php if(is_array($tophighestnews)): ?>
														<?php foreach($tophighestnews as $c): ?>
														<tr>
															<td>
																<?php echo $c->ACCOUNTID; ?>
															</td>
															<td>
																<a href="<?php echo base_url(); ?>admin/view_account/<?php echo $c->ACCOUNTID; ?>"><?php echo $c->USERNAME; ?></a>
															</td>
															<td>
																<?php echo $c->FULLNAME; ?>
															</td>
															<td>
																<?php echo $c->totalnews; ?>
															</td>
														</tr>
														<?php endforeach; ?>
													<?php endif; ?>
													</tbody>
												</table>
											</div>
										</div>

									</div>
								</div>
							</div>
						</div>
						<!-- End: life time stats -->
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
<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/flot/jquery.flot.js" type="text/javascript"></script>
<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/flot/jquery.flot.resize.js" type="text/javascript"></script>
<script src="<?php echo base_url(); ?>resource/admin/assets/global/plugins/flot/jquery.flot.categories.js" type="text/javascript"></script>
<!-- END PAGE LEVEL PLUGINS -->
<!-- BEGIN PAGE LEVEL SCRIPTS -->
<script src="<?php echo base_url(); ?>resource/admin/assets/global/scripts/metronic.js" type="text/javascript"></script>
<script src="<?php echo base_url(); ?>resource/admin/assets/admin/layout2/scripts/layout.js" type="text/javascript"></script>
<script src="<?php echo base_url(); ?>resource/admin/assets/admin/layout2/scripts/demo.js" type="text/javascript"></script>
<script src="<?php echo base_url(); ?>resource/admin/assets/admin/pages/scripts/ecommerce-index.js"></script>
<!-- END PAGE LEVEL SCRIPTS -->
<script>
        jQuery(document).ready(function() {    
           Metronic.init(); // init metronic core components
Layout.init(); // init current layout
Demo.init(); // init demo features
           EcommerceIndex.init();
        });
    </script>
<!-- END JAVASCRIPTS -->
</body>
<!-- END BODY -->
</html>