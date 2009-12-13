<?php
if(!isset($EXEC)) die("Access restricted");
if(isset($_GET['p'])){
	$page=$_GET['p'];
	if(!preg_match("/^[_\d]+$/is", $page)){
		unset($page);
	}
}
addNavItem('', 'mobi.astromaximum', 0);
$frm_act="?$lang_&amp;p=selector";
?>
<p>Leave fields empty to enter as guest</p>
<form action="<?php echo $frm_act ?>" method="post">
<p>
login <input name="login" type="text" size="15" class="numinput" inputmode="digits"/><br/>
pass <input name="pass" type="password" size="15" class="numinput" inputmode="digits"/><br/>
<input type="submit" accesskey="1" name="action" value="Proceed"/>
</p>
</form>
