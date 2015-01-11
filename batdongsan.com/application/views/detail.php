<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>Xem tin</title>

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


	<div class="menu" style="top: 0px;">
		<?php $this->load->view('navbar', 'nhatro'); ?>
	</div>

	<div class="topbarfixed" style="position: fixed; top: 0px; display: block;">
		<img id="logo2" class="btnImg" src="<?php echo base_url(); ?>resource/uphinh/logo2.png"/>
	</div>
	

	<div class="row content pub_body " >
		<div class="col-md-9 detailItem">
			<div class="col-md-12 detailContent">
				<?php foreach ($detail as $obj) : ?>
					<div class="detailHeader col-md-12">
						<?php echo $obj->KIND; ?> > <?php echo $obj->TYPE; ?>
					</div>

					<div class="detailTitle col-md-12">
						<?php echo $obj->TITLE; ?>
					</div>

					<div class="col-md-12">
						<div class="col-md-1 detailStyle1">
							Địa chỉ
						</div>
						<div class="col-md-11">
							: <?php echo $obj->ADDRESS; ?> - <?php echo $obj->DISTRICTNAME; ?> - <?php echo $obj->PROVINCENAME; ?>
						</div>
						
						<div class="col-md-1 detailStyle1">
							Diện tích
						</div>
						<div class="col-md-11">
							: <?php echo $obj->SIZE; ?>
						</div>

						<div class="col-md-1 detailStyle1">
							Giá
						</div>
						<div class="col-md-11">
							: <?php echo $obj->PRICE; ?>
						</div>

						<div class="col-md-8 photo-galary">
							
							<div id="myCarousel" class="carousel slide col-md-12" data-ride="carousel">
								<!-- Indicators -->
								<ol class="carousel-indicators">
									<?php $x = 0; ?>
									<?php foreach ($photo as $photoobj) : ?>

										<li data-target="#myCarousel" data-slide-to="<?php echo $x ?>" class="<?php if($x == 0) echo "active" ?>"></li>
										<?php $x += 1; ?>
									<?php endforeach; ?>
								</ol>



								<div class="carousel-inner" role="listbox">
									<?php $x = 0; ?>
									<?php foreach ($photo as $photoobj) : ?>
										<div class="item <?php if($x == 0) echo "active" ?>">
											<img style="height: 300px;" src="<?php echo base_url();?>resource/uphinh/<?php echo $photoobj->url; ?>">
										</div>
										<?php $x += 1; ?>
									<?php endforeach; ?>
								</div>



								<a class="left carousel-control" href="#myCarousel" role="button" data-slide="prev">
									<span class="glyphicon glyphicon-chevron-left" aria-hidden="true"></span>
									<span class="sr-only">Previous</span>
								</a>
								<a class="right carousel-control" href="#myCarousel" role="button" data-slide="next">
									<span class="glyphicon glyphicon-chevron-right" aria-hidden="true"></span>
									<span class="sr-only">Next</span>
								</a>
							</div>
							
							<div class="col-md-12 thumbSlide">
								<?php $x = 0; ?>
								<?php foreach ($photo as $photoobj) : ?>
									<div class="col-md-3 thumbItem">
										<a class="thumbnail" id="carousel-selector-<?php echo $x; ?>"> <img style="height: 110px;" src="<?php echo base_url();?>resource/uphinh/<?php echo $photoobj->url; ?>"></a>
									</div>
									<?php $x += 1; ?>
								<?php endforeach; ?>
							</div>
							
						</div>

						<div class="col-md-4 contactInfo">
							<div class="row contactHeader">
								<div class="col-md-12">
									THÔNG TIN LIÊN HỆ
								</div>
							</div>
							<div class="row">
								<div class="col-md-4 contactTitle">
									Tên liên hệ
								</div>
								<div class="col-md-8">
									: <?php echo $obj->CONTACTNAME; ?>
								</div>
							</div>
							<div class="row">
								<div class="col-md-4 contactTitle">
									SĐT
								</div>
								<div class="col-md-8">
									: <?php echo $obj->CONTACTTEL; ?>
								</div>
							</div>
							<div class="row">
								<div class="col-md-4 contactTitle">
									Email
								</div>
								<div class="col-md-8">
									: <?php echo $obj->CONTACTEMAIL; ?>
								</div>
							</div>
							<div class="row">
								<div class="col-md-4 contactTitle">
									Địa chỉ
								</div>
								<div class="col-md-8">
									: <?php echo $obj->CONTACTADDRESS; ?>
								</div>
							</div>
						</div>
					</div>
					
					
					<div class="col-md-12 detailStyle2">
						<div class="col-md-12 descriptionHeader">
							THÔNG TIN MÔ TẢ
						</div>

						<div class="col-md-12 " >
							<p class="description">
								<?php echo $obj->DESCRIPTION; ?>
							</p>
						</div>
						<div class="col-md-12 detailStyle2">
							<div class="col-md-12 descriptionHeader">
								HÌNH ẢNH CHI TIẾT
							</div>
							<div class="col-md-12">
								<?php foreach ($photo as $obj) : ?>
									<div class="item">
										<img width="75%" src="<?php echo base_url();?>resource/uphinh/<?php echo $obj->url; ?>" data-src="holder.js/900x500/auto/#666:#444/text:Second slide" alt="Second slide [900x500]" data-holder-rendered="true">
									</div>
								<?php endforeach; ?>
							</div>
						</div>
					</div>

				</div>

			<?php endforeach; ?>

		</div>



		

		<div class="col-md-3">
			<p>THÔNG TIN QUẢNG CÁO</p>
		</div>

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

<script>
	jQuery(document).ready(function($) {

		$('#myCarousel').carousel({
			interval: 2000
		});

        //Handles the carousel thumbnails
        $('[id^=carousel-selector-]').click( function(){
        	var id_selector = $(this).attr("id");
        	var id = id_selector.substr(id_selector.length -1);
        	var id = parseInt(id);
        	$('#myCarousel').carousel(id);
        });

    });
</script>
</html>

