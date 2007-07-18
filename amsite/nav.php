<?php
include('dbconnect.php');

function registered_user(){
	if($sid=session_id()){
		echo '<h3>'.$sid.'</h3>';
	}
	else{
		session_start();
	}
	$result=mysql_query('SELECT * FROM customers')
		or die(mysql_error());
	echo '<table>';
	while($row=mysql_fetch_array($result)){
		echo '<tr><td>'.implode('</td><td>',$row).'</td></tr>';
	}
	echo '</table>';
	return 'aivushkin';
}
?>

<table border="1" width="100%" height=100%>
	<tr height=35%>
		<td width="22%" align=center valign=top>
			<img src="/img/logo.png" border="0" alt="Astromaximum logo">
			<table style="font-size:smaller;" width=100%><tr align=center>
			<td><a href="index.php?lang=en">English</a></td>
			<td><a href="index.php?lang=ru">Русский</a></td>
			</tr></table>
			<br><br>
			<a href=index.php?<?php echo "$lang_>".$i18['MAIN']?></a><br><br>
			<a href=feat.php?<?php echo "$lang_>".$i18['FEAT']?></a><br><br>
			<a href=scr.php?<?php echo "$lang_>".$i18['SCR']?></a><br><br>
			<a href=req.php?<?php echo "$lang_>".$i18['REQ']?></a><br><br>
			<a href=contact.php?<?php echo "$lang>".$i18['CONTACT']?></a><br><br>
			<a href=links.php?<?php echo "$lang_>".$i18['LINKS']?></a><br><br>
		</td>
		<td rowspan=3 valign=top>
			<?php echo $content ?>
		</td>
	</tr>
	<tr>
	<td height=20%>
	<center>
		
<?php
if($username=registered_user())
{
?>
	<font size=-1>
	<h4><?php echo $i18['MEM_LOGIN']?></h4>
	<span class=login>
	<form method='post' action=<?php echo $_SERVER['SCRIPT_NAME']."?$lang_"?> >
		<?php echo $i18['USERNAME']?> <input type="text" name="user"></input>
		<br><?php echo $i18['PWD']?> <input type="password" name="passwd"></input>
		<br><input type=submit value='<?php echo $i18['LOG_IN']?>'></input>
	</form></span>
	<center>
	<?php 
		if(isset($_GET['LOGIN_MSG'])){
			echo $i18['LOGIN_MSG'];
		}
	?>
	&nbsp;</center></font>
<?php
}
?>

	</center>
	</td></tr>
	<tr align=center>
		<td>
			<p><a href=test.php?<?php echo "$lang_>".$i18['TEST']?></a></p>
			<p><a href=demo.php?<?php echo "$lang_>".$i18['DEMO']?></a></p>
			<p><table cellpadding="0" cellspacing="0">
				<tr align=center><td><a href=order.php?<?php echo "$lang_>".$i18['ORDER']?></a></td>
				<td><span align="center">&nbsp;
				<img src="/img/paypal.png" alt="PayPal"></span></td></tr>
			</table></p>
			<p><a href=geo.php?<?php echo "$lang_>".$i18['DB']?></a></p>
		</td>
	</tr>
</table>



