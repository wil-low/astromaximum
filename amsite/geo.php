<?php
include_once('lang.php');
?>

<html>
<head>
<title>Cities database - Astromaximum</title>
<meta name="generator" content="Bluefish 1.0.7">
<meta name="author" content="Unknown">
<meta name="date" content="2007-09-30T12:44:24+0300">
<meta name="copyright" content="">
<meta name="keywords" content="">
<meta name="description" content="">
<meta name="ROBOTS" content="NOINDEX, NOFOLLOW">
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8">
<meta http-equiv="content-style-type" content="text/css">
<meta http-equiv="expires" content="0">
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
if(!$chac){ 
	echo "<br><p align=center>".$i18['DB_ACCESS']."</p>";
	emit_nav2();
	exit();
}
if($chac==1){
	emit_admin();
} 
?>
<h3 align=center><?php echo $i18['DB']?></h3>

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
<form method="post" action="<?php echo $_SERVER['REQUEST_URI']?>" name="main">
<table width=100% border=1><tr valign=top><td>
<font color='red'><i><?php echo $i18['STEP']?> 1:</i></font>
<br><b><?php echo $i18['YEAR']?>:</b> 
<input type="hidden" name="cid" value=""  />
<input type="hidden" name="stateid" value="0"  />
<select name="year" onchange="javascript:document.forms.namedItem('main').submit()">
<?php
	$years=array(2005,2006,2007,2008);
	foreach($years as &$y){
		$sel='';
		if($y==$defyear){
			$sel='selected=1 ';
		}
		echo "<option value=$y $sel>$y</option>\n";
	}
?>
</select></td>
<td rowspan=2 width=25%><center><b><?php echo $i18['SEL_CITIES']?>:</b></center>
<div align=right><input type='button'  value='Delete selected' onclick='city_del()' /></div>
<div id=selcit>
<?php
	$sc=',';
	if(isset($_POST['sc'])){
		$sc=$_POST['sc'];
	}
	$sc1=trim($sc,",");
	if($sc1){
		$stat="SELECT cities.id, cities.name, countries.name FROM cities,countries WHERE cities.id IN ($sc1) and countries.id=country_id ORDER BY countries.name,cities.name";
		$sth = mysql_query($stat);
		while($row = mysql_fetch_row($sth)){
			echo "<input type=checkbox name=sss id=$row[0]></input>$row[1], $row[2]<br>\n";	
		}
	}
?>
</div>
<p align=center><font color='red'><i>Step 3:</i></font>
<input type="hidden" name="sc" value="<?php echo $sc ?>"  /> 
<input type=submit name='Action' value='Get data'></p></td></tr>
<tr><td><font color='red'><i><?php echo $i18['STEP']?> 2:</i></font>
<br><b><?php echo $i18['COUNTRY']?>: </b>

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
		echo "<a href='#' onclick='showc({$row[0]},0)'>{$row[1]}</a>&nbsp;\n";
	}
	mysql_free_result($sth);
	$stat=sprintf("SELECT DISTINCT states.id, states.name FROM states,".
		"countries WHERE country_id=%s ORDER BY states.name",quote_smart($cnum));
	$sth=mysql_query($stat);	
	$cur_state='';
	$allst="&lt;".$i18['ALL_STATES']."&gt;";
	$statenum=0;
	if(isset($_POST['stateid'])){
		$statenum=$_POST['stateid'];
	}
	if(!$statenum){
		$allst="<font color=red>$allst</font>";
	}
	if(mysql_num_rows($sth)){
		echo "<hr><a href='#' onclick=\"showc(".$cnum.",0)\">".$allst."</a>&nbsp;\n";
		while($row = mysql_fetch_row($sth)){
#			if(!$statenum){
#				$statenum=$row[0];
#				param('stateid',$statenum);
#			}
			if($row[0]==$statenum){
				$cur_state=$row[1];
				$row[1]="<font color=red>$row[1]</font>" ;
			}
			echo "<a href='#' onclick=\"showc($cnum,$row[0])\">$row[1]</a>&nbsp;\n"; 
		}
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
	echo "<hr><div id=chkcit>";
	$i=0;
	while($row = mysql_fetch_row($sth)){
		echo "<input type=checkbox id=$row[0] value='$row[1]'>$row[1]</input>\n"; 
		$i++;
	}
	echo "</div>";
	mysql_free_result($sth);
	if($i>0){
		echo "<input type=button value='{$i18['ADD_CITIES']}' onClick='city_add(\"$cur_country\",\"$cur_state\")'/>";
	}
	else{
		echo "<i>{$i18['NO_CITIES']}</i>";
	}

?>
</td></tr></table>
</form>

<?php
	if(isset($_POST['Action']) && ($_POST['Action']==$i18['GET_DATA']) && 
			isset($_POST['sc'])){
		$sc=$_POST['sc'];
		if($sc!=","){
			$id=create_jar($defyear, $sc);
			$url='data.php?r='.$id;
			echo "<p><center><font color='red'><i>{$i18['STEP']} 4:</i></font>";
			echo "<h4>{$i18['PC_DL']}:</h4>";
			echo "<b>{$i18['JAR_LINK']}: <a href='$url'>$id</a><br><br>";
			$url=str_replace("?r", "?d", $url);
			echo "{$i18['JAD_LINK']}: <a href='$url'>$id</a><br><br></b>";
			$url=str_replace("?d", "?t", $url);
			echo "<h4>{$i18['PHONE_DL']}:</h4>";
			echo "<b>{$i18['DIRECTLINK']}: <a href='$url'>$id</a><br>";
			echo "<br><font color='red'>{$i18['VALID_LINKS']}</font></b></center>";
		}
	}
	emit_nav2();

function create_jar($year, $ids){
	global $DIR_FILES, $DIR_SOURCE;
	$ids=trim($ids,',');
	include_once('amtools.php');
	list($dir,$fn)=amtools_random($DIR_FILES,'.r');
	
	$srcdir="/tmp/$fn";
	mkdir($srcdir);
	$infile=fopen("$DIR_SOURCE/template.jad","rb");
	$template = fread($infile, 1000000);
	fclose($infile);
	$code="-".substr($fn,-4);
	$fname="Cities$code";
	$ye=substr($year,-2);
	$template=str_replace('<YEAR>', $ye, $template);
#	$jad=~s/<REGION>/$reg/isg;
	$template=str_replace('<CODE>', $code, $template);
#	$jad=~s/<DESC>/$desc/isg;
	$template=str_replace('<JAR>', "$fname.jar", $template);
#	echo $template;
	
	$server="http://".$_SERVER['SERVER_NAME'];
	$stat=sprintf("SELECT DISTINCT cities.name, data FROM cities, locations ".
		"WHERE cities.id IN (%s) AND city_id=cities.id AND year=%s".
		" ORDER BY cities.name",$ids,$year);
#	print $stat;
	$sth = mysql_query($stat);
	$i=0;
	while($row = mysql_fetch_row($sth)){
		$data[$i++]=$row[1];		
	}
	mysql_free_result($sth);
	$cmd=sprintf($UNZIP, "$DIR_SOURCE/template.zip", "$DIR_SOURCE/$fn");
#	echo $cmd;
	system($cmd);
	
	$inf=fopen("$DIR_SOURCE/$fn/META-INF/MANIFEST.MF", 'wb');
	fwrite($inf, $template);
	fclose($inf);
	join_datafiles2("$DIR_SOURCE/$fn/locations.dat", $data);
	$inf=fopen("$DIR_SOURCE/icons/".substr($year,-1).".png", 'rb');
	$icon=fread($inf,5000);
	fclose($inf);
	$inf=fopen("$DIR_SOURCE/$fn/icon.png", 'wb');
	fwrite($inf,$icon);
	fclose($inf);
	$cmd=sprintf($ZIP, "$DIR_SOURCE/$fn", "$DIR_FILES/$fn");
#	echo "<br>$cmd<br><pre>";
	system($cmd);
#	echo "</pre>";
	$inf=fopen("$DIR_FILES/$fn.d", 'wb');
	$asize= filesize("$DIR_FILES/$fn.r");
	$template.="MIDlet-Jar-Size: $asize\n";
	fwrite($inf, $template);
	fclose($inf);
	$template=preg_replace('/(MIDlet-Jar-URL: ).+?\n/is',"$1$server/data.php?r=$fn\n", $template);
	$inf=fopen("$DIR_FILES/$fn.t", 'wb');
	fwrite($inf, $template);
	fclose($inf);
	system("rm -R $DIR_SOURCE/$fn");
/*	
	my $sql='INSERT INTO files (id, type, user_id, end_tm) VALUES';
	foreach (('r','d','t')){
		$sql.=" ($fn, \'$_\', ".$userid.", NOW()+ INTERVAL 2 HOUR),";
	}
	$sql=~s/,$//is;
	$sth = $dbh->prepare($sql)|| die $dbh->errstr;
	$sth->execute|| die $dbh->errstr;
	$sth->finish;
*/	
	return $fn;
}
	
?>
