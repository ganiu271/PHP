<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
</head>
<body>
</body>
</html>
<?php if( ! defined('BASEPATH')) exit('No direct script access allowed');

class Login extends CI_Controller{

	public function index(){

		if(isset($_POST['isLogin']))
		{
			$isLogin = $_POST['isLogin'];
		}
		else
		{
			$isLogin = false;
		}
		$data['isLogin'] = $isLogin;

		$this->load->model('model_Account');
		$result = $this->model_Account->checkLogin();
		if($result != NULL){
			if ($result == 'login_fail')
			{
				$data['login_fail'] = TRUE;
				$data['txtUsername'] = $_POST['txtUsername'];
				$data['txtPassword'] = $_POST['txtPassword'];
				$data['isLogin'] = 'true';
			}
			else
			{
				foreach($result as $obj)
				{
					$username = $obj->USERNAME;
					$accountid = $obj->ACCOUNTID;
					$roldid = $obj->ROLEID;
				}
				$data['accountid'] = $accountid;
				$data['roleid'] = $roldid;
				$data['logged_in'] = TRUE;
				$data['username'] = $username;
				$isLogin = false;
				echo "<script>alert('Chúc mừng $username đã đăng nhập thành công!');</script>";
			}
		}
		$this->session->set_userdata($data);

		$isRequestPublish = $_POST['isRequestPublish'];
		
		if($isRequestPublish == 'true'){
			$url = base_url()."dang_tin";
			redirect($url,'refresh');
		}
		else{
			$currentPage = $_POST['currentPage'];
			$currentPage = substr($currentPage, 15);

			redirect($currentPage,'refresh');
		}
		
	}
}