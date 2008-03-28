<?php
reject2index("index.php?$lang_");
//print_r($_REQUEST);
if(isset($_GET['u'])){
	$row=array();
	$id=$_GET['u'];
	if(strcmp($id, 'add')==0){
		$row=array("", "", "", 2, 5, 1);
		$hdr="Add new user:";
		$act="Add";
	}
	if(is_numeric($id)){
		$stat="SELECT realname, name, email, dl_count, city_count, active from customers WHERE id=".quote_smart($id);
		$sth=mysql_query($stat);
		$row=mysql_fetch_row($sth);
		$hdr="Edit user - ";
		$act="Save";
	}
	if(count($row)){
		$active="";
		if($row[5]){
			$active=" checked=\"checked\"";
		}
		echo <<<EOF
<h3>$hdr $row[0]</h3>
<script type="text/javascript">
<!--
	function is_empty(id){
		return findObj(id).value.length==0;
	}

	function check_notify(){
		if(!findObj('u_notify').checked) return true;
		return !is_empty('u_pwd1') && !is_empty('u_email') && !is_empty('u_login');
	}

	function check_user(){
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
		if(check_notify()){
			findObj('usredit').submit();
		}
		else{
			alert("Notify user: Missing login or password or email");
		}

	}
	
	function do_random(input_id){
		var str='';
		for(i=0; i<9; i++){
			str=str.concat(Math.floor(Math.random()*9));
		}
		findObj(input_id).value=str;
	}
-->
</script>
<form id="usredit" action="index.php?$lang_&amp;p=usermgr" method="post">
<input type="hidden" name="u_id" value="$id"/>
<input type="text" name="u_realname" value="$row[0]"/> realname &nbsp; 
<p>
<input type="text" name="u_login" value="$row[1]" maxlength="9"/>
<a href="javascript:void(0)" onclick="do_random('u_login');">login</a> &nbsp;
</p> 
<p><i>Enter new password when changing login !!!</i></p>
<p>
<input type="text" name="u_pwd1" value="" size="9" maxlength="9"/> 
<a href="javascript:void(0)" onclick="do_random('u_pwd1');">pwd</a> &nbsp;
<br/><input type="text" name="u_pwd2" size="9" maxlength="9"> pwd again &nbsp;
<input type="text" name="u_email" size="48" value="$row[2]"/> e-mail</p>
<p>
<input type="text" name="u_dlc" value="$row[3]" size="3"/> dl &nbsp; &nbsp; 
<input type="text" name="u_cityc" value="$row[4]" size="3"/> city &nbsp; &nbsp; 
<input type="checkbox" name="u_active"$active/> Active &nbsp; &nbsp; 
<input type="checkbox" name="u_notify"/> E-mail credentials to this user</a></p>
<p>
<input type="button" name="action" value="$act" onclick="check_user()"/>
<input type="submit" name="cancel" value="Cancel"/></p>
</form>
EOF;
	}
	return;
}
echo "<h3>User manager</h3>";
if(isset($_POST['u_id'])){
	if(!isset($_POST['cancel'])){ 
		$id=$_POST['u_id'];
		$is_num=is_numeric($id);
		$active=isset($_POST['u_active']) ? 1: 0;
		$succ=false;
		if($is_num){
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
			$succ=mysql_query($stat);
		}
		if(strcmp($id, "add")==0){
			$stat=sprintf("INSERT INTO customers(realname, name, email, dl_count, city_count, active, subscr_date) ".
				"VALUES (%s, %s, %s, %d, %d, %d, CURRENT_DATE)",
				quote_smart($_POST['u_realname']),
				quote_smart($_POST['u_login']),
				quote_smart($_POST['u_email']),
				quote_smart($_POST['u_dlc']),
				quote_smart($_POST['u_cityc']),
				quote_smart($active)
			);
			$succ=mysql_query($stat);
			if($succ){
				$id=mysql_insert_id();
			}
		}
		if($succ){
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
				else{
					if(isset($_POST['u_notify'])){
						include_once("mobi/amtools.php");
						if(pwd_send($_POST['u_email'], $_POST['u_login'], $_POST['u_realname'],
						 	$_POST['u_dlc'], $_POST['u_cityc'], $_POST['u_pwd1'])){
						 		echo "Notification was sent to ".$_POST['u_email']."<br/>";
						}
						else{
							echo "<font color=\"red\">Error when sending notification to ".$_POST['u_email'].
								"</font><br/>";
						}
					}
				}
			}
			else{
				if($is_num){
					echo "Password is not changed";
				}
				else{
					echo "<font color=\"red\">Password is <b>UNDEFINED</b>!!! No one knows it!</font>";
				}
			}
		}
		else{
			echo "<font color=\"red\">Error when updating user!</font><br/>".mysql_error();
		}
	}
}
?>
<table>
<tr><td colspan="4" style="background-color:white; text-align:right">
<a href="index.php?<?php echo $lang_ ?>&amp;p=usermgr&amp;u=add">Add user</a></td></tr>
<tr><th>Realname</th><th>email</th><th>dl count</th><th>city count</th></tr>
<?php
$stat="SELECT realname, email, dl_count, city_count, hash, active, id from customers ORDER BY realname";
$sth=mysql_query($stat);
$i=0;
while($row=mysql_fetch_row($sth)){
	$i++;
	$id=array_pop($row);
	$active=array_pop($row);
	$hash=array_pop($row);
	$back="";
	if(!$active){
	 	$back=" style=\"background-color: rgb(220,220,220)\""; //inactive
	}
	if(!$hash){
	 	$back=" style=\"background-color: rgb(236,113,113)\""; //password invalid
	}
	$row[0]="<a href=\"index.php?$lang_&amp;p=usermgr&amp;u=$id\">$i. ".$row[0]."</a>";
	$row[1]="<a href=\"mailto:$row[1]\">$row[1]</a>";
	echo "<tr><td$back>".implode("</td>\n<td>", $row)."</td></tr>";
}
mysql_free_result($sth);
?>
</table>

