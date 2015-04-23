@extends('app')

@section('content')
        <div class="container">
            <div class="row">
                <div class="col-md-10 col-md-offset-1">
                    <div class="panel panel-primary">
                        <div class="panel-heading">User profile</div>

                        <div class="panel-body">

                            {{$id}}
                            {{ $name }}

                            {{ $email }}
                        </div>
                    </div>
                </div>
            </div>
        </div>
@endsection
