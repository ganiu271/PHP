<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Address extends CI_Controller {

	public function __contruct()
	{
		parent::__construct();
	}

	public function getDistrict()
	{
		$id = $_REQUEST["id"];

		$this->load->model('model_Address');
		$data['listDistrictByID'] = $this->model_Address->getListDistrictByID($id);

		$data['test'] = "addaa";
		
		$this->load->view('ajaxShowDistrict',$data);
	}
}
