<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<link rel="stylesheet" href="<?php echo base_url();?>resource/bootstrap/css/bootstrap-theme.css">
	<link rel="stylesheet" href="<?php echo base_url();?>resource/bootstrap/css/bootstrap.css">
	<script src="<?php echo base_url();?>resource/bootstrap/jquery.min.js"></script>
	<script src="<?php echo base_url();?>resource/bootstrap/js/bootstrap.js"></script>
	<script src="<?php echo base_url();?>resource/myjs/myjavascript.js"></script>
	<link rel="stylesheet" href="<?php echo base_url();?>resource/bootstrap/css/font-awesome.css">

	
</head>
<body>


	<?php
	$input = "SmackFactory";



	function encryptIt( $q ) {
		$cryptKey  = 'qJB0rGtIn5UB1xG03efyCp';
		$qEncoded      = base64_encode( mcrypt_encrypt( MCRYPT_RIJNDAEL_256, md5( $cryptKey ), $q, MCRYPT_MODE_CBC, md5( md5( $cryptKey ) ) ) );
		return( $qEncoded );
	}

	function decryptIt( $q ) {
		$cryptKey  = 'qJB0rGtIn5UB1xG03efyCp';
		$qDecoded      = rtrim( mcrypt_decrypt( MCRYPT_RIJNDAEL_256, md5( $cryptKey ), base64_decode( $q ), MCRYPT_MODE_CBC, md5( md5( $cryptKey ) ) ), "\0");
		return( $qDecoded );
	}

	$encrypted = encryptIt( $input );
	$decrypted = decryptIt( $encrypted );

	echo $encrypted . '<br />' . $decrypted;
	?>
</body>
</html>
