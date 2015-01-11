<nav id="navigationbar" class="navbar navbar-default" role="navigation">
	<div class="container-fluid">
		<!-- Collect the nav links, forms, and other content for toggling -->
		<div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
			<ul class="nav navbar-nav">
				<li class="<?php if(isset($menuactive) && $menuactive == 'nhatro') echo "active"; ?> dropdown" class="active">
					<a href="<?php echo base_url(); ?>nha_tro" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-expanded="false">NHÀ TRỌ<span class="caret"></span></a>
					<ul class="dropdown-menu" role="menu">
						<li><a href="<?php echo base_url(); ?>nha_tro/nha_tro_cho_thue" >NHÀ TRỌ CHO THUÊ</a></li>
						<li><a href="<?php echo base_url(); ?>nha_tro/tim_nguoi_o_ghep" >TÌM NGƯỜI Ở GHÉP</a></li>
					</ul>
				</li>
				<li class=" <?php if(isset($menuactive) && $menuactive == 'canho') echo "active"; ?> dropdown">
					<a href="<?php echo base_url(); ?>/can_ho" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-expanded="false">CĂN HỘ <span class="caret"></span></a>
					<ul class="dropdown-menu" role="menu">
						<li><a href="<?php echo base_url(); ?>can_ho/can_ho_cho_thue">CĂN HỘ CHO THUÊ</a></li>
						<li><a href="<?php echo base_url(); ?>can_ho/can_ho_can_ban">CĂN HỘ CẦN BÁN</a></li>
					</ul>
				</li>
				<li class= "<?php if(isset($menuactive) && $menuactive == 'vanphong') echo "active"; ?> dropdown">
					<a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-expanded="false">VĂN PHÒNG / MẶT BẰNG <span class="caret"></span></a>
					<ul class="dropdown-menu" role="menu">
						<li><a href="<?php echo base_url(); ?>van_phong_mat_bang/van_phong_cho_thue">VĂN PHÒNG CHO THUÊ</a></li>
						<li><a href="<?php echo base_url(); ?>van_phong_mat_bang/mat_bang_cho_thue">MẶT BẰNG CHO THUÊ</a></li>
					</ul>
				</li>
				<li class="<?php if(isset($menuactive) && $menuactive == 'nhadat') echo "active"; ?> dropdown">
					<a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-expanded="false">NHÀ ĐẤT <span class="caret"></span></a>
					<ul class="dropdown-menu" role="menu">
						<li><a href="<?php echo base_url(); ?>nha_dat/nha_cho_thue">NHÀ CHO THUÊ</a></li>
						<li><a href="<?php echo base_url(); ?>nha_dat/nha_dat_can_ban">NHÀ ĐẤT CẦN BÁN</a></li>
					</ul>
				</li>
				<li class="<?php if(isset($menuactive) && $menuactive == 'phongthuy') echo "active"; ?>"><a  href="#">PHONG THỦY <span class="sr-only">(current)</span></a></li>
				<li class="<?php if(isset($menuactive) && $menuactive == 'hoidap') echo "active"; ?>"><a  href="#">HỎI ĐÁP <span class="sr-only">(current)</span></a></li>
				<li class="<?php if(isset($menuactive) && $menuactive == 'lienhe') echo "active"; ?>"><a  href="<?php echo base_url();?>site/lien_he">LIÊN HỆ <span class="sr-only">(current)</span></a></li>
			</ul>
		</div><!-- /.navbar-collapse -->
	</div><!-- /.container-fluid -->
</nav>