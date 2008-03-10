<?php
reject2index("index.php?$lang_");
//print_r($_REQUEST);
if(isset($_GET['u']) && is_numeric($_GET['u'])){
	$id=$_GET['u'];
	$stat="SELECT realname, name, email, dl_count, city_count, active from customers WHERE id=".quote_smart($id);
	$sth=mysql_query($stat);
	if($row=mysql_fetch_row($sth)){
		$active="";
		if($row[5]){
			$active=" checked=\"checked\"";
		}
		echo <<<EOF
<h3>Edit user - $row[0]</h3>
<script type="text/javascript">
<!--
	function checkuser(){
//		alert(findObj('u_pwd1'));
		upw1=findObj('u_pwd1').value;
		upw2=findObj('u_pwd2').value;
		if(upw1!=upw2){
			alert("Passwords do not match!"); return;
		}
		else{
			if(upw1.length>0 && upw1.length!=9){
				alert("Password must be 9 digits long!"); return;
			}
		}
		frm=findObj('usredit');
		frm.submit();
	}
-->
</script>
<form id="usredit" action="index.php?$lang_&amp;p=usermgr" method="post">
<input type="hidden" name="u_id" value="$id"/>
<input type="text" name="u_realname" value="$row[0]"/> realname
<p><input type="text" name="u_email" value="$row[2]"/> e-mail</p>
<p><input type="text" name="u_login" value="$row[1]" maxlength="9"/> login</p>
<p><i>Enter new password when changing login !!!</i></p>
<p><input type="password" name="u_pwd1" value="" size="9"/> pwd
<br/><input type="password" name="u_pwd2" value="" size="9"/> pwd again</p>
<p><input type="text" name="u_dlc" value="$row[3]" size="3"/> dl &nbsp;
&nbsp; <input type="text" name="u_cityc" value="$row[4]" size="3"/> city &nbsp;
&nbsp; <input type="checkbox" name="u_active"$active/> Active</p>
<p><input type="button" name="action" value="Save" onclick="checkuser()"/>
<input type="submit" name="cancel" value="Cancel"/></p>
</form>
EOF;
	}
	mysql_free_result($sth);
	return;
}
echo "<h3>User manager</h3>";
if(!isset($_POST['cancel']) && isset($_POST['u_id']) && is_numeric($_POST['u_id'])){
	$id=$_POST['u_id'];
	$active=isset($_POST['u_active']) ? 1: 0;
	$stat=sprintf("UPDATE customers set realname=%s, name=%s, email=%s, dl_count=%d, city_count=%d, ".
		"active=%d WHERE id=%d",
		quote_smart($_POST['u_realname']),
		quote_smart($_POST['u_login']),
		quote_smart($_POST['u_email']),
		quote_smart($_POST['u_dlc']),
		quote_smart($_POST['u_cityc']),
		quote_smart($active),
		quote_smart($id)
	);
//	echo "$stat<br/>";
	if(mysql_query($stat)){
		$pwd1=$_POST['u_pwd1'];
		$pwd2=$_POST['u_pwd2'];
		if(strcmp($pwd1, $pwd2)==0 && strlen($pwd1)==9){
			$pwd1=pwd_convert2(pwd_convert1($_POST['u_login'], $pwd1));
			$stat=sprintf("UPDATE customers set hash=%s WHERE id=%d",
				quote_smart($pwd1),
				quote_smart($id)
			);
//			echo "$stat<br/>";
			if(!mysql_query($stat)){
				echo "<font color=\"red\">Error when setting password!</font><br/>".mysql_error();
			}
		}
		else{
			echo "Password is not changed";
		}
	}
	else{
		echo "<font color=\"red\">Error when updating user!</font><br/>".mysql_error();
	}
}
?>
<table>
<tr><th>Realname</th><th>email</th><th>dl count</th><th>city count</th></tr>
<?php
$stat="SELECT realname, email, dl_count, city_count, active, id from customers ORDER BY realname";
$sth=mysql_query($stat);
$i=0;
while($row=mysql_fetch_row($sth)){
	$i++;
	$id=array_pop($row);
	$active=array_pop($row);
	$back="";
	if(!$active){
	 	$back=" style=\"background-color: rgb(220,220,220)\""; //inactive
	}
	$row[0]="<a href=\"index.php?$lang_&amp;p=usermgr&amp;u=$id\">$i. ".$row[0]."</a>";
	$row[1]="<a href=\"mailto:$row[1]\">$row[1]</a>";
	echo "<tr><td$back>".implode("</td>\n<td$back>", $row)."</td></tr>";
}
mysql_free_result($sth);
?>
</table>

