<?php
include_once('lang.php');
?>

<html>
<head>
<title>Cities database stats - Astromaximum</title>
<meta name="generator" content="Bluefish 1.0.7">
<meta name="author" content="">
<meta name="date" content="2007-08-28T21:52:46+0300">
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
if($chac!=1){ 
	echo "<br><p align=center>".$i18['DB_ACCESS']."</p>";
	emit_nav2();
	exit();
}

$mode='data';
if(isset($_POST['mode'])){
	$mode=$_POST['mode'];
}
?>
<b>Admin:</b> <a href='db_stats.php'>DB stats</a> 

<p><a href='db_stats.php?mode=data'>Datafile summary</a>
<?php
if($mode=='data'){
?>
	<table cellpadding="0" cellspacing="0" border="1">
	<th>Country</th><th>
	<?php
		$sth=mysql_query("SELECT DISTINCT year FROM locations ORDER BY year");
		while($row=mysql_fetch_row($sth)){
			echo $row[0]."</th><th>";
		}
		echo "</th></tr>";
		mysql_free_result($sth);
		$sth=mysql_query("SELECT countries.id, countries.name, COUNT(cities.id) FROM countries,cities".
			" WHERE cities.country_id=countries.id GROUP BY cities.country_id ORDER BY countries.name");
		while($row=mysql_fetch_row($sth)){
			echo "<tr><td>{$row[1]}</td>";
			$sth2=mysql_query(sprintf("SELECT COUNT(locations.id) FROM locations,cities WHERE locations.city_id=cities.id AND cities.country_id=%d GROUP BY year ORDER BY year",$row[0]));
			while($row2=mysql_fetch_row($sth2)){
				echo "<td>{$row2[0]}</td>\n";
			}
			mysql_free_result($sth2);
			echo "</tr>\n";
		}
		mysql_free_result($sth);
	?>
	</table>
<?php
}
?>

<?php

emit_nav2();
?>
