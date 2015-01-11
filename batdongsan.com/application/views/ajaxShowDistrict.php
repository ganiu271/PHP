<select name="district" id="selectbox_district" class="myselectbox" >
	<option value="0">Chọn Quận/Huyện</option>
	<?php foreach($listDistrictByID as $obj):?>
		<option <?php if($districtID == $obj->DISTRICTID) echo "selected"; ?> value="<?php echo $obj->DISTRICTID; ?>"><?php echo $obj->DISTRICTNAME; ?></option>
	<?php endforeach; ?>
</select>