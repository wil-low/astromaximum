<?php
if(!isset($EXEC)) die("Access restricted");
if(isset($_GET['p'])){
	$page=$_GET['p'];
	if(!preg_match("/^[_\d]+$/is", $page)){
		unset($page);
	}
}
if(isset($page) && $chac!=-1 && $chac!=1){
	$dir=dirname($_SERVER['SCRIPT_FILENAME']);
	$fn="$dir/html/$lang/$page.php";
	if(file_exists($fn)){
		$content=file_get_contents($fn);
		echo str_replace("src=\"mobi/", "src=\"", $content);
		if(strcmp($page, '0_0')){
			echo "<p><br/><a href=\"?$lang_&amp;p=0_".$page{0}."\"><strong>назад к теме</strong></a></p>";
		}
		exit;
	}
	else{
		redirect("selector.php?lang=$lang");
	}
}
$frm_act="?$lang_&amp;p=selector";
?>
<form action="<?php echo $frm_act ?>" method="post">
<p>
<input type="radio" name="lang" value="en"/>en
<input type="radio" name="lang" value="ru" checked="checked"/>ru
<br/>
<input type="radio" name="dest" value="ph" checked="checked"/>Mobile
<input type="radio" name="dest" value="pc"/>PC</p>
<?php if($chac==-1){ ?>
<p>
login <input name="login" type="text" size="9" maxlength="9"/><br/>
pass <input name="pass" type="password" size="9" maxlength="9" class="numinput"/><br/>
</p>
<?php } ?>
<input type="submit" accesskey="1" name="action" value="Proceed"/>
</form>
