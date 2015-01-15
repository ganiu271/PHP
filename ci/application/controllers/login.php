<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Login extends  CI_Controller{

    public function __construct() {
        parent::__construct();
    }

    public function index(){
        $this->load->view('login');
    }

    public function check(){

//        $this->load->model('Account_DAO');
//        $this->load->library('encrypt');
        $user_name=$this->input->post('email');
        $password=$this->input->post('password');
        echo $user_name.$password;
    }
}
