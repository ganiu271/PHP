<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Dang_tin extends CI_Controller {

	public function loadAddress(&$data){
		$this->load->model('model_Address');
		$data['listProvince'] = $this->model_Address->getListProvince();
		$data['listDistrict'] = $this->model_Address->getListDistrict();
	}

	public function index()
	{
		$data = array();

		$this::loadAddress($data);

		$this->load->view('publish_item',$data);
	}

	public function save(){

		$this->load->model('model_Realty');
		$this->model_Realty->publishItem();
		$this->model_Realty->uploadFile();
		redirect(base_url(),'refresh');
	}

}