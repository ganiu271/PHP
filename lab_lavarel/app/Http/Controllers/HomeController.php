<?php namespace App\Http\Controllers;

use App\User;

class HomeController extends Controller {

	public function __construct()
    {
		$this->middleware('auth');
	}

	/**
	 * Show the application dashboard to the user.
	 */
	public function index()
	{
		return view('home');
	}

    public function profile(){
        $profile=\Auth::user();
        switch($profile->role){
            case 0:
                $profile->role = 'Normal';
                break;
            case 999:
                $profile->role = 'Admin';
                break;
        }
        return view('profile')->with([
            'id' => $profile->id,
            'name'=>$profile->name,
            'email' => $profile->email,
            'role' => $profile->role,
            'create_at'=>date($profile->created_at),
            'update_at'=>date($profile->updated_at)
        ]);
    }
}
