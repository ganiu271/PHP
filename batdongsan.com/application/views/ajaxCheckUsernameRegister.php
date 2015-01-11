<?php

$q = $_REQUEST["q"];

if(isset($listUsername))
{
	$count = 0;
	foreach ($listUsername as $obj) {
		if($obj->username == $q)
			$count++;
	}

	if($count != 0)
	{
		echo "Tên đăng nhập đã có người sử dụng. Vui lòng nhập tên khác!";
		
	}
	else
	{
		if(strlen($q) < 6 )
		{
			echo "Tên đăng nhập phải dài hơn 5 ký tự.";
		}
		else
		{
			echo "<span style='color: #2d81d7'>Tên đăng nhập hợp lệ.</span>";
		}
	}

}

?>
