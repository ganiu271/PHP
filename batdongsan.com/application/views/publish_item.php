<!DOCTYPE html>
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>Đăng tin bất động sản</title>

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
			$('#logo1, #logo2').click(function(){
				window.location.replace("<?php echo base_url(); ?>");
			});
			showDistrict();
		});
	</script>

</head>

<?php
$username = $this->session->userdata('username');
if($username == ''){
	$url = base_url();
	redirect($url,'refresh');
}
?>

<body>

	<div class="menu" style="top: 0px;">
		<?php $this->load->view('navbar', 'nhatro'); ?>
	</div>

	<div class="topbarfixed" style="position: fixed; top: 0px; display: block;">
		<img id="logo2" class="btnImg" src="<?php echo base_url(); ?>resource/images/logo2.png"/>
	</div>

	<div class="row content pub_body " >

		<form id="publishForm" method="POST" action="<?php echo base_url(); ?>dang_tin/save" enctype="multipart/form-data">
			<div class="col-md-9">
				<div class="pub_title col-md-12">
					<center>
						Xin chào <?php echo $username;?>
						<h3>ĐĂNG TIN RAO BÁN, CHO THUÊ NHÀ ĐẤT</h3>
						(Quý vị nhập thông tin đầy đủ vào các mục dưới đây)
					</center>
				</div>

				<div class="box pub_basicinfo col-md-12">
					<div class="titlebox col-md-12">
						THÔNG TIN BẤT ĐỘNG SẢN
					</div>

					<div class="detailbox col-md-12">
						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Tiêu đề
							</div>
							<div class="col-md-10">
								<input name="title" id="title" class="mytextbox" type="text" placeholder="Cho thuê nhà trọ...">
							</div>
						</div>

						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Loại bất động sản
							</div>
							<div class="col-md-5">
								<select name="kind" id="kind" class="myselectbox" onchange="ShowTypeOfRealty(this.value)">
									<option value="0">Chọn</option>
									<option value="1">NHÀ TRỌ</option>
									<option value="2">CĂN HỘ</option>
									<option value="3">VĂN PHÒNG / MẶT BẰNG</option>
									<option value="4">NHÀ ĐẤT</option>
								</select>
							</div>
						</div>

						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Hình thức
							</div>
							<div class="col-md-10" id="div_ShowTypeOfRealty">
								<!-- SHOW HERE -->
							</div>
						</div>

						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Địa chỉ
							</div>
							<div class="col-md-10">
								<input name="address" class="mytextbox" type="text" placeholder="Số nhà - tên đường">
							</div>
						</div>

						<div class="publishItem col-md-12">
							<div class="col-md-2">
							</div>
							<div class="col-md-5">
								<select name="province" id="selectbox_province" class="myselectbox" onchange="showDistrict();"  >
									<option value="0">Chọn Tỉnh/Thành phố</option>
									<?php foreach($listProvince as $obj):?>
										<option value="<?php echo $obj->PROVINCEID; ?>"><?php echo $obj->PROVINCENAME; ?></option>
									<?php endforeach; ?>
								</select>
							</div>
							<div class="col-md-5">
								<div id="divShowDistrict">
									<select name="district" class="myselectbox" id="selectbox_district" >
										<option value="0">Chọn Quận/Huyện</option>
									</select>
								</div>
							</div>
						</div>
						
						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Giá
							</div>
							<div class="col-md-5">
								<input name="price" class="mytextbox" type="text" placeholder="2400000000">
							</div>
							<div class="col-md-5">
								<label class="mylabel">2 tỉ 4 trăm triệu đồng</label>
							</div>
						</div>

						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Diện tích
							</div>
							<div class="col-md-10">
								<input name="size" class="mytextbox" type="text" placeholder="110m²">
							</div>
						</div>

						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Mô tả
							</div>
							<div class="col-md-10">
								<textarea name="description" rows="8" class="mytextarea" placeholder="Thông tin mô tả"></textarea>
							</div>
						</div>

						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Hình ảnh mô tả
							</div>
							<div class="col-md-7">
								<input type="file" name="files[]" multiple="" accept="image/*">
							</div>
						</div>
						
					</div>
				</div>

				<div class="box pub_contact col-md-12">
					<div class="titlebox col-md-12">
						THÔNG TIN LIÊN HỆ
					</div>

					<div class="detailbox col-md-12">

						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Tên liên hệ
							</div>
							<div class="col-md-10">
								<input name="contactName" class="mytextbox" type="text" placeholder="Hương Hk">
							</div>
						</div>

						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Số điện thoại
							</div>
							<div class="col-md-10">
								<input name="contactTel" class="mytextbox" type="text" placeholder="0932 789 297	">
							</div>
						</div>

						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Email
							</div>
							<div class="col-md-10">
								<input name="contactEmail" class="mytextbox" type="text" placeholder="huonghk.uit@gmail.com">
							</div>
						</div>

						<div class="publishItem col-md-12">
							<div class="col-md-2">
								Địa chỉ
							</div>
							<div class="col-md-10">
								<input name="contactAddress" class="mytextbox" type="text" placeholder="194 Bạch Đằng, phường 24, quận Bình Thạnh, Tp. HCM">
							</div>
						</div>
						

					</div>
				</div>

				<center>
					<p>&nbsp<br><br></p>
					<button id="btnPublish" type="button" class="btn btn-success btn-lg">ĐĂNG TIN</button>
				</center>
			</div>

		</form>


		<div class="col-md-3">
			THÔNG TIN QUẢNG CÁO
		</div>
	</div>
	<div class="footer row">
		<?php $this->load->view('footer'); ?>
	</div>
</body>

<?php
$districtID = 0; //for ajax show district
?>

<script>
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

function ShowTypeOfRealty(str) {
	if (str == "0") {
		document.getElementById("div_ShowTypeOfRealty").innerHTML = "";
		return;
	} else {
		switch (str) {
			case '1':
			{
				document.getElementById("div_ShowTypeOfRealty").innerHTML = "<input name='type' type='radio' value='Nhà trọ cho thuê'>Nhà trọ cho thuê &nbsp <input name='type' type='radio' value='Tìm người ở ghép'>Tìm người ở ghép";
				return;
				break;
			}
			case '2':
			{
				document.getElementById("div_ShowTypeOfRealty").innerHTML = "<input name='type' type='radio' value='Căn hộ cho thuê'>Căn hộ cho thuê &nbsp <input name='type' type='radio' value='Căn hộ cần bán'>Căn hộ cần bán";
				return;
				break;
			}
			case '3':
			{
				document.getElementById("div_ShowTypeOfRealty").innerHTML = "<input name='type' type='radio' value='Văn phòng cho thuê'>Văn phòng cho thuê &nbsp <input name='type' type='radio' value='Mặt bằng cho thuê'>Mặt bằng cho thuê";
				return;
				break;
			}
			case '4':
			{
				document.getElementById("div_ShowTypeOfRealty").innerHTML = "<input name='type' type='radio' value='Nhà cho thuê'>Nhà cho thuê &nbsp <input name='type' type='radio' value='Nhà đất cần bán'>Nhà đất cần bán";
				return;
				break;
			}

			default:
			break;
		}
	}
}

$(document).ready(function(){
	


	$('#btnPublish').click(function(){
		var title = $('#title').val();
		var kind = $('#kind').val();
		var type = $('#type').val();
		var p_province = $('#selectbox_province').val();
		var p_district = $('#selectbox_district').val();
		if(title == '' || kind == 0 || type == '' || p_province == 0 || p_district == 0)
			alert('Yêu cầu điền đầy đủ thông tin!');
		else{
			document.getElementById('publishForm').submit();	
		}
	});
});


</script>
</html>

