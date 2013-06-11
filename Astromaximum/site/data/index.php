<?php 
    $common_key = '';
    if (isset ($_GET['buy'])) {
        $common_key = $_GET['buy'];
        if (isset ($_POST['p']) && (strcmp ($_POST['p'], '999ec9eafd3eb789e70c2a2584d21391') == 0)) {
            include_once('year_keys.php.inc');
            $common_key = $GLOBALS['amax_droid_keys'][$common_key];
        }
    }
    if (isset ($_POST['k']) && (strcmp ($_POST['k'], '44b62ab3e3165298849ac71428eca191')) == 0) {
        $fn = $common_key.'/common';
        $handle = fopen($fn, "rb");
        if(!$handle) {
            no_data();
            exit;
        }
        $clen=filesize($fn);
        $data = fread($handle, $clen);
        fclose($handle);
        header('Content-Type: application/octet-stream');
        header("Content-Length: $clen");
        $content='Content-Disposition: attachment; filename="common"';
        header($content);
        header('Content-Transfer-Encoding: binary');
        ob_clean();
        flush();
        readfile($fn);
    }

    function no_data() {
        header("HTTP/1.0 403 Forbidden");
        exit;
    }
?>
