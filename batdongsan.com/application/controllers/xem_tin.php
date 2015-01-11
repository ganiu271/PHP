<?php if( ! defined('BASEPATH')) exit('No direct script access allowed');

class Xem_tin extends CI_Controller{

	public function id($id){
		$this->load->model('model_Realty');
		$data['detail'] = $this->model_Realty->loadRealtyDetail($id);
		$data['photo'] = $this->model_Realty->loadRealtyPhoto($id);
		$this->load->view('detail',$data);
	}
}
?>