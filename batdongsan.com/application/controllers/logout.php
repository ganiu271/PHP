<?php if( ! defined('BASEPATH')) exit('No direct script access allowed');

class Logout extends CI_Controller{
	function index(){
		
		$this->session->userdata = array();
		$this->session->sess_destroy();

		$currentPage = $_POST['currentPage'];
		$currentPage = substr($currentPage, 15);
		redirect($currentPage,'refresh');
	}
}