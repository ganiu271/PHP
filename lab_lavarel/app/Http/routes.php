<?php

/*
|--------------------------------------------------------------------------
| Application Routes
|--------------------------------------------------------------------------
|
| Here is where you can register all of the routes for an application.
| It's a breeze. Simply tell Laravel the URIs it should respond to
| and give it the controller to call when that URI is requested.
|
*/

Route::get('/', 'HomeController@index');
Route::get('home', 'HomeController@index');

Route::get('user/profile','HomeController@profile');
Route::get('user/list','UserController@index');

Route::post('user/create','UserController@create');
Route::post('user/edit','UserController@edit');
Route::get('user/delete/{id}','UserController@destroy');
Route::post('user/table','UserController@getData');

Route::controllers([
	'auth' => 'Auth\AuthController',
	'password' => 'Auth\PasswordController',
]);
