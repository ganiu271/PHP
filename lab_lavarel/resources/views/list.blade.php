@extends('app')

@section('content')
    <button style="float: right;margin-top: 5px" type="button" class="btn btn-primary btn-lg" onclick="showAddUserForm()">Add</button>
    <table id="users" class="table table-hover table-condensed"></table>
    <link href="{{ asset('/js/bootstrap-dialog/css/bootstrap-dialog.css')}}" rel="stylesheet" type="text/css" />
    <script src="{{ asset('/js/jquery.dataTables.min.js') }}"></script>
    <script src="{{ asset('/js/dataTables.bootstrap.js') }}"></script>
    <script src="{{ asset('/js/notify.min.js') }}"></script>
    <script src="{{ asset('/js/bootstrap-dialog/js/bootstrap-dialog.js')}} "></script>
    <script type="text/javascript">

        var token="{{ Session::token() }}";
        $.ajax({
            url : '{{ url('user/table') }}',
            type : 'POST',
            data: {_token: token},
            dataType: 'JSON',
            success : function(data){
                var arr = [];
                for (var prop in data) {
                    var tmp_arr = [];
                    for (var obj_data in data[prop]){
                        tmp_arr.push(data[prop][obj_data]);
                    }
                    arr.push(tmp_arr);
                }
                updateNotification('Load dữ liệu thành công');
                var table = $('#users').DataTable({
                    "data": arr,
                    "columns": [
                        { "title": "ID" },
                        { "title": "User Name" },
                        { "title": "Email" },
                        { "title": "Role", "class": "center",data:null,render : function(data){
                            switch (data[3]){
                                case 0:
                                    data[3]='Normal';
                                    break;
                                case 999:
                                    data[3] = 'Admin';
                                    break;
                            }
                            return data[3];
                        } },
                        { "title": "Create Date", "class": "center" }
                    ]
                });
                $('#users tbody').on('dblclick', 'tr', function () {
                    var row_data = table.row(this).data();
                    showUpdateUserForm(row_data);
                });
                /* Handle click event */
                $('#users tbody').on( 'mousedown', 'tr', function (e) {
                    var row_data = table.row(this).data();
                    if( e.button == 2 ) { // right mouse
                        BootstrapDialog.show({
                            type: BootstrapDialog.TYPE_DANGER,
                            size:BootstrapDialog.SIZE_SMALL,
                            title: 'Warning ! ',
                            message: 'Bạn có muốn xóa : ' + row_data['1'],
                            buttons: [{
                                autospin: true,
                                icon: 'glyphicon glyphicon-send',
                                label: 'Xóa',
                                cssClass: 'btn-success',
                                draggable:true,
                                action: function(dialog) {
                                    $.ajax({
                                        url : '{{ url('user/delete') }}'+'/'+row_data['0'],
                                        type : 'GET',
                                        success : function(data){
                                            if(data.result="ok"){
                                                dialog.close();
                                                updateNotification('Xóa thành công : '+row_data['1']);
                                                location.reload();
                                            }else{
                                                updateNotification('Xóa ['+row_data['1']+'] thất bại','error');
                                            }
                                        }
                                    });
                                }
                            }, {
                                label: 'Đóng',
                                cssClass: 'btn-normal',
                                action: function(dialog) {
                                    dialog.close();
                                }
                            }]
                        });
                    }
                    return true;
                });
            }
        });

        function updateNotification(data,type){
            type = typeof type !== 'undefined' ? type : 'success';
            $.notify( data,{
                position:"top center",
                autoHide:true,
                autoHideDelay:3000,
                className:type
            });
        }
        function showAddUserForm(){
            BootstrapDialog.show({
                size:BootstrapDialog.SIZE_WIDE,
                title: 'Thêm user',
                message: '' +
                '</br> <label>Name:</label> <input id="name_create" type="text" class="form-control" required> ' +
                '</br> <label>Email :</label> <input id="email_create" type="text" class="form-control">'+
                '</br> <label>Password :</label> <input id="password_create" type="text" class="form-control">'+
                '</br> <label>Role :</label> <select id="role_create" class="form-control">' +
                '<option value="999">Admin</option><option value="0"  selected="selected">Normal</option></select>',
                buttons: [{
                    autospin: true,
                    icon: 'glyphicon glyphicon-send',
                    label: 'Save',
                    cssClass: 'btn-success',
                    draggable:true,
                    action: function(dialog) {
                        $.ajax({
                            url : '{{ url('user/create') }}',
                            type : 'POST',
                            data : {
                                _token: token,
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

        function showUpdateUserForm(row_data){
            var data = '<option value="999"selected="selected">Admin</option><option value="0"  >Normal</option></select>';
            if(row_data[3]!="Admin"){
                data = '<option value="999">Admin</option><option value="0"  selected="selected">Normal</option></select>';
            }
            BootstrapDialog.show({
                size:BootstrapDialog.SIZE_WIDE,
                title: 'Update user',
                message: '' +
                '</br> <label>Name:</label> <input id="name_create" value="'+row_data[1]+'" type="text" class="form-control" required> ' +
                '</br> <label>Email :</label> <input id="email_create" value="'+row_data[2]+'" type="text" class="form-control">'+
                '</br> <label>Password :</label> <input id="password_create" type="text" class="form-control">'+
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
                                "id" : row_data[0],
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
    </script>
@endsection
