<?php
    $matches = array();
    if (preg_match('/^(\d+);([0-9a-f]+)$/', $HTTP_RAW_POST_DATA, $matches) > 0) {
        $param = $matches[1];
        $keycode = $matches[2];
        if (file_exists('/tmp/microemu_'.$keycode)) {
            $dir = '../mobi/dl/source';
            switch ($param) {
                case 0:
                    $ifile = '2008.comm';
                    break;
                case 1:
                    $ifile = 'locations2008.dat';
                    break;
            }
            $ifile = "$dir/$ifile";
            header ('Content-type: application/octet-stream');
            if (file_exists ($ifile))
                readfile($ifile);
        }
    }
    else {
        $ifile = "./Astromaximum-microemu.jar";
        header ('Content-Type: application/java-archive');
        if (file_exists ($ifile))
            readfile($ifile);
    }
?>
