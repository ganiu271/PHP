<?php namespace App\Http\Controllers;

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
        return view('profile')->with([
            'id' => $profile->id,
            'name'=>$profile->name,
            'email' => $profile->email,
        ]);
    }
}
