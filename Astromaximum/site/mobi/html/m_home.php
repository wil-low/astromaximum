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

if($chac>=0)
    redirect($frm_act);
?>
<form action="<?php echo $frm_act ?>" method="post">
<p><?php if($chac==-1){ ?>
login <input name="login" type="text" size="15"/><br/>
pass <input name="pass" type="password" size="15"/><br/>
<?php } ?>
<input type="submit" accesskey="1" name="action" value="Proceed"/></p>
</form>
