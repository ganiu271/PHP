@extends('app')

@section('content')
<div class="container">
	<div class="row">
		<div class="col-md-10 col-md-offset-1">
			<div class="panel panel-default">
				<div class="panel-heading">Home</div>

				<div class="panel-body">
					You are logged in!
                    <div class="quote">{{ Inspiring::quote() }}</div>
				</div>
			</div>
		</div>
	</div>
{{--    {{ HTML::script('assets/js/jquery.dataTables.min.js') }}--}}
</div>
@endsection