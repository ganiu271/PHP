<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>Nhà trọ</title>


	<link rel="stylesheet" href="<?php echo base_url();?>resource/bootstrap/css/bootstrap.min.css">
	<link rel="stylesheet" href="<?php echo base_url();?>resource/bootstrap/css/bootstrap-theme.min.css">
	<link rel="stylesheet" href="<?php echo base_url();?>resource/mycss/mystyle.css">

	<script src="<?php echo base_url();?>resource/bootstrap/jquery.min.js"></script>
	<script src="<?php echo base_url();?>resource/bootstrap/js/bootstrap.min.js"></script>
	<script src="<?php echo base_url();?>resource/myjs/myjavascript.js"></script>

	<link rel="stylesheet" href="<?php echo base_url();?>resource/bootstrap/css/font-awesome.css">
	<link rel="stylesheet" href="<?php echo base_url();?>resource/mycss/menuSideBarStyle.css">
	
	<script>

		$(document).ready(function(){
			$(window).bind('scroll', function() {
				var navHeight = 20;
				if ($(window).scrollTop() > navHeight) {
					$('.topbar').hide();
					$('.topbarfixed').show();
					$('.menu').css('top','0px');
				}
				else {
					$('.topbar').show();
					$('.topbarfixed').hide();
					$('.menu').css('top','80px');
				}
			});
		});

		$(document).ready(function(){
			$('#logo1, #logo2').click(function(){
				window.location.replace("<?php echo base_url(); ?>");
			});
			showDistrict();
		});
	</script>
</head>

<?php
$username = $this->session->userdata('username');
$role = $this->session->userdata('role');

if($this->session->userdata('isLogin') != NULL)
	$isLogin = $this->session->userdata('isLogin');

$logged_in = $this->session->userdata('logged_in');
$login_fail = $this->session->userdata('login_fail');
$txtUsername = $this->session->userdata('txtUsername');
$txtPassword = $this->session->userdata('txtPassword');

?>

<body>																							
	
	<div class="menu">
		<?php $this->load->view('navbar', $menuactive); ?>
	</div>

	<div class="topbar">
		<form id="login" action="<?php echo $_SERVER['REQUEST_URI'] ;?>" method="POST">
			<a id="menuLogin" <?php if($logged_in == TRUE) echo "style = 'display: none'" ?> onclick="this.parentNode.submit()" href="#">ĐĂNGNHẬP</a>
			<input type="hidden" name="isLogin" value="true" />
			<input type="hidden" name="isRequestPublish" value="<?php echo $isRequestPublish; ?>">
		</form>
		<form action="<?php echo $_SERVER['REQUEST_URI'] ;?>" method="POST">
			<a id="menuRegister" <?php if($logged_in == TRUE) echo "style = 'display: none'" ?> onclick="this.parentNode.submit()" href="#">ĐĂNG KÝ</a>
			<input type="hidden" name="isRegister" value="true" />
		</form>
		<form action="<?php echo base_url() ;?>logout" method="POST">
			<a id="menuLogout" <?php if($logged_in == TRUE) echo "style = 'display: block'" ?> onclick="this.parentNode.submit()" href="#">ĐĂNGXUẤT</a>
			<input type="hidden" name="currentPage" value="<?php echo $_SERVER['REQUEST_URI'] ;?>" />
		</form>
		<a id="menuAccountInfo" <?php if($logged_in == TRUE) echo "style = 'display: block'" ?> href="#"><span class="menutext">Xin chào <?php echo $username; ?></span></a>
		
		<form action="<?php echo $_SERVER['REQUEST_URI'] ;?>" method="POST">
			<input type="hidden" name="isRequestPublish" value="true" />
			<button id="menuPublishItem" type="submit" class="btn btn-info btn-lg" onclick="requestPublish();"><span style="">ĐĂNG TIN RAO</span></button>
		</form>

		<img id="logo1" class="btnImg" src="<?php echo base_url(); ?>resource/uphinh/logo.png"/>
	</div>

	<div class="topbarfixed">
		<img id="logo2" class="btnImg" src="<?php echo base_url(); ?>resource/uphinh/logo2.png"/>
	</div>

	<div class="div_absolute">
		<form action="<?php echo $_SERVER['REQUEST_URI'] ;?>" method="POST">
			<input type="hidden" name="isLogin" value="false" />
			<div id="opacitybg" onclick="this.parentNode.submit()" <?php if($isLogin == 'true' || $isRegister == 'true') echo "style = 'display: block'"; else echo "style = 'display: none'" ; ?> ></div>
		</form>

		<div id="logindiv" <?php if($isLogin == 'true') echo "style = 'display: block'"; else echo "style = 'display: none'" ;?>  >
			<form action="<?php echo $_SERVER['REQUEST_URI'] ;?>" method="POST">
				<input type="hidden" name="isLogin" value="false" />
				<img id="cancel" onclick="this.parentNode.submit()" class="btnImg" style="float: right; margin-top: 5px; margin-right: 5px; height: 20px;>" src="<?php echo base_url();?>resource/uphinh/btnCancel.png"/>
			</form>

			<form id="formLogin" method="post" action="<?php echo base_url();?>login">
				<input style="display:none" type="text" name="fakeusernameremembered"/>
				<input style="display:none" type="password" name="fakepasswordremembered"/>
				<input type="hidden" name="isRequestPublish" value="<?php echo $isRequestPublish; ?>">

				<input type="hidden" name="currentPage" value="<?php echo $_SERVER['REQUEST_URI'] ;?>" />

				<div style="margin-bottom: 20px;">
					<center><span style="font-weight: bold; font-size: 30px;">ĐĂNG NHẬP</span></center>
				</div>

				<div style="color: red; margin-bottom: 10px; font-style: italic">
					<?php if($login_fail == TRUE) echo "Sai tên đăng nhập hoặc mật khẩu."; ?>
				</div>
				
				<div class="input-group transparent" style="margin-bottom: 10px;">
					<span class="input-group-addon ">
						<i class="fa fa-user"></i>
					</span>
					<input name='txtUsername' type="text" class="form-control" placeholder="Tên đăng nhập" value="<?php echo $txtUsername; ?>">
				</div>
				
				<div class="input-group transparent" style="margin-bottom: 30px;">
					<span class="input-group-addon ">
						<i class="fa fa-unlock-alt"></i>
					</span>
					<input name='txtPassword' type="password" class="form-control" placeholder="Mật khẩu" value="<?php echo $txtPassword; ?>">
				</div>

				<center>
					<button id="btnLogin" type="submit" class="btn btn-success btn-cons"><span style="font-weight: bold">Đăng nhập</span></button>
					<br><br>
					<span style="font-size: 13px;">Quên mật khẩu?</span>
					<a href="#"><span style="font-size: 13px;">Bấm vào đây để lấy lại mật khẩu <i class="fa fa-angle-double-right"></i> </span></a>
					<hr style="border-top: 2px dotted #f2f2f2;">
				</center>

				<div>
					<center><span>Hoặc đăng nhập bằng</span></center>
					<br>
					<div class="col-md-4">
						<center>
							<img class="imgBtn"  src="<?php echo base_url();?>resource/uphinh/iconFB.png"/>
						</center>
					</div>
					<div class="col-md-4">
						<center>
							<img  class="imgBtn"  src="<?php echo base_url();?>resource/uphinh/iconGoogle.png"/>
						</center>
					</div>
					<div class="col-md-4">
						<center>
							<img class="imgBtn" src="<?php echo base_url();?>resource/uphinh/iconTwitter.png"/>
						</center>
					</div>

					<div class="col-md-12">
						<p>&nbsp<hr style="border-top: 2px dotted #f2f2f2;"></p>
					</div>
				</div>

				<center>
					<span style="font-size: 13px;">Chưa có tài khoản?</span>
					<a href="#"><span style="font-size: 15px; font-weight: bold">ĐĂNG KÝ <i class="fa fa-angle-double-right"></i> </span></a>
				</center>
			</form>
		</div>

		<div id="registerdiv" <?php if($isRegister == 'true') echo "style = 'display: block'"; else echo "style = 'display: none'" ;?>>
			<form action="<?php echo $_SERVER['REQUEST_URI'] ;?>" method="POST">
				<input type="hidden" name="isRegister" value="false" />
				<img id="cancelregister" onclick="this.parentNode.submit()" class="btnImg" style="float: right; margin-top: 5px; margin-right: 5px; height: 20px;>" src="<?php echo base_url();?>resource/uphinh/btnCancel.png"/>
			</form>

			<form id="formRegister" method="post" action="<?php echo $_SERVER['REQUEST_URI'] ;?>" style="padding: 20px;">
				<div style="margin-bottom: 20px;">
					<center><span style="font-weight: bold; font-size: 30px;">ĐĂNG KÝ TÀI KHOẢN</span></center>
				</div>

				<div class="regItem col-md-12">
					<div class="col-md-4">
						Tên đăng nhập
					</div>
					<div class="col-md-8">
						<input name="usernameRegister" id="usernameRegister" class="mytextbox" type="text" placeholder="" onkeyup="checkUsernameRegister(this.value);" value="<?php if(isset($usernameRegister)) echo $usernameRegister; ?>">
					</div>

				</div>

				<div class="col-md-12">
					<div class="col-md-4">
					</div>
					<div class="col-md-8" id="showResultCheckUsernameRegister" style="color: red; font-style: italic;">
						<?php if(isset($errUsernameRegister)) echo $errUsernameRegister; ?>
					</div>
				</div>

				<div class="regItem col-md-12">
					<div class="col-md-4">
						Mật khẩu
					</div>
					<div class="col-md-8">
						<input name="password1" id="password1" class="mytextbox" type="password" placeholder="" onkeyup="checkLenghtOfPassword();" value="<?php if(isset($password1)) echo $password1; ?>">
					</div>
				</div>

				<div class="col-md-12">
					<div class="col-md-4">
					</div>
					<div class="col-md-8" id="showResultCheckLenghtOfPassword" style="color: red; font-style: italic;">
						<?php if(isset($errPassword1Register)) echo $errPassword1Register; ?>
					</div>
				</div>

				<div class="regItem col-md-12">
					<div class="col-md-4">
						Nhập lại mật khẩu
					</div>
					<div class="col-md-8">
						<input name="password2" id="password2" class="mytextbox" type="password" placeholder="" onkeyup="checkConfirmPassword();" value="<?php if(isset($password2)) echo $password2; ?>">
					</div>
				</div>

				<div class="col-md-12">
					<div class="col-md-4">
					</div>
					<div class="col-md-8" id="showResultCheckConfirmPassword" style="color: red; font-style: italic;">
						<?php if(isset($errPassword2Register)) echo $errPassword2Register; ?>
					</div>
				</div>

				<div class="regItem col-md-12">
					<div class="col-md-4">
						
					</div>
					<div class="col-md-8">
						<input name="emailRegister" class="mytextbox" type="hidden" placeholder="" value="<?php if(isset($emailRegister)) echo $emailRegister; ?>">
					</div>
				</div>

				<div class="regItem col-md-12">
					<center style="margin: 10px;">
						<hr style="border-top: 2px dotted #f2f2f2; background-color: white;">
						<input name="isAgree" type="checkbox" <?php if(isset($isAgree) && $isAgree=='on') echo "checked"; ?>> Tôi đã đọc và đồng ý với <a style="cursor: pointer">điều khoản</a> của website
					</center>
					<center  style="color: red; font-style: italic;">
						<?php if(isset($errAgree)) echo "$errAgree"; ?>
					</center>
				</div>

				<div class="regItem col-md-12">
					<center>
						<button id="btnRegister" type="submit" class="btn btn-success btn-cons btn-medium"><span style="font-weight: bold">Đăng ký</span></button>
					</center>
				</div>
			</form>
		</div>
	</div>

	<div class="content row" >

		<div class="col-md-2 sidebar box">
			<div class="nav-side-menu">

				<div class="brand">CHỌN KHU VỰC</div>
				<i class="fa fa-bars fa-2x toggle-btn" data-toggle="collapse" data-target="#menu-content"></i>

				<div class="menu-list">
					<form id="MainMenu" method="post" action="<?php echo base_url()."nha_tro/nha_tro1" ; ?>">
						<ul id="menu-content" class="menu-content collapse out">
							<?php $x = 0; ?>
							<?php foreach($listProvince as $provinceObj): ?>
								<?php $x += 1; ?>
								<li  data-toggle="collapse" data-target="#<?php echo $x ?>" class="collapsed <?php if($provinceID == $provinceObj->PROVINCEID) echo "active" ?>">
									<a href="#"><i class="fa fa-star"></i><?php echo $provinceObj->PROVINCENAME ?><span class="arrow"></span></a>
								</li>
								<ul class="sub-menu <?php if($provinceID == $provinceObj->PROVINCEID) echo "collapsed"; else echo "collapse"; ?>" id="<?php echo $x ?>">
									<?php foreach($listDistrict as $districtObj):  ?>
										<?php if($districtObj->PROVINCEID == $provinceObj->PROVINCEID): ?>
											<li class="<?php if($districtID == $districtObj->DISTRICTID) echo "active"; ?>"><a href="<?php echo base_url()."nha_tro/nha_tro_cho_thue1/".$provinceObj->PROVINCEID."/".$districtObj->DISTRICTID ; ?>"><?php echo $districtObj->DISTRICTNAME ; ?></a></li>
										<?php endif; ?>
									<?php endforeach; ?>
								</ul>
							<?php endforeach; ?>
						</ul>
					</form>
				</div>
			</div>
		</div>

		<div class="col-md-8">
			<form id="tab1" action="<?php echo base_url(); ?>nha_tro/nha_tro_cho_thue" method="POST">
			</form>
			<form id="tab2" action="<?php echo base_url(); ?>nha_tro/tim_nguoi_o_ghep" method="POST">
			</form>

			<ul class="nav nav-tabs" role="tablist" id="myTab">
				<li role="presentation" <?php if ($hinhthuc == "nha_tro_cho_thue") echo "class='active'" ; ?>  > 
					<a id="contenttab1" href="#hostelRentTab" aria-controls="home" role="tab" data-toggle="tab">Nhà trọ cho thuê</a>
				</li>
				<li role="presentation" <?php if ($hinhthuc == "tim_nguoi_o_ghep") echo "class='active'" ; ?>> 
				<a id="contenttab2" href="#hostelFindPartnerTab" aria-controls="profile" role="tab" data-toggle="tab">Tìm người ở ghép</a>
				</li>
			</ul>

			<div class="tab-content">

				<div role="tabpanel" class="tab-pane <?php if ($hinhthuc == 'nha_tro_cho_thue') echo 'active'; ?>" id="hostelRentTab">
					<div id="showContentDefault">
						<?php foreach($listHostelRent as $obj): ?>
							<div class="row navtabitem">
								<div class="col-md-2">
									<?php if($obj->url != '0') $imgURL = $obj->url; else $imgURL = "logo.PNG"; ?>
									<img class="headImage" src=<?php echo base_url()."resource/uphinh/".$imgURL ; ?> alt="">
								</div>
								<div class="col-md-10 besideImage">
									<div class="col-md-12">
										<a href="<?php echo base_url(); ?>xem_tin/id/<?php echo $obj->REALTYID; ?>"><span class="item_link">
											<?php echo $obj->TITLE; ?>
										</span></a>
									</div>
									<div class="col-md-12 publishdate">
										<span class="pull-right">
											Đăng ngày: <?php echo $obj->PUBLISHDATE?>
										</span>
									</div>
									<div class="col-md-2">
										Giá
									</div>
									<div class="col-md-10">
										: <?php echo $obj->PRICE; ?>
									</div>
									<div class="col-md-2">
										Diện tích
									</div>
									<div class="col-md-10">
										: <?php echo $obj->SIZE; ?>
									</div>
									<div class="col-md-2">
										Địa chỉ
									</div>
									<div class="col-md-10">
										: <?php echo $obj->ADDRESS.' - '.$obj->DISTRICTNAME.' - '.$obj->PROVINCENAME ?>
									</div>

								</div>
							</div>
						<?php endforeach; ?>
					</div>
					<div id="showTabContent">
						<!-- ajax show here -->
					</div>

					<center>
						<nav>
							<?php echo $paginations; ?>
						</nav>
					</center>

				</div>

				<div role="tabpanel" class="tab-pane <?php if ($hinhthuc == 'tim_nguoi_o_ghep') echo 'active'; ?>" id="hostelFindPartnerTab" >
					<?php foreach($listHostelFindPartner as $obj): ?>
						<div class="row navtabitem">
							<div class="col-md-2">
								<?php if($obj->url != '0') $imgURL = $obj->url; else $imgURL = "logo.PNG"; ?>
								<img class="headImage" src=<?php echo base_url()."resource/uphinh/".$imgURL ; ?> alt="">
							</div>
							<div class="col-md-10">
								<div class="col-md-12">
									<a href="<?php echo base_url(); ?>xem_tin/id/<?php echo $obj->REALTYID; ?>"><span class="item_link">
										<?php echo $obj->TITLE; ?>
									</span></a>
								</div>
								<div class="col-md-12 publishdate">
									<span class="pull-right">
										Đăng ngày: <?php echo $obj->PUBLISHDATE?>
									</span>
								</div>
								<div class="col-md-2">
									Giá
								</div>
								<div class="col-md-10">
									: <?php echo $obj->PRICE; ?>
								</div>
								<div class="col-md-2">
									Diện tích
								</div>
								<div class="col-md-10">
									: <?php echo $obj->SIZE; ?>
								</div>
								<div class="col-md-2">
									Địa chỉ
								</div>
								<div class="col-md-10">
									: <?php echo $obj->ADDRESS.' - '.$obj->DISTRICTNAME.' - '.$obj->PROVINCENAME ?>
								</div>

							</div>
						</div>
					<?php endforeach; ?>

					<center>
						<nav>
							<?php echo $paginations; ?>
						</nav>
					</center>
				</div>
			</div>
		</div>

		<div class="col-md-2 box">

			<form method="post" action="<?php echo $_SERVER['REQUEST_URI'] ;?>">
				<div class="searchbox_title">TÌM KIẾM THEO</div>
				<div class="searchbox_detail col-md-12">
					<div class="searchbox_item col-md-12">
						<select name="province" id="selectbox_province" class="myselectbox" onchange="showDistrict();"  >
							<option value="0">Chọn Tỉnh/Thành phố</option>
							<?php foreach($listProvince as $obj):?>
								<option <?php if( $provinceID == $obj->PROVINCEID) echo "selected";?> value="<?php echo $obj->PROVINCEID; ?>"><?php echo $obj->PROVINCENAME; ?></option>
							<?php endforeach; ?>
						</select>
					</div>

					<div class="searchbox_item col-md-12" >
						<div id="divShowDistrict">
							<select name="district" class="myselectbox" id="selectbox_district" >
								<option value="0">Chọn Quận/Huyện</option>
							</select>
						</div>
					</div>

					<div class="searchbox_item col-md-12">
						<select name="size" id="selectbox_size" class="myselectbox"  >
							<option value="0">Chọn diện tích</option>
						</select>
					</div>

					<div class="searchbox_item col-md-12">
						<select name="price" id="selectbox_price" class="myselectbox"  >
							<option value="0">Chọn mức giá</option>
						</select>
					</div>

					<div class="searchbox_item col-md-12 search_item_bottom">
						<center>
							<button type="submit" class="btn btn-success btn-medium" >TÌM KIẾM</button>
						</center>
					</div>
				</div>
			</form>
		</div>
	</div>

	<div class="footer row">
		<div class="col-md-12">
			<p class="footerDetail">
				<a style="cursor: pointer; font-weight: bold; text-decoration: none;" href="<?php echo base_url();?>">UIT REALTY</a> &nbsp  &nbsp
				Hotline: 0932 789 297 - 
				Email: huonghk.uit@gmail.com
			</p>
		</div>
	</div>

</body>


<?php
if($isRequestPublish == 'true' && $username == '' && $isLogin != 'true'){ 

	echo "<script>var r = confirm('Yêu cầu đăng nhập! Bạn có muốn ĐĂNG NHẬP ngay bây giờ không?');
	if (r == true) {
		document.getElementById('login').submit();	
	}</script>";
}
elseif($isRequestPublish == 'true' && $username != ''){
	$url = base_url()."dang_tin";
	redirect($url,'refresh');
}
?>

<script>
//JQuery for navtabs
$('#myTab a').click(function (e) {
	e.preventDefault();
	$(this).tab('show');
})

//JQuery show Register Form
$(document).ready(function() 
{
	$('#menuRegister').click(function(){
		$('#registerdiv').css('display', 'block');
		$('#opacitybg').css('display', 'block');
		$('#cancelregister, #opacitybg').click(function() {
			window.location.replace('http://stackoverflow.com/questions/1034621/get-current-url-in-web-browser');
		});
	});
	$('#contenttab1').click(function(){
		document.getElementById('tab1').submit();	
	});
	$('#contenttab2').click(function(){
		document.getElementById('tab2').submit();	
	});

});

//Ajax check username on Register Form
function checkUsernameRegister(str) {
	if (str == "") {
		document.getElementById("showResultCheckUsernameRegister").innerHTML = "";
		return;
	} else {
		if (window.XMLHttpRequest) {
			xmlhttp = new XMLHttpRequest();
		} else {
			xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
		}
		xmlhttp.onreadystatechange = function() {
			if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
				document.getElementById("showResultCheckUsernameRegister").innerHTML = xmlhttp.responseText;
			}
		}
		xmlhttp.open("GET","<?php echo base_url(); ?>site/ajaxRegister?q="+str,true);
		xmlhttp.send();
	}
}

//JQuery check leght of password on Register Form
function checkLenghtOfPassword(){
	var str = $('#password1').val();
	if (str == "") {
		document.getElementById("showResultCheckLenghtOfPassword").innerHTML = "";
		return;
	}
	else {
		var n = str.length;
		if(n < 5){
			document.getElementById("showResultCheckLenghtOfPassword").innerHTML = "Mật khẩu phải dài hơn 4 ký tự";
			return;
		}
		else
		{
			document.getElementById("showResultCheckLenghtOfPassword").innerHTML = "<span style='color: #2d81d7'>Mật khẩu hợp lệ</span>";
			return;
		}
	}
}

//JQuery check password confirm on Register Form
$(document).ready(function(){
	$("#password2, #password1").keyup(function(){
		var password1 = $('#password1').val();
		var password2 = $('#password2').val();
		if(password1 == password2){
			document.getElementById("showResultCheckConfirmPassword").innerHTML = "<span style='color: #2d81d7'>Khớp mật khẩu</span>";
			return;
		} 
		else{
			document.getElementById("showResultCheckConfirmPassword").innerHTML = "Mật khẩu chưa khớp";
			return;
		}
	});
});


//Ajax show District by Province
function showDistrict(){

	var id = $('#selectbox_province').val();

	if (window.XMLHttpRequest) {
		xmlhttp = new XMLHttpRequest();
	} else {
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}
	xmlhttp.onreadystatechange = function() {
		if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			document.getElementById("divShowDistrict").innerHTML = xmlhttp.responseText;
		}
	}
	var districtID = <?php echo $districtID; ?>;
	xmlhttp.open("GET","<?php echo base_url(); ?>site/ShowDistrict?id="+id+"&districtID="+districtID,true);
	xmlhttp.send();
}


//JQuery remove white space on Register Form
$(document).ready(function(){
	$("#usernameRegister").on({
		keydown: function(e) {
			if (e.which === 32)
			{
				document.getElementById("showResultCheckUsernameRegister").innerHTML = "Không được nhập khoảng trắng";
				return false;
			}
		},
		change: function() {
			this.value = this.value.replace(/\s/g, "");
		}
	});
});

</script>

</html>

