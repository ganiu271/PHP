@extends('app')

@section('content')
    <table id="users" class="table table-hover table-condensed">
        <thead>
            <tr>
                <th class="col-md-3">{{{ Lang::get('users/table.username') }}}</th>
                <th class="col-md-3">{{{ Lang::get('users/table.email') }}}</th>
                <th class="col-md-3">{{{ Lang::get('users/table.created_at') }}}</th>
                <th class="col-md-3">{{{ Lang::get('table.actions') }}}</th>
            </tr>
        </thead>
    </table>
    <script src="{{ asset('/js/jquery.min.js') }}"></script>
    <script src="{{ asset('/js/jquery.dataTables.min.js') }}"></script>
    <script src="{{ asset('/js/dataTables.bootstrap.js') }}"></script>
    <script type="text/javascript">
        var token="{{ Session::token() }}";
        $(document).ready(function() {
            var oTable = $('#users').DataTable({
                "processing": true,
                "serverSide": true,
                "ajax": "user/table",
                "columns": [
                    {tittle:"User Name",data: 'username', name: 'username'},
                    {data: 'email', name: 'email'},
                    {data: 'created_at', name: 'created_at'},
                    {data: 'actions', name: 'actions'}
                ]
            });
        });


    </script>
@endsection
