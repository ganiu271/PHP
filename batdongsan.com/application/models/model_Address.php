<?php
class Model_Address extends CI_Model{

	function __construct()
	{
		parent::__construct();
	}

	function getListProvince()
	{
		$sql = "SELECT `province`.`PROVINCEID`, `province`.`PROVINCENAME` FROM `province` ORDER BY `province`.`PROVINCENAME`";
		$query = $this->db->query($sql);
		if ($query->num_rows() >0)
		{
			return $query->result();
		}
		else
		{
			return NULL;
		}
	}

	function getListDistrict()
	{
		$sql = "SELECT `DISTRICTID`, CONCAT(`TYPE`,' ',`DISTRICTNAME`) as `DISTRICTNAME`, `PROVINCEID` FROM `district` ORDER BY CONCAT(`TYPE`,' ',`DISTRICTNAME`)";
		$query = $this->db->query($sql);
		if($query->num_rows() >0 )
		{
			return $query->result();
		}
		else
		{
			return NULL;
		}
	}
	
	function getListDistrictByID($provinceID)
	{
		$sql = "SELECT `DISTRICTID`, CONCAT(`TYPE`,' ',`DISTRICTNAME`) as `DISTRICTNAME`, `PROVINCEID` FROM `district`  where `district`.`PROVINCEID` = '$provinceID' ORDER BY CONCAT(`TYPE`,' ',`DISTRICTNAME`) DESC";
		$query = $this->db->query($sql);
		if($query->num_rows() >0 )
		{
			return $query->result();
		}
		else
		{
			return NULL;
		}
	}
} 
?>