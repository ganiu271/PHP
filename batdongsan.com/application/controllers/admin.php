<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Admin extends CI_Controller {

	public function __construct()
	{
		parent::__construct();

	}

	public function index()
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_Account');
			$this->load->model('model_News');

			$data['menuactive'] = 'dashboard';
			$data['totalaccounts'] = $this->model_Account->total_accounts();
			$data['totalnews'] = $this->model_News->total_News();
			$data['top5newusers'] = $this->model_Account->top5newusers();
			$data['top5newnews'] = $this->model_News->top5newnews();
			$data['tophighestnews'] = $this->model_Account->tophighestnews();
			$this->load->view('admin/index', $data);
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function login()
	{
		$this->load->model('model_Account');
		$username = $this->input->post('username');
		$password = $this->input->post('password');
		$password = md5($password);
		if($username != '')
		{
			$result = $this->model_Account->checkloginadmin($username, $password);
			if($result != false)
			{
				$sessiondata = array(
					'accountid' => $result[0]->ACCOUNTID,
					'roleid'    => $result[0]->ROLEID,
	                'username'  => $result[0]->USERNAME,
	                'fullname'  => $result[0]->FULLNAME
               	);
				$this->session->set_userdata($sessiondata);
				redirect('admin');
			}
			else
			{
				$data['status'] = "Wrong";
				$this->load->view('admin/adminlogin',$data);
			}
		}
		else
		{
			$data['status'] = "Wrong";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function logout()
	{
		$this->session->sess_destroy();
		redirect('admin');
	}

	public function manage_account()
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_Account');
			$data['accounts'] = $this->model_Account->loadaccounts();
			$data['menuactive'] = 'accounts';
			$this->load->view('admin/accounts', $data);
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function add_account()
	{	
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_Account');
			$data['roles'] = $this->model_Account->loadrole();
			$data['menuactive'] = 'newaccount';
			$this->load->view('admin/addaccount', $data);
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function new_account()
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_Account');
			$username = $this->input->post('username');
			$password = $this->input->post('password');
			$password = md5($password);
			$fullname = $this->input->post('fullname');
			$sex = $this->input->post('sex');
			$email = $this->input->post('email');
			$role = $this->input->post('role');

			$result = $this->model_Account->insert_account_admin($username, $password, $email, $fullname, $sex, $role);
			
			if($result)
			{
				echo "<script>alert('Create account successful');</script>";
				echo "<script>window.location.href = 'manage_account';</script>";
			}
			else
			{
				echo "<script>alert('Fail to create account');</script>";
				echo "<script>window.location.href = 'manage_account';</script>";
			}
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function checkusername($username)
	{
		$this->load->model('model_Account');
		$result = $this->model_Account->checkusername1($username);
		if($result)
			echo "dung";
		else
			echo "sai";
	}

	public function view_account($accountid)
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_Account');
			$data['accountinfo'] = $this->model_Account->loadaccountinfo($accountid);
			$data['roles'] = $this->model_Account->loadrole();
			$data['menuactive'] = 'accounts';
			$this->load->view('admin/editaccount', $data);
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function update_account()
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_Account');
			$accountid = $this->input->post('accountid');
			$fullname = $this->input->post('fullname');
			$sex = $this->input->post('sex');
			$email = $this->input->post('email');
			$role = $this->input->post('role');

			$result = $this->model_Account->updateaccount($accountid, $email, $fullname, $sex, $role);
			if($result)
			{
				echo "<script>alert('Update account successful');</script>";
				echo "<script>window.location.href = 'view_account/$accountid';</script>";
			}
			else
			{
				echo "<script>alert('Fail to update account');</script>";
				echo "<script>window.location.href = 'manage_account';</script>";
			}
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function changepassword()
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_Account');
			$accountid = $this->input->post('accountid');
			$password = $this->input->post('password');
			$password = md5($password);

			$result = $this->model_Account->changepassword($accountid, $password);
			if($result)
			{
				echo "<script>alert('Change password successful');</script>";
				echo "<script>window.location.href = 'view_account/$accountid';</script>";
			}
			else
			{
				echo "<script>alert('Fail to change password');</script>";
				echo "<script>window.location.href = 'manage_account';</script>";
			}
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function manage_news()
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_News');
			$data['News'] = $this->model_News->loadnews();
			$data['menuactive'] = 'news';
			$this->load->view('admin/news', $data);
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function loaddistrict($provinceid)
	{
		$this->load->model('model_News');
		$data['districts'] = $this->model_News->loaddistrictbyprovinceid($provinceid);
		$this->load->view('admin/ajaxdistrict', $data);
	}

	public function add_news()
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_News');
			$data['districts'] = $this->model_News->loaddistrictbyprovinceid(1);
			$data['provinces'] = $this->model_News->loadprovince();
			$data['menuactive'] = 'newnews';
			$this->load->view('admin/addnews', $data);
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function insert_news()
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_News');
			$accountid = $this->session->userdata('accountid');
			$title = $this->input->post('title');
			$kind = $this->input->post('kind');
			$type = $this->input->post('type');
			$price = $this->input->post('price');
			$size = $this->input->post('size');
			$description = $this->input->post('description');
			$contactname = $this->input->post('contactname');
			$contactnumbers = $this->input->post('contactnumber');
			$contactemail = $this->input->post('contactemail');
			$contactaddress = $this->input->post('contactaddress');
			$status = "Available";
			$publishdate = date('Y-m-d');

			$address = $this->input->post('address');
			$district = $this->input->post('district');
			$province = $this->input->post('province');
			$checkaddress = $this->model_News->checkaddress($address, $district, $province);
			if($checkaddress)
			{
				$addressinfo = $this->model_News->loadaddressinfo($address, $district, $province);
				$addressid = $addressinfo->ADDRESSID;
				$result = $this->model_News->insert_news($accountid, $addressid, $kind, $type, $publishdate, $status, $title, $price, $size, $description, $contactname, $contactnumbers, $contactemail, $contactaddress);
				if($result)
				{
					$id = mysql_insert_id();
					//Khai bao bien cau hinh upload
					$config = array();
					//thuc mục chứa file
					$config['upload_path'] = './resource/uphinh/';
					$config['allowed_types'] = 'gif|jpg|png|jpeg';
					//Dung lượng tối đa KB
					$config['max_size']      = '0';
					//Chiều rộng tối đa
					$config['max_width']     = '1028';
					//Chiều cao tối đa
					$config['max_height']    = '1028';
					//load thư viện upload
					//bien chua cac ten file upload
					$name_array = array();

					//lưu biến môi trường khi thực hiện upload
					$file  = $_FILES['userfile'];
					$count = count($file['name']);//lấy tổng số file được upload
					for($i=0; $i<=$count-1; $i++) {

					    $_FILES['userfile']['name']     = time().'-'.$file['name'][$i];  //khai báo tên của file thứ i
					    $_FILES['userfile']['type']     = $file['type'][$i]; //khai báo kiểu của file thứ i
					    $_FILES['userfile']['tmp_name'] = $file['tmp_name'][$i]; //khai báo đường dẫn tạm của file thứ i
					    $_FILES['userfile']['error']    = $file['error'][$i]; //khai báo lỗi của file thứ i
					    $_FILES['userfile']['size']     = $file['size'][$i]; //khai báo kích cỡ của file thứ i
					    //load thư viện upload và cấu hình
					    $this->load->library('upload', $config);
					    //thực hiện upload từng file
					    if($this->upload->do_upload())
					    {
					       $this->model_News->addimage(time().'-'.$file['name'][$i],$id);
					    }     
					}
					echo "<script>alert('Create news successful');</script>";
					echo "<script>window.location.href = 'manage_news';</script>";
				}
				else
				{
					echo "<script>alert('Fail to create news');</script>";
					echo "<script>window.location.href = 'manage_news';</script>";
				}
			}
			else
			{
				$insertaddress = $this->model_News->insert_address($address, $district, $province);
				if($insertaddress)
				{
					$addressid = mysql_insert_id();
					$result = $this->model_News->insert_news($accountid, $addressid, $kind, $type, $publishdate, $status, $title, $price, $size, $description, $contactname, $contactnumbers, $contactemail, $contactaddress);
					if($result)
					{
						$id = mysql_insert_id();
						//Khai bao bien cau hinh upload
						$config = array();
						//thuc mục chứa file
						$config['upload_path'] = './resource/uphinh/';
						$config['allowed_types'] = 'gif|jpg|png|jpeg';
						//Dung lượng tối đa KB
						$config['max_size']      = '0';
						//Chiều rộng tối đa
						$config['max_width']     = '1028';
						//Chiều cao tối đa
						$config['max_height']    = '1028';
						//load thư viện upload
						//bien chua cac ten file upload
						$name_array = array();

						//lưu biến môi trường khi thực hiện upload
						$file  = $_FILES['userfile'];
						$count = count($file['name']);//lấy tổng số file được upload
						for($i=0; $i<=$count-1; $i++) {

						    $_FILES['userfile']['name']     = time().'-'.$file['name'][$i];  //khai báo tên của file thứ i
						    $_FILES['userfile']['type']     = $file['type'][$i]; //khai báo kiểu của file thứ i
						    $_FILES['userfile']['tmp_name'] = $file['tmp_name'][$i]; //khai báo đường dẫn tạm của file thứ i
						    $_FILES['userfile']['error']    = $file['error'][$i]; //khai báo lỗi của file thứ i
						    $_FILES['userfile']['size']     = $file['size'][$i]; //khai báo kích cỡ của file thứ i
						    //load thư viện upload và cấu hình
						    $this->load->library('upload', $config);
						    //thực hiện upload từng file
						    if($this->upload->do_upload())
						    {
						       $this->model_News->addimage(time().'-'.$file['name'][$i],$id);
						    }     
						}
						echo "<script>alert('Create news successful');</script>";
						echo "<script>window.location.href = 'manage_news';</script>";
					}
					else
					{
						echo "<script>alert('Fail to create news');</script>";
						echo "<script>window.location.href = 'manage_news';</script>";
					}

				}
				else
				{
					echo "<script>alert('Fail to create news');</script>";
					echo "<script>window.location.href = 'manage_news';</script>";
				}
			}
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function edit_news($id)
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_News');
			$result = $this->model_News->loadnewsbyid($id);
			$data['districts'] = $this->model_News->loaddistrictbyprovinceid($result->PROVINCEID);
			$data['provinces'] = $this->model_News->loadprovince();
			$data['districtselected'] = $result->DISTRICTID;
			$data['News'] = $result;
			$data['images'] = $this->model_News->loadimage($id);
			$data['menuactive'] = 'news';
			$this->load->view('admin/editnews', $data);
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function update_news()
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_News');
			$realtyid = $this->input->post('realtyid');
			$accountid = $this->input->post('accountid');
			$title = $this->input->post('title');
			$kind = $this->input->post('kind');
			$type = $this->input->post('type');
			$price = $this->input->post('price');
			$size = $this->input->post('size');
			$description = $this->input->post('description');
			$contactname = $this->input->post('contactname');
			$contactnumbers = $this->input->post('contactnumber');
			$contactemail = $this->input->post('contactemail');
			$contactaddress = $this->input->post('contactaddress');
			$status = "Available";

			$address = $this->input->post('address');
			$district = $this->input->post('district');
			$province = $this->input->post('province');
			$checkaddress = $this->model_News->checkaddress($address, $district, $province);
			if($checkaddress)
			{
				$addressinfo = $this->model_News->loadaddressinfo($address, $district, $province);
				$addressid = $addressinfo->ADDRESSID;
				$result = $this->model_News->update_news($realtyid,$accountid, $addressid,$kind, $type, $status, $title, $price, $size, $description, $contactname, $contactnumbers, $contactemail, $contactaddress);
				if($result)
				{
					//Khai bao bien cau hinh upload
					$config = array();
					//thuc mục chứa file
					$config['upload_path'] = './resource/uphinh/';
					$config['allowed_types'] = 'gif|jpg|png|jpeg';
					//Dung lượng tối đa KB
					$config['max_size']      = '0';
					//Chiều rộng tối đa
					$config['max_width']     = '1028';
					//Chiều cao tối đa
					$config['max_height']    = '1028';
					//load thư viện upload
					//bien chua cac ten file upload
					$name_array = array();

					//lưu biến môi trường khi thực hiện upload
					$file  = $_FILES['userfile'];
					$count = count($file['name']);//lấy tổng số file được upload
					for($i=0; $i<=$count-1; $i++) {

					    $_FILES['userfile']['name']     = time().'-'.$file['name'][$i];  //khai báo tên của file thứ i
					    $_FILES['userfile']['type']     = $file['type'][$i]; //khai báo kiểu của file thứ i
					    $_FILES['userfile']['tmp_name'] = $file['tmp_name'][$i]; //khai báo đường dẫn tạm của file thứ i
					    $_FILES['userfile']['error']    = $file['error'][$i]; //khai báo lỗi của file thứ i
					    $_FILES['userfile']['size']     = $file['size'][$i]; //khai báo kích cỡ của file thứ i
					    //load thư viện upload và cấu hình
					    $this->load->library('upload', $config);
					    //thực hiện upload từng file
					    if($this->upload->do_upload())
					    {
					       $this->model_News->addimage(time().'-'.$file['name'][$i],$realtyid);
					    }     
					}
					echo "<script>alert('Update news successful');</script>";
					echo "<script>window.location.href = 'manage_news';</script>";
				}
				else
				{
					echo "<script>alert('Fail to update news');</script>";
					echo "<script>window.location.href = 'manage_news';</script>";
				}
			}
			else
			{
				$insertaddress = $this->model_News->insert_address($address, $district, $province);
				if($insertaddress)
				{
					$addressid = mysql_insert_id();
					$result = $this->model_News->update_news($realtyid,$accountid, $addressid,$kind, $type, $status, $title, $price, $size, $description, $contactname, $contactnumbers, $contactemail, $contactaddress);
					if($result)
					{
						//Khai bao bien cau hinh upload
						$config = array();
						//thuc mục chứa file
						$config['upload_path'] = './resource/uphinh/';
						$config['allowed_types'] = 'gif|jpg|png|jpeg';
						//Dung lượng tối đa KB
						$config['max_size']      = '0';
						//Chiều rộng tối đa
						$config['max_width']     = '1028';
						//Chiều cao tối đa
						$config['max_height']    = '1028';
						//load thư viện upload
						//bien chua cac ten file upload
						$name_array = array();

						//lưu biến môi trường khi thực hiện upload
						$file  = $_FILES['userfile'];
						$count = count($file['name']);//lấy tổng số file được upload
						for($i=0; $i<=$count-1; $i++) {

						    $_FILES['userfile']['name']     = time().'-'.$file['name'][$i];  //khai báo tên của file thứ i
						    $_FILES['userfile']['type']     = $file['type'][$i]; //khai báo kiểu của file thứ i
						    $_FILES['userfile']['tmp_name'] = $file['tmp_name'][$i]; //khai báo đường dẫn tạm của file thứ i
						    $_FILES['userfile']['error']    = $file['error'][$i]; //khai báo lỗi của file thứ i
						    $_FILES['userfile']['size']     = $file['size'][$i]; //khai báo kích cỡ của file thứ i
						    //load thư viện upload và cấu hình
						    $this->load->library('upload', $config);
						    //thực hiện upload từng file
						    if($this->upload->do_upload())
						    {
						       $this->model_News->addimage(time().'-'.$file['name'][$i],$realtyid);
						    }     
						}
						echo "<script>alert('Update news successful');</script>";
						echo "<script>window.location.href = 'manage_news';</script>";
					}
					else
					{
						echo "<script>alert('Fail to update news');</script>";
						echo "<script>window.location.href = 'manage_news';</script>";
					}
				}
				else
				{
					echo "<script>alert('Fail to update news');</script>";
					echo "<script>window.location.href = 'manage_news';</script>";
				}
			}
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

	public function deleteimage($id, $realtyid)
	{
		$roleid = $this->session->userdata('roleid');
		if($roleid == '1')
		{
			$this->load->model('model_News');
			$result = $this->model_News->deleteimage($id);
			if($result)
			{
				redirect('admin/edit_news/'.$realtyid);
			}
		}
		else
		{
			$data['status'] = "";
			$this->load->view('admin/adminlogin',$data);
		}
	}

}

?>