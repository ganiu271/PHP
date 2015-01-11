<?php
class Model_News extends CI_Model{

	function __construct()
	{
		parent::__construct();
	}

	public function loadnews()
	{
		$this->db->join('account', 'account.accountid = realty.accountid');
		$result = $this->db->get('realty');
		if($result->num_rows() > 0)
			return $result->result();
		else
			return false;
	}

	public function loaddistrictbyprovinceid($provinceid)
	{
		$this->db->where("PROVINCEID",$provinceid);
		$result = $this->db->get('district');
		if($result->num_rows() > 0)
			return $result->result();
		else
			return false;
	}

	public function loadprovince()
	{
		$result = $this->db->get('province');
		if($result->num_rows() > 0)
			return $result->result();
		else
			return false;
	}
	// update contactaddress
	public function insert_news($accountid, $addressid,$kind, $type, $publishdate, $status, $title, $price, $size, $description, $contactname, $contactnumbers, $contactemail, $contactaddress)
	{
		$data['ACCOUNTID'] = $accountid;
		$data['ADDRESSID'] = $addressid;
		$data['TITLE'] = $title;
		$data['KIND'] = $kind;
		$data['TYPE'] = $type;
		$data['PUBLISHDATE'] = $publishdate;
		$data['STATUS'] = $status;
		$data['PRICE'] = $price;
		$data['SIZE'] = $size;
		$data['DESCRIPTION'] = $description;
		$data['CONTACTNAME'] = $contactname;
		$data['CONTACTTEL'] = $contactnumbers;
		$data['CONTACTEMAIL'] = $contactemail;
		$data['CONTACTADDRESS'] = $contactaddress;

		return $this->db->insert('realty', $data);
	}

	public function insert_address($address, $district, $province)
	{
		$data['ADDRESS'] = $address;
		$data['DISTRICTID'] = $district;
		$data['PROVINCEID'] = $province;

		return $this->db->insert('address', $data);
	}

	public function loadnewsbyid($newsid)
	{
		$this->db->join('account', 'account.accountid = realty.accountid');
		$this->db->join('address', 'address.addressid = realty.addressid');
		$this->db->where('REALTYID', $newsid);
		$result = $this->db->get('realty');
		if($result->num_rows() > 0)
			return $result->row();
		else
			return false;
	}

	public function checkaddress($address, $district, $province)
	{
		$this->db->where('ADDRESS', $address);
		$this->db->where('DISTRICTID', $district);
		$this->db->where('PROVINCEID', $province);

		$result = $this->db->get('address');
		if($result->num_rows() > 0)
			return true;
		else
			return false;
	}

	public function loadaddressinfo($address, $district, $province)
	{
		$this->db->where('ADDRESS', $address);
		$this->db->where('DISTRICTID', $district);
		$this->db->where('PROVINCEID', $province);

		$result = $this->db->get('address');
		if($result->num_rows() > 0)
			return $result->row();
		else
			return false;
	}
	//update contact address
	public function update_news($realtyid,$accountid, $addressid,$kind, $type, $status, $title, $price, $size, $description, $contactname, $contactnumbers, $contactemail, $contactaddress)
	{
		$data['ACCOUNTID'] = $accountid;
		$data['ADDRESSID'] = $addressid;
		$data['TITLE'] = $title;
		$data['KIND'] = $kind;
		$data['TYPE'] = $type;
		$data['STATUS'] = $status;
		$data['PRICE'] = $price;
		$data['SIZE'] = $size;
		$data['DESCRIPTION'] = $description;
		$data['CONTACTNAME'] = $contactname;
		$data['CONTACTTEL'] = $contactnumbers;
		$data['CONTACTEMAIL'] = $contactemail;
		$data['CONTACTADDRESS'] = $contactaddress;
		$this->db->where('REALTYID', $realtyid);
		return $this->db->update('realty', $data);
	}

	public function total_News()
	{
		$result = $this->db->get('realty');
		return $result->num_rows();
	}

	public function top5newnews()
	{
		$this->db->join('account','account.accountid = realty.accountid');
		$this->db->order_by('REALTYID', 'desc');
		$this->db->limit(5);
		$result = $this->db->get('realty');
		if($result->num_rows())
			return $result->result();
		else
			return false; 
	}

	public function addimage($imagename, $realtyid)
	{
		$data['URL'] = $imagename;
		$data['REALTYID'] = $realtyid;
		return $this->db->insert('photo', $data);
	}

	public function loadimage($realtyid)
	{
		$this->db->where('REALTYID', $realtyid);
		$result = $this->db->get('photo');
		if($result->num_rows() > 0)
			return $result->result();
		else
			return false;
	}

	public function deleteimage($id)
	{
		$this->db->where('PHOTOID', $id);
		return $this->db->delete('photo');
	}
	
}

?>