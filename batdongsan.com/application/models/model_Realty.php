<?php
class Model_Realty extends CI_Model{

	function __construct(){
		parent::__construct();
	}

	function publishItem(){
		$title 			= $this->input->post('title');
		$kindtemp		= $this->input->post('kind');
		switch ($kindtemp) {
			case '1':
			{
				$kind = "Nhà trọ";
				break;
			}
			case '2':
			{			
				$kind = "Căn hộ";
				break;
			}
			case '3':
			{
				$kind = "Văn phòng/Mặt bằng";
				break;
			}
			case '4':
			{
				$kind = "Nhà đất";
				break;
			}
			default:
			break;
		}
		$type 			= $this->input->post('type');
		$description 	= $this->input->post('description');
		$price 			= $this->input->post('price');
		$size			= $this->input->post('size');
		$address 		= $this->input->post('address');
		$province 		= $this->input->post('province');
		$district 		= $this->input->post('district');
		$contactName 	= $this->input->post('contactName');
		$contactTel 	= $this->input->post('contactTel');
		$contactEmail	= $this->input->post('contactEmail');
		$contactAddress 	= $this->input->post('contactAddress');

		$sqlInsertAddress = "INSERT INTO `realty`.`address` (`ADDRESSID`, `DISTRICTID`, `PROVINCEID`, `ADDRESS`) VALUES (NULL, '$district', '$province', '$address');";

		$this->db->query($sqlInsertAddress);

		$sqlSelectAddressID = "SELECT max(`ADDRESSID`) as ADDRESSID from `realty`.`address`";
		$query = $this->db->query($sqlSelectAddressID);
		$result = $query->result();
		foreach ($result as $obj)
		{
			$addressID = $obj->ADDRESSID;
		}
		$accountid = $this->session->userdata('accountid');
		$sqlInsertRealty = "INSERT INTO `realty`(`REALTYID`, `ADDRESSID`, `ACCOUNTID`, `KIND`, `TYPE`, `TITLE`, `PRICE`, `SIZE`, `DESCRIPTION`, `TAG`, `PUBLISHDATE`, `STATUS`, `CONTACTNAME`, `CONTACTTEL`, `CONTACTEMAIL`, `CONTACTADDRESS`) VALUES (NULL, '$addressID', '$accountid', '$kind', '$type', '$title', '$price', '$size', '$description', 'tag', '2015-01-05', 'active', '$contactName', '$contactTel', '$contactEmail', '$contactAddress')";

		$this->db->query($sqlInsertRealty);
	}

	public function getRealtyID(){
		$sql = "SELECT max(`REALTYID`) as REALTYID FROM `realty`";
		$query = $this->db->query($sql);
		$result = $query->result();
		foreach($result as $obj)
		{
			$id = $obj->REALTYID;
		}
		return $id;
	}

	public function uploadFile(){
		if(isset($_FILES['files'])){
			$errors= array();
			foreach($_FILES['files']['tmp_name'] as $key => $tmp_name ){
				$file_name = $key.$_FILES['files']['name'][$key];
				$file_size =$_FILES['files']['size'][$key];
				$file_tmp =$_FILES['files']['tmp_name'][$key];
				$file_type=$_FILES['files']['type'][$key];	
				if($file_size > 2097152){
					$errors[]='File size must be less than 2 MB';
				}
				$realtyID = $this->getRealtyID();
				$sql="INSERT INTO `photo`(`REALTYID`, `URL`) VALUES ('$realtyID','$file_name')";
				$desired_dir="./resource/uphinh/";
				if(empty($errors)==true){
					if(is_dir($desired_dir)==false){
						mkdir("$desired_dir", 0700);
					}
					if(is_dir("$desired_dir/".$file_name)==false){
						move_uploaded_file($file_tmp,"$desired_dir/".$file_name);
					}
					else{								
						$new_dir="$desired_dir/".$file_name.time();
						rename($file_tmp,$new_dir) ;				
					}
					$this->db->query($sql);			
				}
				else{
					print_r($errors);
				}
			}
			if(empty($error)){
				echo "Success";
			}
		}
	}
	
	public function showListHostel($kind, $type, $districtID, $provinceID, $size, $price, $limit, $offset){
		$sql = "SELECT `DISTRICTNAME` FROM `district` WHERE `DISTRICTID` = $districtID";
		$query = $this->db->query($sql);
		$result = $query->result();

		$districtName = '';
		foreach ($result as $obj) {
			$districtName = $obj->DISTRICTNAME;
		}
		$districtName = "%".$districtName;

		$sql = "SELECT `PROVINCENAME` FROM `province` WHERE `PROVINCEID` = $provinceID";
		$query = $this->db->query($sql);
		$result = $query->result();

		$provinceName = '';
		foreach ($result as $obj) {
			$provinceName = $obj->PROVINCENAME;
		}
		$provinceName = "%".$provinceName;

		$sql = "select * from (select realty.*, r.url from realty left join (SELECT * FROM `photo` group by `REALTYID`) as R on realty.realtyid = R.realtyid) as S, address, district, province
		where S.addressid = address.addressid
		and	address.districtid = district.districtid and district.provinceid = province.provinceid
		and S.`KIND` = '$kind' and S.TYPE = '$type'
		and DISTRICTNAME like '$districtName' and PROVINCENAME like '$provinceName' order by realtyid desc
		limit $limit 
		offset $offset";

		$query = $this->db->query($sql);
		$result = $query->result();
		return $result;
	}

	public function total_showListHostel($kind, $type, $districtID, $provinceID, $size, $price)
	{
		$sql = "SELECT `DISTRICTNAME` FROM `district` WHERE `DISTRICTID` = $districtID";
		$query = $this->db->query($sql);
		$result = $query->result();

		$districtName = '';
		foreach ($result as $obj) {
			$districtName = $obj->DISTRICTNAME;
		}
		$districtName = "%".$districtName;

		$sql = "SELECT `PROVINCENAME` FROM `province` WHERE `PROVINCEID` = $provinceID";
		$query = $this->db->query($sql);
		$result = $query->result();

		$provinceName = '';
		foreach ($result as $obj) {
			$provinceName = $obj->PROVINCENAME;
		}
		$provinceName = "%".$provinceName;

		$sql = "SELECT * FROM `address`, district, province, realty
		WHERE
		address.districtid = district.districtid and district.provinceid = province.provinceid and address.addressid = realty.addressid
		and realty.`KIND` = '$kind' and realty.TYPE = '$type'
		and DISTRICTNAME like '$districtName' and PROVINCENAME like '$provinceName' ";

		$query = $this->db->query($sql);
		$result = $query->num_rows();
		return $result;
	}

	public function loadRealtyDetail($id)
	{
		$sql = "SELECT * FROM `realty`, `address`, `district`, `province` where realty.addressid = address.addressid and address.districtid = district.districtid and address.provinceid = province.provinceid and realty.realtyid = $id";
		$query = $this->db->query($sql);
		$result = $query->result();
		return $result;
	}

	public function loadRealtyPhoto($id)
	{
		$sql = "select url from photo inner join realty on photo.realtyid = realty.realtyid where realty.realtyid = $id";
		$query = $this->db->query($sql);
		$result = $query->result();
		return $result;
	}
} 
?>