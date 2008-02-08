<?php
include_once('../lang.php');
lang_load("source");
?>

<html>
<head>
<title>Cities database stats - Astromaximum</title>
<meta name="generator" content="Bluefish 1.0.7">
<meta name="author" content="Unknown">
<meta name="date" content="2008-01-18T19:15:53+0200">
<meta name="copyright" content="">
<meta name="keywords" content="">
<meta name="description" content="">
<meta name="ROBOTS" content="NOINDEX, NOFOLLOW">
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8">
<meta http-equiv="content-style-type" content="text/css">
<meta http-equiv="expires" content="0">
<link href="../style.css" rel="stylesheet" type="text/css">
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
if($chac!=0){ 
	echo "<br><p align=center>".$i18['DB_ACCESS']."</p>";
	emit_nav2();
	exit();
}

$mode='data';
if(isset($_POST['mode'])){
	$mode=$_POST['mode'];
}
?>
<!-- <p><a href='db_stats.php?mode=data'>Datafile summary</a>-->
<?php
	emit_admin();
if($mode=='data'){
?>
	<table style="cellpadding:0; cellspacing:0; border:1; font-size:8pt">
	<th>Country</th><th>Cities</th><th>
	<?php
		$sth=mysql_query("SELECT DISTINCT year FROM locations ORDER BY year");
		while($row=mysql_fetch_row($sth)){
			echo $row[0]."</th><th>";
			$a_years[]=$row[0];
		}
		echo "</th></tr>";
		mysql_free_result($sth);
		$sth=mysql_query("SELECT countries.id, countries.name, COUNT(cities.id) FROM countries,cities".
			" WHERE cities.country_id=countries.id GROUP BY cities.country_id ORDER BY countries.name");
		$colored=1;
		while($row=mysql_fetch_row($sth)){
			echo "<tr";
			if($colored){
				echo " bgcolor='#E5E5E5'";
			}
			$colored=1-$colored;
			echo "><td>{$row[1]}</td><td>{$row[2]}</td>";
			foreach($a_years as $i=>$value){
				$sth2=mysql_query(sprintf("select count(locations.id) from locations, cities where year=%d".
					" and locations.city_id=cities.id and cities.country_id=%d", $value, $row[0]));
				while($row2=mysql_fetch_row($sth2)){
					$bg='';
					if($row[2]>$row2[0]){
						$bg=" style='border-width:2; color:red'";
					}
					echo "<td{$bg}>{$row2[0]}</td>\n";
				}
				mysql_free_result($sth2);
			}
			echo "</tr>\n";
		}
		mysql_free_result($sth);
	?>
	</table></font>
<?php
}
emit_nav2();
?>
