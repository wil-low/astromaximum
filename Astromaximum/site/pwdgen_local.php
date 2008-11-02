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
	echo "<p>Email: $user<br/>Hash: $pwd2</p>";
        echo "<a href=\"$script\">back</a>";
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