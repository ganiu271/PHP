$("#pw1, #pw2").on('keyup', function(e){
	var pw1 = $("#pw1").val();
	var pw2 = $("#pw2").val();
	if(pw1 == pw2)
	{
		$("#pw1, #pw2").css("background-color","green");
		document.getElementById('pwstt').innerHTML = 'Password is match';
		$("#regacc").attr('disabled',false);
	}
	else
	{
		$("#pw1, #pw2").css("background-color","red");
		document.getElementById('pwstt').innerHTML = 'Password is not match';
		$("#regacc").attr('disabled',true);
	}
});

$("#username").on('keyup', function(e){

	var username = $("#username").val();
	$.ajax({
		url:"http://localhost/batdongsan.com/admin/checkusername/"+username,
		success: function(data) {
			if(data == "dung")
			{
				$("#username").css('background-color','red');
				document.getElementById('userstt').innerHTML = 'Username is Unavailable';
				$("#regacc").attr('disabled',true);
			}
			else
			{
				$("#username").css('background-color','green');
				document.getElementById('userstt').innerHTML = 'Username is Available';
				$("#regacc").attr('disabled',false);
			}
    	} 
	});
});

$("#provinceaddnews").on('change', function(){
	var province = $(this).val();
	$.ajax({
		url:"http://localhost/batdongsan.com/admin/loaddistrict/"+province,
		success: function(data) {
			$('#districtaddnews').html(data);
    	} 
	});
});

$("#kind").on('change', function(){
	var kind = $(this).val();

	switch (kind)
	{
		case "Nhà trọ":
			$('#type').html("<option value'Nhà trọ cho thuê'>Nhà trọ cho thuê</option><option value'Tìm người ở ghép'>Tìm người ở ghép</option>");
			break;

		case "Căn hộ":
			$('#type').html("<option value'Căn hộ cho thuê'>Căn hộ cho thuê</option><option value'Căn hộ cần bán'>Căn hộ cần bán</option>");
			break;

		case "Văn phòng/Mặt bằng":
			$('#type').html("<option value'Văn phòng cho thuê'>Văn phòng cho thuê</option><option value'Mặt bằng cho thuê'>Mặt bằng cho thuê</option>");
			break;

		case "Nhà đất":
			$('#type').html("<option value'Nhà cho thuê'>Nhà cho thuê</option><option value'Nhà đất cần bán'>Nhà đất cần bán</option>");
			break;

	}
	
});