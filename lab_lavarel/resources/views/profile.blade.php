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
        <link href="{{ asset('/js/bootstrap-dialog/css/bootstrap-dialog.css')}}" rel="stylesheet" type="text/css" />
        <script src="{{ asset('/js/jquery.dataTables.min.js') }}"></script>
        <script src="{{ asset('/js/dataTables.bootstrap.js') }}"></script>
        <script src="{{ asset('/js/notify.min.js') }}"></script>
        <script src="{{ asset('/js/bootstrap-dialog/js/bootstrap-dialog.js')}} "></script>
        <script type="text/javascript">
            var token="{{ Session::token() }}";
            function show(){
                var data = '<option value="999"selected="selected">Admin</option><option value="0"  >Normal</option></select>';
                if('{{$role}}'!="Admin"){
                    data = '<option value="999">Admin</option><option value="0"  selected="selected">Normal</option></select>';
                }
                BootstrapDialog.show({
                    size:BootstrapDialog.SIZE_WIDE,
                    title: 'Update user',
                    message: '' +
                    '</br> <label>Name:</label> <input id="name_create" value="{{ $name }}" type="text" class="form-control" required> ' +
                    '</br> <label>Email :</label> <input id="email_create" value="{{ $email}}" type="text" class="form-control">'+
                    '</br> <label>Password :</label> <input id="password_create" type="password" class="form-control">'+
                    '</br> <label>Role :</label> <select id="role_create" class="form-control">' +data,
                    buttons: [{
                        autospin: true,
                        icon: 'glyphicon glyphicon-send',
                        label: 'Save',
                        cssClass: 'btn-success',
                        draggable:true,
                        action: function(dialog) {
                            $.ajax({
                                url : '{{ url('user/edit') }}',
                                type : 'POST',
                                data : {
                                    _token: token,
                                    "id" : '{{$id}}',
                                    "name" : dialog.getModalContent().find("#name_create").val(),
                                    "email" : dialog.getModalContent().find("#email_create").val(),
                                    "role" : dialog.getModalContent().find("#role_create").val(),
                                    "password" : dialog.getModalContent().find("#password_create").val()
                                },
                                success : function(data){
                                    data = JSON.parse(data);
                                    if(data.result=="ok"){
                                        dialog.close();
                                        updateNotification('Thêm thành công :');
                                        location.reload();
                                    }else{
                                        updateNotification('Thêm thất bại','error');
                                    }
                                }
                            });
                        }
                    },{
                        label: 'Đóng',
                        cssClass: 'btn-warning',
                        action: function(dialog) {
                            dialog.close();
                        }
                    }]
                });
            }
            function updateNotification(data,type){
                type = typeof type !== 'undefined' ? type : 'success';
                $.notify( data,{
                    position:"top center",
                    autoHide:true,
                    autoHideDelay:3000,
                    className:type
                });
            }
        </script>
@endsection
