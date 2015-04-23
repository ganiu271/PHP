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
        if(\Auth::user()->role==999){

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

    public function getData()
    {
        $users = User::all();
        return Datatables::of($users)
            ->setRowId('id') // via column name if exists else just return the value
            ->setRowId(function($user) {
                return $user->id;
            }) // via closure
            ->setRowId('{{ $id }}') // via blade parsing

            ->setRowClass('id') // via column name if exists else just return the value
            ->setRowClass(function($user) {
                return $user->id;
            }) // via closure
            ->setRowClass('{{ $id }}') // via blade parsing

            ->setRowData([
                'string' => 'data',
                'closure' => function($user) {
                    return $user->name;
                },
                'blade' => '{{ $name }}'
            ])
            ->addRowData('a_string', 'value')
            ->addRowData('a_closure', function($user) {
                return $user->name;
            })
            ->addRowData('a_blade', '{{ $name }}')
            ->setRowAttr([
                'color' => 'data',
                'closure' => function($user) {
                    return $user->name;
                },
                'blade' => '{{ $name }}'
            ])
            ->addRowAttr('a_string', 'value')
            ->addRowAttr('a_closure', function($user) {
                return $user->name;
            })
            ->addRowAttr('a_blade', '{{ $name }}')
            ->make(true);
    }

}
