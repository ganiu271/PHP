<?php

//Function start, destroy session
session_start();
if(isset($isLogout))
{
	session_unset(); 
	session_destroy();
	redirect(base_url(),'refresh');

}

	//Function open login form
if (isset($isLogin))
{
	$url = base_url();
	echo "<script>
	$(document).ready(function() 
	{
		$('#logindiv').css('display', 'block');
		$('#opacitybg').css('display', 'block');
		$('#cancel, #opacitybg').click(function() {
			$('#logindiv').hide();
			$('#opacitybg').hide();
			window.location.replace('$url');
		});
});
</script>";
}

//Function message login result
if (isset($resultLogin)) //show message after login
{
	if ($resultLogin) //login success
	{
		echo "<script>alert('Chúc mừng $username đăng nhập thành công!');</script>";
		$_SESSION["username"] = $username;
		$_SESSION["role"] = $role;
		redirect(base_url(), 'refresh');
	}
	else //login fail
	{
		echo "<script>alert('Bạn đã nhập sai tên đăng nhập hoặc mật khẩu. Vui lòng đăng nhập lại!');</script>";
	}
}


//Function show menu login, logout, account info... 
if (!isset($_SESSION['username']))
{
	echo "<script>$(document).ready(function(){		$('#menuLogin').css('display', 'block');		});</script>";
	echo "<script>$(document).ready(function(){		$('#menuLogout').css('display', 'none');		});</script>";
	echo "<script>$(document).ready(function(){		$('#menuPublishItem').css('display', 'none');		});</script>";
	echo "<script>$(document).ready(function(){		$('#menuAccountInfo').css('display', 'none');		});</script>";
}
else
{
	echo "<script>$(document).ready(function(){		$('#menuLogin').css('display', 'none');		});</script>";
	echo "<script>$(document).ready(function(){		$('#menuLogout').css('display', 'block');		});</script>";
	echo "<script>$(document).ready(function(){		$('#menuPublishItem').css('display', 'block');		});</script>";
	echo "<script>$(document).ready(function(){		$('#menuAccountInfo').css('display', 'block');		});</script>";
}


?>