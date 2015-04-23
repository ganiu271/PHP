<?php namespace App\Http\Controllers;

use App\Http\Requests;
use App\Http\Controllers\Controller;

use App\User;
use Illuminate\Http\Request;

class UserController extends Controller {

    public function __construct()
    {
        $this->middleware('auth');
    }

	public function index()
	{
        if(\Auth::user()->role!=999){
            return redirect('/')->with('message', 'No permission');
        }
        return view('list');
    }

	public function create()
	{
        $data['result'] = 'fail';
        $input = \Request::all();
        if(\Auth::user()->role==999){
            $user = new User();
            $user->email = isset($input['email'])?$input['email']:'';
            $user->name = isset($input['name'])?$input['name']:'';
            $user->password = isset($input['password'])?bcrypt($input['password']):'';
            $user->role = isset($input['role']) ? $input['role'] : 0;
            if($user->save()){
                $data['result'] = 'ok';
            }
        }
        echo json_encode($data);
	}

	public function edit()
	{
        $data['result'] = 'fail';
        $input = \Request::all();
        if($input['id']==\Auth::user()->id || \Auth::user()->role==999){
            $user = User::find($input['id']);
            if($user!=null){
                $user->email = isset($input['email'])?$input['email']:$user->email;
                $user->name = isset($input['name'])?$input['name']:$user->name;
                $user->password = isset($input['password'])?bcrypt($input['password']):$user->password;
                $user->role = isset($input['role']) ? $input['role'] : $user->role;
                if($user->save()){
                    $data['result'] = 'ok';
                }
            }
        }
        echo json_encode($data);
	}

	public function destroy($id)
	{
        $data['result'] = 'ok';
        if(\Auth::user()->role!=999){
            $data['result'] = 'fail';
        }
        if(\Auth::user()->id!=$id){
            $user = User::destroy($id);
            if($user==0){
                $data['result'] = 'fail';
            }
        }
        echo json_encode($data);
	}

    public function getData(){
        $users = User::all();
        $result=array();
        if(\Auth::user()->role==999){
            foreach ($users as $user) {
                $data=array(
                    'id' => $user->id,
                    'name' => $user->name,
                    'email' => $user->email,
                    'role' => $user->role,
                    'created_at' => date($user->created_at)
                );
                array_push($result, $data);
            }
            echo json_encode($result);
        }
    }

}
