<?php
if(!isset($EXEC)) die("Access restricted");
$stat="SELECT id, name FROM countries ORDER BY name";
$sth=mysql_query($stat);
$i=0;
$letter='';
while($res=mysql_fetch_row($sth)){
	$row[$i]=$res;
	if(strcmp($letter, $res[1][0])){
		echo "<br/>";
		$letter=$res[1][0];
		$comma="";
	}	
	echo $comma." <a href=\"#{$res[0]}\">{$res[1]}</a>";
	$i++;
	$comma=",";
}
foreach($row as $i=>$ctry){
	$stat="SELECT name FROM cities WHERE country_id={$ctry[0]} ORDER BY name";
	if($sth=mysql_query($stat)){
		$num=mysql_num_rows($sth);
		echo "<a id=\"{$ctry[0]}\"></a><p><b>{$ctry[1]}</b> - $num &nbsp; ( <a href=\"#top\">^{$i18['UP']}</a> )<br/>";
		$comma="";
		while($row2=mysql_fetch_row($sth)){
			echo "$comma ".$row2[0];
			$comma=",";
		}
	}
	echo "</p>";
}
?>