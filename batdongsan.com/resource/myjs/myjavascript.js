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
function checkConfirmPassword(){
	var pass1 = $('#password1').val();
	var pass2 = $('#password2').val();

	document.getElementById("showResultCheckConfirmPassword").innerHTML = "";
}
$(document).ready(function(){
	$("#password2").onkeyup(function(e){
		alert('a');
		var pass1 = $('#password1').val();
		var pass2 = $('#password2').val();

		
		if(pass1 != '' && pass2 != ''){
			if(pass1 == pass2){
				document.getElementById("showResultCheckConfirmPassword").innerHTML = "<span style='color: #2d81d7'>Khớp mật khẩu</span>";
				return;
			}
			else{
				document.getElementById("showResultCheckConfirmPassword").innerHTML = "Mật khẩu chưa khớp";
				return;
			}
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