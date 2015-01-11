<?php if(is_array($districts)): ?>
	<?php foreach($districts as $district): ?>
		<option <?php
					if(isset($districtselected))
				 		if($districtselected == $district->DISTRICTID)
				 			echo "selected";
		 		?>
		 	value="<?php echo $district->DISTRICTID; ?>"><?php echo $district->DISTRICTNAME; ?></option>
	<?php endforeach; ?>
<?php endif; ?>