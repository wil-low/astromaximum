<?php
if(!isset($EXEC)) die("Access restricted");
reject2index("index.php?$lang_");
include_once('mobi/amtools.php');
lang_load("source");
?>
<p></p>
<table class="compressed">
<tr><th>Country</th><th>Cities</th>
<?php
	$sth=mysql_query("SELECT DISTINCT year FROM locations ORDER BY year");
	while($row=mysql_fetch_row($sth)){
		echo "<th>$row[0]</th>";
		$a_years[]=$row[0];
	}
	echo "</tr>";
	mysql_free_result($sth);
	$sth=mysql_query("SELECT countries.id, countries.name, COUNT(cities.id) FROM countries,cities".
		" WHERE cities.country_id=countries.id GROUP BY cities.country_id ORDER BY countries.name");
	$colored=1;
	while($row=mysql_fetch_row($sth)){
		echo "<tr";
		if($colored){
			echo ' bgcolor="#E5E5E5"';
		}
		$colored=1-$colored;
		echo "><td>{$row[1]}</td><td>{$row[2]}</td>";
		foreach($a_years as $i=>$value){
			$sth2=mysql_query(sprintf("select count(locations.id) from locations, cities where year=%d".
				" and locations.city_id=cities.id and cities.country_id=%d", $value, $row[0]));
			while($row2=mysql_fetch_row($sth2)){
				$bg='';
				if($row2[0]==0){
					$row2[0]='';
				}
				else{
					if($row[2]>$row2[0]){
						$bg=" style=\"color:red\"";
					}
				}
				echo "<td{$bg}>{$row2[0]}</td>\n";
			}
			mysql_free_result($sth2);
		}
		echo "</tr>\n";
	}
	mysql_free_result($sth);
?>
</table>

