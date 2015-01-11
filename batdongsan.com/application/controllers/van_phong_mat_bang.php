<?php if( ! defined('BASEPATH')) exit('No direct script access allowed');

class Van_phong_mat_bang extends CI_Controller{

	function _remap($method){
		if (method_exists($this, $method))
		{
			$this->$method();
		}
		else {
			$this->index($method);
		}
	}

	public function loadAddress(&$data){
		$this->load->model('model_Address');
		$data['listProvince'] = $this->model_Address->getListProvince();
		$data['listDistrict'] = $this->model_Address->getListDistrict();
	}

	public function getValue($name){
		if(!isset($_POST[$name]))
		{
			return 0;
		}
		else
		{
			return $_POST[$name];
		}
	}

	public function post($name){
		if(isset($_POST[$name]))
		{
			return $_POST[$name];
		}
		else
		{
			return '';
		}
	}

	public function showLoginForm(&$data){
		if(isset($_POST['isLogin']))
		{
			$isLogin = $_POST['isLogin'];
		}
		else
		{
			$isLogin = false;
		}
		$data['isLogin'] = $isLogin;
	}

	public function showRegisterForm(&$data){
		if(isset($_POST['isRegister']))
		{
			$isRegister = $_POST['isRegister'];
		}
		else
		{
			$isRegister = false;
		}
		$data['isRegister'] = $isRegister;
	}

	function encryptIt( $q ) {
		$cryptKey  = 'qJB0rGtIn5UB1xG03efyCp';
		$qEncoded      = base64_encode( mcrypt_encrypt( MCRYPT_RIJNDAEL_256, md5( $cryptKey ), $q, MCRYPT_MODE_CBC, md5( md5( $cryptKey ) ) ) );
		return( $qEncoded );
	}

	public function register(&$data){
		$this->load->model('model_Account');

		$usernameRegister = $this::post('usernameRegister');
		$password1 = $this::post('password1');
		$password2 = $this::post('password2');
		$emailRegister = $this::post('emailRegister');
		$isAgree = $this::post('isAgree');

		if($usernameRegister != NULL || $password1 != NULL || $password2 != NULL || $emailRegister != NULL || $isAgree != NULL)
		{
			$isRegister = 'true';

			if(strlen($usernameRegister) < 6) {
				$errUsernameRegister = 'Tên đăng nhập phải dài hơn 5 ký tự';
				$data['errUsernameRegister'] = $errUsernameRegister;
			}
			else
			{
				$x = $this->model_Account->checkUsername($usernameRegister);
				if($x == 1)
				{
					$errUsernameRegister = 'Tên đăng nhập đã có người sử dụng. Vui lòng nhập tên khác!';
					$data['errUsernameRegister'] = $errUsernameRegister;
				}
				
			}

			if(strlen($password1) < 5){
				$errPassword1Register = 'Mật khẩu phải dài hơn 4 ký tự';
				$data['errPassword1Register'] = $errPassword1Register;
			}

			if($password1 != $password2){
				$errPassword2Register = 'Mật khẩu không khớp';
				$data['errPassword2Register'] = $errPassword2Register;
			}
			if($isAgree != 'on'){
				$errAgree = 'Bạn phải đồng ý với điều khoản của website';
				$data['errAgree'] = $errAgree;
			}

			if(strlen($usernameRegister) < 5 || strlen($password1) < 5 || $password1 != $password2 || $isAgree != 'on'){

				$data['usernameRegister'] = $usernameRegister;
				$data['password1'] = $password1;
				$data['password2'] = $password2;
				$data['emailRegister'] = $emailRegister;
				$data['isAgree'] = $isAgree;
			}
			else{
				$password1 = md5($password1);
				$this->model_Account->insertAccount($usernameRegister, $password1, $emailRegister);
				echo "<script>alert('Đăng ký thành công!');</script>";
				$isRegister = false;
			}
			$data['isRegister'] = $isRegister;
		}
	}

	public function requestPublish(&$data){
		if(isset($_POST['isRequestPublish']))
		{
			$isRequestPublish = $_POST['isRequestPublish'];
		}
		else
		{
			$isRequestPublish = false;
		}
		$data['isRequestPublish'] = $isRequestPublish;
	}

	public function index($type){

		$data = array();

		$this::loadAddress($data);
		$this::showLoginForm($data);
		$this::showRegisterForm($data);
		$this::register($data);
		$this::requestPublish($data);

		$this->load->model('model_Realty');

		$provinceID = $this::getValue("province");
		$districtID = $this::getValue("district");
		$size = $this::getValue("size");
		$price = $this::getValue("price");

		$kind = "Văn phòng/Mặt bằng";
		$typeRent ='';
		$typeFindPartner ='';

		if($type == 'van_phong_cho_thue')
		{
			$config['base_url'] = base_url('van_phong_mat_bang/van_phong_cho_thue');
			$typeRent="Văn phòng cho thuê";
			$config['total_rows'] = $this->model_Realty->total_showListHostel($kind, $typeRent, $districtID, $provinceID, $size, $price);
		}
		elseif($type == 'mat_bang_cho_thue')
		{
			$config['base_url'] = base_url('van_phong_mat_bang/mat_bang_cho_thue');
			$typeFindPartner = "Mặt bằng cho thuê";
			$config['total_rows'] = $this->model_Realty->total_showListHostel($kind, $typeFindPartner, $districtID, $provinceID, $size, $price);
		}

		$config['per_page'] = 4; 
		$config['use_page_numbers']=TRUE;
		$config['uri_segment'] = 3;

		$config['full_tag_open'] = '<ul class="pagination">';
		$config['full_tag_close'] = '</ul>';
		$config['prev_link'] = '&lt;';
		$config['prev_tag_open'] = '<li>';
		$config['prev_tag_close'] = '</li>';
		$config['next_link'] = '&gt;';
		$config['next_tag_open'] = '<li>';
		$config['next_tag_close'] = '</li>';
		$config['cur_tag_open'] = '<li class="active"><a href="#">';
		$config['cur_tag_close'] = '</a></li>';
		$config['num_tag_open'] = '<li>';
		$config['num_tag_close'] = '</li>';

		$config['first_tag_open'] = '<li>';
		$config['first_tag_close'] = '</li>';
		$config['last_tag_open'] = '<li>';
		$config['last_tag_close'] = '</li>';

		$config['first_link'] = '&lt;&lt;';
		$config['last_link'] = '&gt;&gt;';

		$this->pagination->initialize($config); 
		$currentPage =  $this->uri->segment(3)?$this->uri->segment(3):1;
		$start = ($currentPage-1)*$config['per_page']; 
		$data['paginations'] = $this->pagination->create_links();

		$data['provinceID'] = $provinceID;
		$data['districtID'] = $districtID;
		$data['size'] = $size;
		$data['price'] = $price;

		$data['menuactive'] = 'vanphong';
		$data['hinhthuc'] = $type;

		$data['listHostelRent'] = $this->model_Realty->showListHostel($kind, $typeRent, $districtID, $provinceID, $size, $price, $config['per_page'], $start);
		$data['listHostelFindPartner'] = $this->model_Realty->showListHostel($kind, $typeFindPartner, $districtID, $provinceID, $size, $price, $config['per_page'], $start);
		
		$this->load->view('office',$data);
	}

	/*public function van_phong_cho_thue1($provinceid, $districtid)
	{
		
		$this->load->model('model_Address');
		$data['listProvince'] = $this->model_Address->getListProvince();
		$data['listDistrict'] = $this->model_Address->getListDistrict();
		
		$this->load->model('model_Account');
		$data['listUsername'] = $this->model_Account->getListUsername();
		
		if(!isset($_POST["page"]))
		{
			$pageNumber = 1;
		}
		else
		{
			$pageNumber = $_POST["page"];
		}
		$numberOfItem = 4;
		$limit = ($pageNumber - 1) * $numberOfItem;

		if(!isset($_POST["size"]))
		{
			$size = 0;
		}
		else
		{
			$size = $_POST["size"];
		}

		if(!isset($_POST["price"]))
		{
			$price = 0;
		}
		else
		{
			$price = $_POST["price"];
		}

		$this->load->model('model_Realty');
		
		$config['base_url'] = base_url('van_phong_mat_bang/van_phong_cho_thue1')."/".$provinceid."/".$districtid;
		$config['total_rows'] = $this->model_Realty->total_showListHostelRent($districtid, $provinceid, $size, $price);
		$config['per_page'] = 4; 
		$config['use_page_numbers']=false;
		$config['uri_segment'] = 5;

		$config['full_tag_open'] = '<ul class="pagination">';
		$config['full_tag_close'] = '</ul>';
		$config['prev_link'] = '&lt;';
		$config['prev_tag_open'] = '<li>';
		$config['prev_tag_close'] = '</li>';
		$config['next_link'] = '&gt;';
		$config['next_tag_open'] = '<li>';
		$config['next_tag_close'] = '</li>';
		$config['cur_tag_open'] = '<li class="active"><a href="#">';
		$config['cur_tag_close'] = '</a></li>';
		$config['num_tag_open'] = '<li>';
		$config['num_tag_close'] = '</li>';

		$config['first_tag_open'] = '<li>';
		$config['first_tag_close'] = '</li>';
		$config['last_tag_open'] = '<li>';
		$config['last_tag_close'] = '</li>';

		$config['first_link'] = '&lt;&lt;';
		$config['last_link'] = '&gt;&gt;';

		$this->pagination->initialize($config); 
		$tranghienhanh = $this->uri->segment(5)?$this->uri->segment(5):1;
		$start = ($tranghienhanh-1)*$config['per_page']; 
		$data['paginations'] = $this->pagination->create_links();


		$data['listHostelRent'] = $this->model_Realty->showListHostelRent($districtid, $provinceid, $size, $price, $config['per_page'], $start);
		
		$data['listHostelFindPartner'] = $this->model_Realty->showListHostelFindPartner();
		$data['provinceID'] = $provinceid;
		$data['districtID'] = $districtid;
		$data['size'] = $size;
		$data['price'] = $price;

		if(isset($_POST['isLogin']))
		{
			$isLogin = $_POST['isLogin'];
		}
		else
		{
			$isLogin = false;
		}
		$data['isLogin'] = $isLogin;
		$data['menuactive'] = 'nhatro';
		$data['hinhthuc'] = 'van_phong_cho_thue';

		$this->load->view('hostel',$data);
	}
	*/

}

?>