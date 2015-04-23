@extends('app')

@section('content')
        <div class="container">
            <div class="panel panel-primary">
                <div class="panel-heading">Profile</div>
                <div class="panel-body">
                    <div class="list-group">
                        <a href="#" class="list-group-item">
                            <i class="fa fa-comment fa-fw"></i> UserName : {{ $name }}
                        </a>
                        <a href="#" class="list-group-item">
                            <i class="fa fa-twitter fa-fw"></i>Email : {{ $email }}
                        </a>
                        <a href="#" class="list-group-item">
                            <i class="fa fa-envelope fa-fw"></i>Role : {{$role}}
                        </a>
                        <a href="#" class="list-group-item">
                            <i class="fa fa-tasks fa-fw"></i>Create date : {{ $create_at }}
                        </a>
                        <a href="#" class="list-group-item">
                            <i class="fa fa-upload fa-fw"></i>Last update : {{ $update_at }}
                        </a>

                    </div>
                    <!-- /.list-group -->
                    <a href="#" class="btn btn-default btn-block" onclick="show()">Update Profile</a>
                </div>
                <!-- /.panel-body -->
            </div>
        </div>
        <script src="{{ asset('/assets/jquery/jquery-1.10.2.min.js')}}"></script>
        <link href="{{ asset('/assets/bootstrap/css/bootstrap.min.css')}}" rel="stylesheet" type="text/css" />
        <script src="{{ asset('/assets/bootstrap/js/bootstrap.min.js')}}"></script>
        <script src="{{ asset('/assets/prettify/run_prettify.js')}}"></script>
        <link href="{{ asset('/assets/bootstrap-dialog/css/bootstrap-dialog.min.css')}}" rel="stylesheet" type="text/css" />
        <script src="{{ asset('/assets/bootstrap-dialog/js/bootstrap-dialog.min.js')}}"></script>
        <script type="text/javascript">
            function show(){
                BootstrapDialog.show({
                    title: 'Button Hotkey',
                    message: $('<textarea class="form-control" placeholder="Try to input multiple lines here..."></textarea>'),
                    buttons: [{
                        label: '(Enter) Button A',
                        cssClass: 'btn-primary',
                        hotkey: 13, // Enter.
                        action: function() {
                            alert('You pressed Enter.');
                        }
                    }]
                });
            }
        </script>
@endsection
