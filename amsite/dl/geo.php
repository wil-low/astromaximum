<?php
include_once('../lang.php');
?>

<html>
<head>
<title>Cities database - Astromaximum</title>
<meta name="generator" content="Bluefish 1.0.7">
<meta name="author" content="Unknown">
<meta name="date" content="2007-11-17T16:54:40+0200">
<meta name="copyright" content="">
<meta name="keywords" content="">
<meta name="description" content="">
<meta name="ROBOTS" content="NOINDEX, NOFOLLOW">
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8">
<meta http-equiv="content-style-type" content="text/css">
<meta http-equiv="expires" content="0">
<style type="text/css">
<!--
body,td,th {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	color: #000000;
	font-size: 10pt;
}
body {
	margin-left: 0px;
	margin-top: 0px;
	margin-right: 0px;
	margin-bottom: 0px;
}
-->
</style>
<link href="../../style.css" rel="stylesheet" type="text/css">
</head>
<body>
<?php
include('nav.php');
emit_nav1();
$defyear=2007;
if(isset($_POST['year'])){
	$defyear=$_POST['year'];
}
$chac=check_access();
/*
if(!$chac){ 
	echo "<br><p align=center>{$i18['DB_ACCESS']}</p>";
	emit_nav2();
	exit();
}
*/
if($chac==1){
	emit_admin();
} 
$sc='';
?>
<!-- <h3 align=center><?php echo $i18['DB']?></h3> -->

<script>
function city_add(cname,sname){
	selc=document.getElementById("selcit");
	out="";
	frm=document.forms.namedItem("main");
	sc=frm.elements.namedItem("sc");
	for(i=0; i<frm.elements.length; i++){
		opt=frm.elements.item(i);
		if(opt.type=="checkbox" && opt.checked && sc.value.indexOf(","+opt.id+",")<0){
			out=out+"<input type=checkbox name=sss id="+opt.id+">"+opt.value;
//			if(sname!=""){
//				out=out+", "+sname;
//			}
			out=out+", "+cname+"</input><br>";
			sc.value=sc.value+opt.id+",";
		}
	}
	selc.innerHTML=selc.innerHTML+out;
};

function showc(country,state){
	frm=document.forms.namedItem("main");
	if(frm.elements.namedItem('cid').value!=country || frm.elements.namedItem('stateid').value!=state){
		frm.elements.namedItem('stateid').value=state;
		frm.elements.namedItem('cid').value=country;
		frm.submit();
	}
};

function city_del(){
	frm=document.forms.namedItem("main");
	sc=frm.elements.namedItem("sc");
	oldsc=sc.value;
	sc.value=",";
	for(i=0; i<frm.elements.length; i++){
		opt=frm.elements.item(i);
		if(opt.type!="checkbox") continue;
		if( opt.name!="sss") continue;
		if(!opt.checked){
			sc.value=sc.value+opt.id+",";
		}
	}
	if(oldsc!=sc.value){
		frm.submit();
	}
};
</script>
<?php
	if(isset($_POST['Action']) && ($_POST['Action']==$i18['GET_DATA']) && 
			isset($_POST['sc'])){
		$sth=get_selected_cities('sc');
		if($sth){
			echo "<p>You have selected:</p>";
			while($row = mysql_fetch_row($sth)){
				echo "$row[1], $row[2]; \n";	
			}
			include_once('../amtools.php');
			$id=create_jar($defyear, $sc);
			$url='../data.php?r='.$id;
			echo "<center><h4>{$i18['PC_DL']}:</h4>";
			echo "<b>{$i18['JAR_LINK']}: <a href='$url'>$id</a><br><br>";
			$url=str_replace("?r", "?d", $url);
			echo "{$i18['JAD_LINK']}: <a href='$url'>$id</a><br><br></b>";
			$url=str_replace("?d", "?t", $url);
			echo "<h4>{$i18['PHONE_DL']}:</h4>";
			echo "<b>{$i18['DIRECTLINK']}: <a href='$url'>$id</a><br>";
			echo "<br><font color='red'>{$i18['VALID_LINKS']}</font></b></center>";
		}
		emit_nav2();
		exit(0);
	}
?>
<form method="post" action="<?php echo $_SERVER['REQUEST_URI']?>" name="main">
<table class=geo border="1" width="100%">
<tr><td colspan=4>
<font color='red'><i><?php echo $i18['STEP']?> 1:</i></font>
<b><?php echo $i18['YEAR']?> </b> 
<input type="hidden" name="cid" value=""  />
<input type="hidden" name="stateid" value="0"  />
<select name="year" onchange="javascript:document.forms.namedItem('main').submit()">
<?php
	$years=array(2005,2006,2007,2008);
	foreach($years as $y){
		$sel='';
		if($y==$defyear){
			$sel='selected=1 ';
		}
		echo "<option value=$y $sel>$y</option>\n";
	}
?>
</select></td></tr>
<tr>
<td colspan=3><font color='red'><i><?php echo $i18['STEP']?> 2:</i></font>
<b><?php echo $i18['CHOICE']?></b></td>
<td width=25% rowspan=2 class=geo>
<center><b><?php echo $i18['SEL_CITIES']?>:</b></center>
<div align=right><input type='button'  value='<?php echo $i18['DEL_SEL']?>' onclick='city_del()' /></div>
<div id=selcit>
<?php
	$sth=get_selected_cities('sc');
	if($sth){
		while($row = mysql_fetch_row($sth)){
			echo "<input type=checkbox name=sss id=$row[0]></input>$row[1], $row[2]<br>\n";	
		}
	}
?>
</div>
<p align=center><font color='red'><i><?php echo $i18['STEP']?> 3:</i></font>
<input type="hidden" name="sc" value="<?php echo $sc ?>"  /> 
<input type=submit name='Action' value='<?php echo $i18['GET_DATA'] ?>'></p></td>


</tr>
<tr><td width=15% class=geo>
<?php
	$cnum=0;
	if(isset($_POST['cid'])){
		$cnum=$_POST['cid'];
	}
	$sth=mysql_query("SELECT countries.id, countries.name FROM countries ORDER BY countries.name");
	while($row=mysql_fetch_row($sth)){
		if(!$cnum){
			$cnum=$row[0];
			$_POST['cid']=$cnum;
		}
		if($row[0]==$cnum){
			$cur_country=$row[1];
			$row[1]="<font color=red>$row[1]</font>" ;
		}
		echo "<a href='#' onclick='showc({$row[0]},0)'>{$row[1]}</a><br>\n";
	}
	mysql_free_result($sth);
	echo "</td>";
	$stat=sprintf("SELECT DISTINCT states.id, states.name FROM states,".
		"countries WHERE country_id=%s ORDER BY states.name",quote_smart($cnum));
	$sth=mysql_query($stat);	
	$cur_state='';
	$allst="<i>".$i18['ALL_STATES']."</i><br>";
	$statenum=0;
	if(isset($_POST['stateid'])){
		$statenum=$_POST['stateid'];
	}
	if(!$statenum){
		$allst="<font color=red>$allst</font>";
	}
	$state_count=mysql_num_rows($sth);
	if($state_count){
		echo "<td width=16% class=geo><a href='#' onclick=\"showc(".$cnum.",0)\">".$allst."</a>&nbsp;\n";
		while($row = mysql_fetch_row($sth)){
#			if(!$statenum){
#				$statenum=$row[0];
#				param('stateid',$statenum);
#			}
			if($row[0]==$statenum){
				$cur_state=$row[1];
				$row[1]="<font color=red>$row[1]</font>" ;
			}
			echo "<br><a href='#' onclick=\"showc($cnum,$row[0])\">$row[1]</a>\n"; 
		}
	}
	else{
			echo "<td width=1></td>\n";
	}
	mysql_free_result($sth);

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
	if($state_count){
		$city_cols=3;
	}
	else{
		$city_cols=4;
	}
	$i=mysql_num_rows($sth); $j=0;
	$city_rows=$i/$city_cols;
	echo "</td></td><td class=geo>";
	if($i>0){
		echo "<center><input type=button value='{$i18['ADD_CITIES']}' onClick='city_add(\"$cur_country\",\"$cur_state\")'/></center><br><br>";
	}
	echo "<div id=chkcit><table width=100%><tr>";
	for($cc=0; $cc<$city_cols; $cc++){
		echo "<td class=geo>";
		while($row = mysql_fetch_row($sth)){
			echo "<input type=checkbox id=$row[0] value='$row[1]'>$row[1]</input><br>\n"; 
			$j++;
			if($j>=$city_rows){
				$j=0;
				break;
			}
		}
		echo "</td>";
	}
	echo "</tr></table></div>";
	mysql_free_result($sth);
	if(!$i){
		echo "<i>{$i18['NO_CITIES']}</i>";
	}

?>
</td>
</tr>
</table>
</form>

<?php
	emit_nav2();

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
