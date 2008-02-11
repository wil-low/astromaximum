<?php
lang_load("mobi/html");
$step=1;
$max_cities=20;

$defyear=date("Y");
if(isset($_POST['y_sel'])){
	$defyear=$_POST['y_sel'];
}
$sc=',';
if(isset($_POST['sc'])){
	$sc=$_POST['sc'];
}
$act=0;
if(isset($_POST['Action'])){
	$act=$_POST['Action'];
}
if(strlen($act) && isset($_POST['sc'])){
	$sth=get_selected_cities('sc');
	if(strlen($sth)>0){
		echo "<h4>{$i18['READY_CITIES']} ($defyear):</h4>\n<ol>";
		while($row = mysql_fetch_row($sth)){
			echo "<li>$row[1], $row[2]</li>\n";
		}
		echo "</ol>\n";
		include_once('mobi/amtools.php');
		$perl=find_perl();
		global $DIR_FILES, $DIR_SOURCE;
		$dsrc="mobi/$DIR_FILES";
		$ye=substr($defyear,-2);
		list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
		$srcdir="$dsrc/$fn";
	#	echo "$dsrc/$destfile";
		$cmd="$perl mobi/dl/gen_amax.cgi geo- $defyear $lang $sc $dsrc/$fn.r nomessjar";
		$ret=0;
		exec($cmd, $outp, $ret);
		if($ret){				
			echo $cmd;
			echo implode('<br>',$outp);
		}
		else{
			$id=$fn;
			if(!add_file($fn, "g $defyear")){
				echo mysql_error();
				exit;
			}

			$url='mobi/data.php?r='.$id;
			echo "<h4>{$i18['PC_DL']}:</h4>";
			echo "<a href='$url'>JAR</a>";
			$url=str_replace("?r", "?d", $url);
			echo " <a href='$url'>JAD</a><br>";
/*
			$url=str_replace("?d", "?t", $url);
			echo "<h4>{$i18['PHONE_DL']}:</h4>";
			echo "<a href='$url'>JAD</a><br>";
*/				
			echo "<br><font color='red'>{$i18['VALID_LINKS']}</font><br><br>";
			echo "<a href={$_SERVER['REQUEST_URI']}>{$i18['BACK']}</a>";
		}
		exit;
	}
}
//print_r($_REQUEST);
?>

<script type="text/javascript">
function city_add(cname,sname){
	selc=document.getElementById("selcit");
	out="";
	count=selc.length;
	frm=document.forms.namedItem("main");
	sc=frm.elements.namedItem("sc");
	var slct=document.getElementById("chkcit");
	for(var i=0; i<slct.length; i++){
		var opt=slct.item(i);
		if(opt.selected && sc.value.indexOf(","+opt.value+",")<0){
			var newopt=document.createElement("option");
			var newtext=document.createTextNode(opt.text+", "+cname);
//		alert(newopt);return;
			newopt.value=opt.value;
			newopt.appendChild(newtext);
			selc.appendChild(newopt);
			sc.value=sc.value+opt.value+",";
			count++;
		}
		if(count><?php echo $max_cities ?>){
			alert("<?php echo sprintf($i18['CITY_LIMIT'], $max_cities) ?>");
			break;
		}
		opt.selected=false;
	}
}

function showc(country,state){
	frm=document.forms.namedItem("main");
	if(frm.elements.namedItem('cid').value!=country || frm.elements.namedItem('stateid').value!=state){
		frm.elements.namedItem('stateid').value=state;
		frm.elements.namedItem('cid').value=country;
		frm.submit();
	}
}

function city_del(removeAll){
	selc=document.getElementById("selcit");
	frm=document.forms.namedItem("main");
	sc=frm.elements.namedItem("sc");
	sc.value=","; out='';
	for(i=0; i<selc.length; i++){
		opt=selc.item(i);
		if(removeAll || opt.selected){
			selc.removeChild(opt);
			i--;
		}
		else{
			sc.value=sc.value+opt.value+",";
		}
	}
}

function check_list(){
	if(document.getElementById("selcit").length){
		document.getElementById("Action").value=1;
		document.forms.namedItem("main").submit();
	}
}
</script>
<h4>Загрузить модули городов</h4>
<form method="post" border=1 action="<?php echo $_SERVER['REQUEST_URI']?>" name="main">
<table id="city" border=1 width=100% cellspacing=0 cellpadding=0>
<tr><th colspan="4" align="left"><font color=red><?php echo $step++ ?></font>.
<select name="y_sel" style="height:auto; width:auto;" onchange="city_del(true)">
<?php
$y_now=date("Y");
for($i=0; $i<3; $i++){
	$yy=$y_now-$i;
	echo "<option value=\"$yy\"";
	if($yy==$defyear) echo "selected=\"selected\"";
	echo ">$yy\n";
}
?>
</select>
</th></tr>
<tr>
<th width=11% align=center><font color=red><?php echo $step++ ?></font>. 
<?php
	echo $i18['H_COUNTRY'];
// First listbox
	$cnum=0; $lb1='';
	if(isset($_POST['cid'])){
		$cnum=$_POST['cid'];
	}
	$sth=mysql_query("SELECT countries.id, countries.name FROM countries ORDER BY countries.name");
	while($row=mysql_fetch_row($sth)){
		if(!$cnum){
			$cnum=$row[0];
			$_POST['cid']=$cnum;
		}
		$selflag='';
		if($row[0]==$cnum){
			$cur_country=$row[1];
			$selflag=' selected';
		}
		$lb1.="<option value=$row[0]{$selflag}>{$row[1]}\n";
	}
?>
</th>
<th width=11% align=center><font color=red><?php echo $step++ ?></font>. 
<?php
	echo $i18['H_STATE'];
	$stat=sprintf("SELECT DISTINCT states.id, states.name FROM states,".
		"countries WHERE country_id=%s ORDER BY states.name",quote_smart($cnum));
	$sth=mysql_query($stat);
	$cur_state=''; $lb2='';
	$allst="<i>".$i18['ALL_STATES']."</i><br>";
	$statenum=0;
	if(isset($_POST['stateid'])){
		$statenum=$_POST['stateid'];
	}
	$selflag='';
	if(!$statenum){
		$selflag=' selected';
	}
	$state_count=mysql_num_rows($sth);

	$lb2.="<option value=0 $selflag>&gt;&gt;".$allst."&lt;&lt;\n";
	while($row = mysql_fetch_row($sth)){
		$selflag='';
		if($row[0]==$statenum){
			$cur_state=$row[1];
			$selflag=' selected';
		}
		$lb2.="<option value=$row[0]$selflag>$row[1]\n";
	}
	mysql_free_result($sth);
?>
</th>
<th width=20% align=center><font color=red><?php echo $step++ ?></font>. 	
<?php echo $i18['H_CITY']?>
<div align=right>
<input type=button class="lbbtn" size=9 style="font-family:Verdana" value='<?php echo $i18['ADD_SEL']?> &gt;&gt;' onclick='<?php echo "city_add(\"$cur_country\",\"$cur_state\");" ?>'/>
</div></th>
<th width=20% align=center><font color=red><?php echo $step++ ?></font>.
<?php echo $i18['SEL_CITIES']?>
<div align="left">
<input type=button class="lbbtn" size=9 value="&lt;&lt; <?php echo $i18['DEL_SEL']?>" style="font-family:Verdana" onclick="city_del(false);"/><nobr/><input 
	type=button class="lbbtn" onclick="check_list()" style="font-weight:bold" value="<?php echo $i18['GET_DATA']?>">
</div>
</th>
</tr>
<tr>
<td width=20% align=center valign=bottom><!-- 1st listbox -->
<select size=34 onchange="showc(item(selectedIndex).value,0);" class=lb>
<?php echo $lb1 ?>
</select>
</td>
<td width=20% align=center valign=bottom><!-- 2nd listbox -->
<?php
 echo "<select size=34 onchange=\"showc($cnum,item(selectedIndex).value);\" class=lb>";
 echo $lb2
?>
</select>
</td>
<?php
	$andst='';
	if($statenum){
		$andst=sprintf(" AND state_id=%s",quote_smart($statenum));
	}
	$stat=sprintf(
		"SELECT cities.id, cities.name FROM cities,countries".
		",locations". # year condition
		" WHERE country_id=%s AND countries.id=country_id".
		" AND city_id=cities.id %s AND year=%s". # year condition
		" ORDER BY cities.name",quote_smart($cnum), $andst, quote_smart($defyear));
	$sth = mysql_query($stat);
?>
<td align=center valign=bottom>
<div align=right>
</div>
<select id=chkcit size=34 multiple class=lb>
<?php
	while($row = mysql_fetch_row($sth)){
		echo "<option value=$row[0]>$row[1]\n";
	}
	mysql_free_result($sth);
?>
</select>
</td>
<td align=center valign=bottom>
<input type="hidden" name="Action" value=""/>
<input type="hidden" name="cid" value=""/>
<input type="hidden" name="stateid" value="0"/>
<input type="hidden" name="sc" value="<?php echo $sc ?>"/>
<select id=selcit size=34 multiple class=lb>
<?php
	$sth=get_selected_cities('sc');
	if($sth){
		while($row = mysql_fetch_row($sth)){
			echo "<option value=$row[0]>$row[1], $row[2]\n";
		}
	}
?>
</select>
</td>
</tr>
</table>
</form>

<?php
function get_selected_cities($param)
{
	global $sc;
	$sc=',';
	if(isset($_POST[$param])){
		$sc=$_POST[$param];
	}
	$sc1=trim($sc,",");
	if($sc1){
		$stat="SELECT cities.id, cities.name, countries.name FROM cities,countries WHERE cities.id IN ($sc1) and countries.id=country_id ORDER BY countries.name,cities.name";
		return mysql_query($stat);
	}
	return null;
}
?>