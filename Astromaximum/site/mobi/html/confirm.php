<?php 
if(!isset($EXEC)) die("Access restricted");
if(isset($_GET['key'])){
// empty name and active=0    
    $stat=sprintf("SELECT customers.id,email,realname,dic_paymode.name,paymode_id FROM customers,dic_paymode WHERE hash=%s AND customers.name='' AND active=0 AND dic_paymode.id=paymode_id LIMIT 1",
        quote_smart($_GET['key']));
    $sth=mysql_query($stat);
    if(mysql_num_rows($sth)==1){
        $row=mysql_fetch_row($sth);
        do{ // names must be unique
            $usr=random9(); 
            $stat=sprintf('SELECT id FROM customers WHERE name=%s LIMIT 1', $usr);
            $sth=mysql_query($stat);
        }while(mysql_num_rows($sth));    
        $pwd=random9();
        $hash=pwd_convert2(pwd_convert1($usr, $pwd));
        $stat=sprintf("UPDATE customers SET name='%s', hash='%s', active=1 WHERE id=%d LIMIT 1",
            $usr, $hash, $row[0]);
//        $sth=mysql_query($stat);
        if(true){
            echo sprintf($i18['CONFIRM_THANKS'], $row[2], $row[1]);
            $tries=get_try_count($row[0]);
//            $mail=pwd_send($row[1], $usr, $row[2], $tries, $pwd);
    
            $msg="Email: $row[1]\nNick: $row[2]\nLogin: $usr\nPaymode: $row[3]\nIP: {$_SERVER['REMOTE_ADDR']}";
            event_send('new customer', $msg);
            show_payment_instructions($row[4]);
        }
        else{
            event_send('error confirm.php', mysql_error()."\n$stat");
            echo 'Error';
        }
        return;
    }
}
?>
<h4>Wrong request</h4>