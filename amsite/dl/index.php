<?php
include_once('../dbconnect.php');
include_once('nav.php');
$useimg=1;
$useregions=0;
$step=1;
$max_cities=20;
?>

<html>
<head>
<title>Cities database - Astromaximum</title>
<meta name="generator" content="Bluefish 1.0.7">
<meta name="author" content="Unknown">
<meta name="date" content="2007-12-28T20:21:21+0200">
<meta name="copyright" content="">
<meta name="keywords" content="">
<meta name="description" content="">
<meta name="ROBOTS" content="NOINDEX, NOFOLLOW">
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8">
<meta http-equiv="content-style-type" content="text/css">
<link href="../style.css" rel="stylesheet" type="text/css">
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
			alert("Sorry, you may select up to <?php echo $max_cities ?> cities.");
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

function city_del(){
	selc=document.getElementById("selcit");
	frm=document.forms.namedItem("main");
	sc=frm.elements.namedItem("sc");
	sc.value=","; out='';
	for(i=0; i<selc.length; i++){
		opt=selc.item(i);
		if(opt.selected){
			selc.removeChild(opt);
			i--;
		}
		else{
			sc.value=sc.value+opt.value+",";
		}
	}
}
</script>
</head>
<body>
<?php
	$defyear=2007;
	if(isset($_POST['year'])){
		$defyear=$_POST['year'];
	}
	$chac=check_access();
//	emit_nav1();
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
	$sc=',';
	if(isset($_POST['sc'])){
		$sc=$_POST['sc'];
	}
	if(isset($_POST['Action']) && isset($_POST['sc'])){
		$sth=get_selected_cities('sc');
		if(strlen($sth)>0){
			echo "<h4>You have selected following cities:</h4>\n<ol>";
			while($row = mysql_fetch_row($sth)){
				echo "<li>$row[1], $row[2]</li>\n";
			}
			echo "</ol>\n";
			include_once('../amtools.php');
			$id=create_jar($defyear, $sc, 'source/template.zip', 0, '', 'GeoInstaller', '');
			$url='../data.php?r='.$id;
			echo "<h4>{$i18['PC_DL']}:</h4>";
			echo "{$i18['JAR_LINK']}: <a href='$url'>$id</a><br><br>";
			$url=str_replace("?r", "?d", $url);
			echo "{$i18['JAD_LINK']}: <a href='$url'>$id</a><br><br>";
			$url=str_replace("?d", "?t", $url);
			echo "<h4>{$i18['PHONE_DL']}:</h4>";
			echo "{$i18['DIRECTLINK']}: <a href='$url'>$id</a><br>";
			echo "<br><font color='red'>{$i18['VALID_LINKS']}</font><br><br>";
			echo "<a href={$_SERVER['PHP_SELF']}>Back</a>";
			exit(0);
		}
	}
?>
<form method="post" border=1 action="<?php echo $_SERVER['REQUEST_URI']?>" name="main">
<table class=geo border=1 width=100% cellspacing=0 cellpadding=0>
<tr>
<?php if($useimg){ ?>
<td width=11% align=center>
<?php if($useregions){ ?>
<font color=red><?php echo $step++ ?></font>. World region (<a href=#>all countries</a>)
<?php } ?>
&nbsp;</td>
<?php } ?>
<td width=11% align=center><font color=red><?php echo $step++ ?></font>. Country
<?php
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
</td>
<td width=11% align=center><font color=red><?php echo $step++ ?></font>. State
<?php
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
</td>
<td width=20% align=center><font color=red><?php echo $step++ ?></font>. City
</td>
<td width=20% align=center><font color=red><?php echo $step++ ?></font>.
Selected cities
</td>
<td width=20% align=center><b><font size=+3><?php echo $defyear ?></font></b>
</td>
</tr>
<tr>
<?php if($useimg){ ?>
	<td width=188>
<?php	if($useregions){ ?>
    <table border="0">
    <tr>
    	<td width="100">
				<img src="img/europe.png" alt="Europe" height="88" width="88" border="1">
			</td>
			<td>
				<div align=center>Europe:</div>
				<a href=# class="nav">Nothern</a><br><a href=# class="nav">Western</a><br>
				<a href=# class="nav">Southern</a><br><a href=# class="nav">Eastern</a><br>
			</td></tr>
    <tr>
    	<td width="25%">
				<img src="img/america.png" alt="America" height="88" width="88" border="1">
			</td>
			<td>
				<div align=center>America:</div>
				<a href=# class="nav">Nothern</a><br><a href=# class="nav">Central</a><br>
				<a href=# class="nav">Southern</a><br><a href=# class="nav">Caribbeans</a><br>
			</td></tr>
    <tr>
    	<td width="25%">
				<img src="img/asia1.png" alt="Asia" height="88" width="88" border="1">
			</td>
			<td>
				<div align=center>Asia:</div>
				<a href=# class="nav">Western</a><br><a href=# class="nav">Central</a><br>
				<a href=# class="nav">Southern</a>
			</td></tr>
    <tr>
    	<td width="25%">
				<img src="img/asia2.png" alt="Asia" height="88" width="88" border="1">
			</td>
			<td>
				<div align=center>Asia:</div>
				<a href=# class="nav">Eastern</a><br><a href=# class="nav">Southeastern</a><br>
			</td></tr>
    <tr>
    	<td width="25%">
				<img src="img/australia.png" alt="Australia" height="88" width="88" border="1">
			</td>
			<td>
				<div align=center>Australia:</div>
				<a href=# class="nav">Southeastern Asia</a><br><a href=# class="nav">Australia</a><br>
				<a href=# class="nav">Polinesia</a>
			</td></tr>
    <tr>
    	<td width="25%">
				<img src="img/africa.png" alt="Africa" height="88" width="88" border="1">
			</td>
			<td>
				<div align=center>Africa:</div>
				<a href=# class="nav">Nothern</a><br><a href=# class="nav">Western</a><br>
				<a href=# class="nav">Middle</a><br><a href=# class="nav">Eastern</a><br>
				<a href=# class="nav">Southern</a>
			</td></tr>
			</table>
<?php }
	else{
?>
<table border="0">
	<tr><td><img src="img/europe.png" alt="Europe" height="88" width="88" border="1"></td></tr>
	<tr><td><img src="img/america.png" alt="America" height="88" width="88" border="1"></td></tr>
	<tr><td><img src="img/asia1.png" alt="Asia" height="88" width="88" border="1"></td></tr>
	<tr><td><img src="img/asia2.png" alt="Asia" height="88" width="88" border="1"></td></tr>
	<tr><td><img src="img/australia.png" alt="Australia" height="88" width="88" border="1"></td></tr>
	<tr><td><img src="img/africa.png" alt="Africa" height="88" width="88" border="1"></td></tr>
</table>
<?php } ?>
		</td>
<?php } ?>
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
<div align=right><input type=button size=9 style="font-family:Verdana" value='Insert &gt;&gt;' onclick='<?php echo "city_add(\"$cur_country\",\"$cur_state\");" ?>'/>
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
<input type="hidden" name="cid" value=""  />
<input type="hidden" name="stateid" value="0"  />
<input type="hidden" name="sc" value="<?php echo $sc ?>"  />
<div align=left valign=top>
<input type=button size=9 value='&lt;&lt; Remove' style="font-family:Verdana" onclick='city_del();'/>
</div>
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
<td align=center valign=top cellpadding=1><input type=submit style="font-family:Verdana" name='Action' value='Make midlet'>
<br><br><a href="../">Home</a>
</td>
</tr>
</table>
</form>
</body>
</html>

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

