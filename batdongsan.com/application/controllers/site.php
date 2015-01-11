<?php if( ! defined('BASEPATH')) exit('No direct script access allowed');

class Site extends CI_Controller{

	public function test(){
		$this->load->view('test');
	}

	public function ajaxShowTabContent(){
		$this->load->model('model_Realty');
		if(!isset($_REQUEST["pageNumber"]))
		{
			$pageNumber = 1;
		}
		else
		{
			$pageNumber = $_REQUEST["pageNumber"];
		}
		$numberOfItem = 4;
		$limit = ($pageNumber - 1) * $numberOfItem;

		$data['listHostelRent'] = $this->model_Realty->showListHostelRent($limit, $numberOfItem);
		$data['listHostelFindPartner'] = $this->model_Realty->showListHostelFindPartner();

		$this->load->view('ajaxShowTabContent',$data);
	}

	public function dang_tin(){
		$this->load->model('model_Address');
		$data['listProvince'] = $this->model_Address->getListProvince();
		$data['listDistrict'] = $this->model_Address->getListDistrict();

		$this->load->view('publish_item',$data);
	}

	public function ajaxRegister(){
		$this->load->model('model_Account');
		$data['listUsername'] = $this->model_Account->getListUsername();
		$this->load->view('ajaxCheckUsernameRegister',$data);
	}

	public function ShowDistrict(){
		$id = $_REQUEST["id"];
		$districtID = $_REQUEST["districtID"];

		$this->load->model('model_Address');
		$data['listDistrictByID'] = $this->model_Address->getListDistrictByID($id);
		$data['districtID'] = $districtID;
		
		$this->load->view('ajaxShowDistrict',$data);
	}

	public function lien_he(){	
		$data['menuactive'] = 'lienhe';
		$this->load->view('contact',$data);
	}
}

?>