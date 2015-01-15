<?php
/**
 * Created by PhpStorm.
 * User: F.U.C.K
 * Date: 15-Jan-15
 * Time: 7:44 PM
 */
class Account_DAO extends  CI_Model{

    private $_id;
    private $_email;
    private $_password;
//    private $

    function __construct(){
        parent::__construct();
    }

    /**
     * Check existing account
     * @param $email
     * @return bool
     */
    function checkAccount($email){
        $this->db->where('email',$email);
        $query=$this->db->get('account');
        if($query->num_rows()>0){
            return true;
        }
        return false;
    }
}
class AccountInfo{
    private $_id;
    private $_email;
    private $_password;

    public function getId()
    {
        return $this->_id;
    }

    public function setId($id)
    {
        $this->_id = $id;
    }

    public function getEmail()
    {
        return $this->_email;
    }

    public function setEmail($email)
    {
        $this->_email = $email;
    }

    public function getPassword()
    {
        return $this->_password;
    }

    public function setPassword($password)
    {
        $this->_password = $password;
    }

}