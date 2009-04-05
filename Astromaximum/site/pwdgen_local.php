<?php
    $EXEC=1;
    include_once('mobi/dbconnect.php');
    $script=$_SERVER['SCRIPT_NAME'];
    $user=''; $pwd='';
    if(isset($_POST['email']) && isset($_POST['pwd'])){
        $user=$_POST['email'];
        $pwd=$_POST['pwd'];
		$pwd1=pwd_convert1($user, $pwd);
		$pwd2=pwd_convert2($pwd1);
	
		$sql_pwd = sprintf("SELECT AMAX_HASH(%s, %s)",  
			quote_smart($user), quote_smart($pwd));
		$sth = mysql_query($sql_pwd);
		$pwd3 = '';
		echo "<p>Email: $user<br/>PHP hash: $pwd2</p>";
		if($sth) {
			$arr = mysql_fetch_array($sth);
			$pwd3 = $arr[0];
			echo "Mysql hash: ".$pwd3;
		}
		else
			echo mysql_error();
		if (strcmp($pwd2, $pwd3))
			echo "<p>Hashes do not match!</p>";
        echo "<p><a href=\"$script\">back</a></p>";
        return;
    }
?>
<form action="<?php echo $script ?>" method="post">
<table>
    <tr><td>Email:</td><td><input type="text" name="email"/></td></tr>
    <tr><td>Pwd:</td><td><input type="text" name="pwd"/></td></tr>
</table>
<input type="submit"/>
</form>