<?php
if (isset ($_GET['h'])) { // city hash
	$hash = $_GET['h'];
	$today = gmdate ('ymd');
	$ifile = "i/daily/informer/$hash/$today.png";
	header ("Content-type: image/png");
	if (file_exists ($ifile))
		readfile($ifile);
}
