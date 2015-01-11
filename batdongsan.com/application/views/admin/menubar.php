<div class="page-sidebar-wrapper">
	<div class="page-sidebar navbar-collapse collapse">
		<!-- BEGIN SIDEBAR MENU -->
		<ul class="page-sidebar-menu page-sidebar-menu-hover-submenu " data-keep-expanded="false" data-auto-scroll="true" data-slide-speed="200">
			<li class="start <?php if($menuactive == 'dashboard') echo "active"; ?>">
				<a href="<?php echo base_url(); ?>admin">
				<i class="icon-home"></i>
				<span class="title">Dashboard</span>
				</a>
			</li>
			
			<li class="<?php if($menuactive == 'accounts') echo "active"; else if($menuactive == 'newaccount') echo "active open"; ?>">
				<a href="<?php echo base_url(); ?>admin/manage_account">
				<i class="icon-users"></i>
				<span class="title">Accounts</span>
				</a>
				<ul class="sub-menu">
					<li class="<?php if($menuactive == 'newaccount') echo "active"; ?>">
						<a href="<?php echo base_url(); ?>admin/add_account">
						Add Account</a>
					</li>
				</ul>
			</li>

			<li class="<?php if($menuactive == 'news') echo "active"; else if($menuactive == 'newnews') echo "active open"; ?>">
				<a href="<?php echo base_url(); ?>admin/manage_news">
					<i class="fa fa-newspaper-o"></i>
					<span class="title">News</span>
				</a>
				<ul class="sub-menu">
					<li class="<?php if($menuactive == 'newnews') echo "active"; ?>">
						<a href="<?php echo base_url(); ?>admin/add_news">
						Add News</a>
					</li>
				</ul>
			</li>
			
		</ul>
		<!-- END SIDEBAR MENU -->
	</div>
</div>