<?php
$q = $_REQUEST["q"];

// lookup all hints from array if $q is different from "" 
switch ($q) {
	case '1':
	{
		$arr = array("Nhà trọ cho thuê","Tìm người ở ghép");
		break;
	}
	case '2':
	{
		$arr = array("Căn hộ cho thuê","Căn hộ cần bán");
		break;
	}
	case '3':
	{
		$arr = array("Văn phòng cho thuê","Mặt bằng cho thuê");
		break;
	}
	case '4':
	{
		$arr = array("Nhà đất cho thuê","Nhà đất cần bán");
		break;
	}
	
	default:
	break;
}
?>


<?php foreach ($arr as $i): ?>
	<input name="type" id="type" type="radio" value="<?php echo $i ?>"><?php echo $i ?> &nbsp
<?php endforeach; ?>
