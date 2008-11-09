<?php 
if(!isset($EXEC)) die("Access restricted");
if(isset($_GET['key'])){
// empty name and active=0    
    $stat=sprintf("SELECT id,email,realname FROM customers WHERE hash=%s AND name='' AND active=0 LIMIT 1",
        quote_smart($_GET['key']));
    $sth=mysql_query($stat);
    if(mysql_num_rows($sth)==1){
        $row=mysql_fetch_row($sth);
        $stat=sprintf("UPDATE customers SET hash='', active=1 WHERE id=%d LIMIT 1", $row[0]);
        $sth=mysql_query($stat);
        echo "Thanks $row[1], $row[2]";
        return;
    }
}
?>
<h4>Wrong request</h4>