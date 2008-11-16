<?php
    $EXEC=1;

// ?mode=clean_files  - delete outdated files and set 'files.deleted' to true
    if(file_exists('pwdgen_local.php') ||
       (isset($_GET['mode']) && strcmp($GET['mode'], 'clean_files'))){
        include_once('mobi/config.php');
        include_once('mobi/dbconnect.php');
        $stat='SELECT id FROM files WHERE deleted=\'f\' AND end_tm<NOW()';
        $sth=mysql_query($stat);
        $ids=array();
        while($row=mysql_fetch_row($sth)){
            array_push($ids, $row[0]);
            $fn='mobi/dl/files/'.$row[0];
            @unlink($fn.'.d');
            @unlink($fn.'.r');
            @unlink($fn.'.t');
            print "$row[0], ";
        }
        if(!empty($ids)){
            $stat='UPDATE files SET deleted=\'t\' WHERE id IN (\''.implode('\',\'', $ids).'\')';
            mysql_query($stat);
        }
        
// delete all old /tmp/sunrise_*
        foreach (glob('/tmp/sunrise_*') as $filename) {
            @unlink($filename);
        }
// delete unconfirmed users that live more than 24 hr      
        $stat="DELETE FROM customers WHERE name='' AND active=0 AND subscr_date<SUBTIME(NOW(),'24:00:00')";
        $sth=mysql_query($stat);
    }
?>
