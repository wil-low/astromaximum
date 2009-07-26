<?php
    $matches = array();
    if (preg_match('/^(\d+);/', $HTTP_RAW_POST_DATA, $matches) > 0) {
        $param = $matches[1];
        switch ($param) {
            case 0:
                $ifile = '2008.comm';
                break;
            case 1:
                $ifile = 'locations2008.dat';
                break;
        }
        $ifile = "../mobi/dl/source/$ifile";
    	$clen=filesize($ifile);
        header ('Content-type: application/octet-stream');
    	header("Content-Length: $clen");
        if (file_exists ($ifile))
            readfile($ifile);
    }
    else {
        $ifile = "./Astromaximum-microemu.jar";
    	$clen=filesize($ifile);
        header ('Content-Type: application/java-archive');
        if (file_exists ($ifile))
            readfile($ifile);
    }
?>
