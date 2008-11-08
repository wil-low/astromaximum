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
</p>
<form action="<?php echo $frm_act ?>" method="post">
<?php if($chac==-1){ ?>
login <input name="login" type="text" size="9" maxlength="9"/><br/>
pass <input name="pass" type="password" size="9" maxlength="9" class="numinput"/><br/>
<?php } ?>
<input type="submit" accesskey="1" name="action" value="Proceed"/></p>
</form>
