<?php
class Model_Account extends CI_Model{

	function __construct()
	{
		parent::__construct();
	}

	function encryptIt($q ) {
		$cryptKey  = 'qJB0rGtIn5UB1xG03efyCp';
		$qEncoded      = base64_encode( mcrypt_encrypt( MCRYPT_RIJNDAEL_256, md5( $cryptKey ), $q, MCRYPT_MODE_CBC, md5( md5( $cryptKey ) ) ) );
		return( $qEncoded );
	}

	function checkLogin()
	{
		$username = $this->input->post('txtUsername');
		$password = $this->input->post('txtPassword');
		$password = md5($password);
		if ($username != '' && $password != '')
		{
			$sql = "select * from account where username = '$username' and password = '$password'";
			$query = $this->db->query($sql);
			if ($query->num_rows() > 0)
			{
				return $query->result();
			}
			else
			{
				return 'login_fail';
			}
		}
		else
		{
			return NULL;
		}
	}


	function getListUsername()
	{
		$sql = "select username from account where 1";
		$query = $this->db->query($sql);
		if ($query->num_rows() > 0)
		{
			return $query->result();
		}
		else
		{
			return NULL;
		}
	}

	function checkUsername($username){
		$sql = "SELECT * FROM `account` WHERE `USERNAME` = '$username'";
		$query = $this->db->query($sql);
		if ($query->num_rows() > 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	function insertAccount($username, $password, $email)
	{	
		$sql = "INSERT INTO `account`(`USERNAME`, `PASSWORD`, `EMAIL`, `ROLEID`) VALUES ('$username','$password','$email',3)";
		$this->db->query($sql);
	}

	/*=============================ADMIN===========================*/
	public function checkloginadmin($username, $password)
	{
		$this->db->where('USERNAME', $username);
		$this->db->where('PASSWORD', $password);
		$result = $this->db->get('account');
		if($result->num_rows() > 0)
			return $result->result();
		else
			return false;
	}

	public function loadaccounts()
	{
		$result = $this->db->get('account');
		if($result->num_rows() > 0)
			return $result->result();
		else
			return false;
	}

	public function loadrole()
	{
		$result = $this->db->get('role');
		if($result->num_rows() > 0)
			return $result->result();
		else
			return false;
	}

	public function insert_account_admin($username, $password, $email, $fullname, $sex, $role)
	{
		$data['USERNAME'] = $username;
		$data['PASSWORD'] = $password;
		$data['EMAIL'] = $email;
		$data['FULLNAME'] = $fullname;
		$data['SEX'] = $sex;
		$data['ROLEID'] = $role;

		return $this->db->insert('account', $data);
	}

	public function checkusername1($username)
	{
		$this->db->where('USERNAME', $username);
		$result = $this->db->get('account');
		if($result->num_rows() > 0)
			return true;
		else
			return false;
	}

	public function loadaccountinfo($accountid)
	{
		$this->db->where('ACCOUNTID', $accountid);
		$result = $this->db->get('account');
		if($result->num_rows() > 0)
			return $result->row();
		else
			return false;
	}

	public function updateaccount($accountid, $email, $fullname, $sex, $role)
	{
		$data['EMAIL'] = $email;
		$data['FULLNAME'] = $fullname;
		$data['SEX'] = $sex;
		$data['ROLEID'] = $role;
		$this->db->where('ACCOUNTID', $accountid);
		return $this->db->update('account', $data);
	}

	public function changepassword($accountid, $password)
	{
		$data['PASSWORD'] = $password;
		$this->db->where('ACCOUNTID', $accountid);
		return $this->db->update('account', $data);
	}

	public function total_accounts()
	{
		$result = $this->db->get('account');
		return $result->num_rows();
	}

	public function top5newusers()
	{
		$this->db->order_by('ACCOUNTID', 'desc');
		$this->db->limit(5);
		$result = $this->db->get('account');
		if($result->num_rows())
			return $result->result();
		else
			return false; 
	}

	public function tophighestnews()
	{
		$this->db->select('*, count(realtyid) as totalnews');
		$this->db->join('realty','account.accountid = realty.accountid');
		$this->db->group_by("account.ACCOUNTID");
		$this->db->order_by('totalnews', 'desc'); 
		$result = $this->db->get('account');
		if($result->num_rows())
			return $result->result();
		else
			return false; 
	}
} 
?>