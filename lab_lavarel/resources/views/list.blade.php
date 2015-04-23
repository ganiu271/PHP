@extends('app')

@section('content')
    <table id="users" class="table table-hover table-condensed"></table>
    <script src="{{ asset('/js/jquery.min.js') }}"></script>
    <script src="{{ asset('/js/jquery.dataTables.min.js') }}"></script>
    <script src="{{ asset('/js/dataTables.bootstrap.js') }}"></script>
    <script type="text/javascript">
        var token="{{ Session::token() }}";
        $.post( "user/table", function( data ) {
            console.log(data);
        });
        var dataSet = [
            ['Trident','Internet Explorer 4.0','Win 95+','4','X'],
            ['Trident','Internet Explorer 5.0','Win 95+','5','C'],
            ['Trident','Internet Explorer 5.5','Win 95+','5.5','A'],
            ['Trident','Internet Explorer 6','Win 98+','6','A'],
            ['Trident','Internet Explorer 7','Win XP SP2+','7','A'],
            ['Trident','AOL browser (AOL desktop)','Win XP','6','A']
        ];
        console.log(dataSet);
        $(document).ready(function() {
            var table = $('#users').dataTable({
                "data": dataSet,
                "columns": [
                    { "title": "Engine" },
                    { "title": "Browser" },
                    { "title": "Platform" },
                    { "title": "Version", "class": "center" },
                    { "title": "Grade", "class": "center" }
                ]
            });
        });


    </script>
@endsection
