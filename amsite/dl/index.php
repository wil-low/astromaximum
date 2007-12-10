<?php
include_once('../lang.php');
?>

<html>
<head>
<title>Cities database - Astromaximum</title>
<meta name="generator" content="Bluefish 1.0.7">
<meta name="author" content="Unknown">
<meta name="date" content="2007-12-10T21:01:13+0200">
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
	frm=document.forms.namedItem("main");
	sc=frm.elements.namedItem("sc");
	slct=document.getElementById("chkcit");
	for(i=0; i<slct.length; i++){
		opt=slct.item(i);
		if(opt.selected && sc.value.indexOf(","+opt.value+",")<0){
			out=out+"<option value="+opt.value+">"+opt.text+", "+cname+"\n";	
			sc.value=sc.value+opt.value+",";
		}
		opt.selected=false;
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
	selc=document.getElementById("selcit");
	frm=document.forms.namedItem("main");
	sc=frm.elements.namedItem("sc");
	sc.value=","; out='';
	for(i=0; i<selc.length; i++){
		opt=selc.item(i);
		if(!opt.selected){
			sc.value=sc.value+opt.value+",";
			out=out+"<option value="+opt.value+">"+opt.text+"\n";	
		}
	}
	selc.innerHTML=out;
};
</script>
</head>
<body>
<?php
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
	$sc=',';
	if(isset($_POST['sc'])){
		$sc=$_POST['sc'];
	}
	if(isset($_POST['Action']) && isset($_POST['sc'])){
		$sth=get_selected_cities('sc');
		if($sth){
			echo "<h4>You have selected following cities:</h4>\n<ol>";
			while($row = mysql_fetch_row($sth)){
				echo "<li>$row[1], $row[2]</li>\n";	
			}
			echo "</ol>\n";
			include_once('../amtools.php');
			$id=create_jar($defyear, $sc, 0);
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
		}
		exit(0);
	}
?>
<form method="post" action="<?php echo $_SERVER['REQUEST_URI']?>" name="main">
<table class=geo border=1 width=100% cellspacing=0 cellpadding=0>
<tr><td colspan=6>&nbsp;</td></tr>
<tr><td width=11% align=center><font color=red>1</font>. World region (<a href=#>all countries</a>)
</td>
<td width=11% align=center><font color=red>2</font>. Country
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
		$lb1.="<option onclick='showc({$row[0]},0)'{$selflag}>{$row[1]}\n";
	}
?>
</td>
<td width=11% align=center><font color=red>3</font>. State
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

	$lb2.="<option onclick=\"showc(".$cnum.",0)\"$selflag>&gt;&gt;".$allst."&lt;&lt;\n";
	while($row = mysql_fetch_row($sth)){
		$selflag='';
		if($row[0]==$statenum){
			$cur_state=$row[1];
			$selflag=' selected';
		}
		$lb2.="<option onclick=\"showc($cnum,$row[0])\"$selflag>$row[1]\n"; 
	}
	mysql_free_result($sth);
?>	
</td>
<td width=22% align=center><font color=red>4</font>. City
</td>
<td width=22% align=center><font color=red>5</font>. 
<input type=submit name='Action' value='Make midlet'></p></td>
</td>
</tr>
<tr><td width=188>
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
			</table></td>
<td width=22% align=center valign=bottom><!-- 1st listbox -->
<select size=33 style="width:100%; height:100%">
<?php echo $lb1 ?>
</select>
</td>
<td width=22% align=center valign=bottom><!-- 2nd listbox -->
<select size=33 style="width:100%; height:100%">
<?php echo $lb2 ?>
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
<div align=right><input type=button style="margin-bottom:2pt" value='Add >>' onClick='<?php echo "city_add(\"$cur_country\",\"$cur_state\")" ?>'/>
&nbsp;&nbsp;</div>
<select id=chkcit size=33 multiple style="width:100%">
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
<div align=left>
&nbsp;&nbsp;<input type=button style="margin-bottom:2pt" value='<< Remove' onClick='city_del()'/>
</div>
<select id=selcit size=33 style="width:100%">
<?php
	$sth=get_selected_cities('sc');
	if($sth){
		while($row = mysql_fetch_row($sth)){
			echo "<option value=$row[0]>$row[1], $row[2]\n";	
		}
	}
?>
</select>
</td></tr>
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

