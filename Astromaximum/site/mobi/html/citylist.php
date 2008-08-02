<?php
if(!isset($EXEC)) die("Access restricted");
if(isset($_GET['n']) && $chac>=0){
	$num=intval($_GET['n']);
	$current_year=get_year();
	$year=0;
	if(isset($_GET['y']))
		$year=intval($_GET['y']);
	if($year>$current_year || $year<$GLOBALS['amax']['min_demo_year']){
		$year=$current_year;
	}
	$month=date("m"); $day=date("d");
	echo '<p> | ';
	for($yy=$current_year; $yy>=$GLOBALS['amax']['min_demo_year']; $yy--){
		if($year==$yy){
			echo "<b>$yy</b> | ";
		}
		else{
			echo "<a href=\"?$lang_&amp;p=citylist&amp;n=$num&amp;y=$yy\">$yy</a> | ";
		}
	}
	echo '</p>';
	$stat=sprintf("SELECT data FROM locations WHERE year=%d AND city_id=%d",
		quote_smart($year), quote_smart($num));
	$sth=mysql_query($stat);
	if(!$sth || mysql_num_rows($sth)!=1){ 
		echo "<h4>{$i18['CITYLIST_NOCITY']}</h4>";
	}
	else{
		$row=mysql_fetch_row($sth);		
		$tmpfname = tempnam("/tmp", "SR");
		$handle = fopen($tmpfname, "wb");
		fwrite($handle, $row[0]);
		fclose($handle);
		$cmd="mobi/sunrise $tmpfname";
		$ret=0;
//		echo $cmd;
		exec($cmd, $outp, $ret);
		unlink($tmpfname);
		if($ret){
			echo "<h4>{$i18['CITYLIST_SUNRISE_ERROR']} $ret</h4>";
		}
		else{
			$stat=sprintf("SELECT cities.name,countries.name FROM cities,countries ".
				"WHERE cities.id=%d AND countries.id=country_id", quote_smart($num));
			$sth=mysql_query($stat);
			$row=mysql_fetch_row($sth);
			echo "<h4>{$row[0]}, {$row[1]}</h4>";		
			echo "<p>{$i18['CITYLIST_SUNRISE']}: {$outp[0]}</p>";
			echo "<p>{$i18['CITYLIST_NOW']}: {$outp[1]}</p>";
			$outp[2]=str_replace('DST', $i18['CITYLIST_DST'], $outp[2]);
			echo "<p>{$i18['CITYLIST_GMT']}: {$outp[2]}</p>";
	//		print_r($outp);
		}
	}
	echo "<p><a href=\"?$lang_&amp;p=citylist\">{$i18['MNU_CITYLIST']}</a></p>";
	return;
}	
if($chac>=0){
	echo "{$i18['CITYLIST_CLICK']}<br/>";
}
$stat="SELECT id, name FROM countries ORDER BY name";
$sth=mysql_query($stat);
$i=0;
$letter='';
$row=array();
while($res=mysql_fetch_row($sth)){
	$row[$i]=$res;
	if(strcmp($letter, $res[1][0])){
		echo "\n<br/>";
		$letter=$res[1][0];
		$comma="";
	}	
	echo $comma." <a href=\"#n{$res[0]}\">{$res[1]}</a>";
	$i++;
	$comma=",";
}
foreach($row as $i=>$ctry){
	$stat="SELECT name, id FROM cities WHERE country_id={$ctry[0]} ORDER BY name";
	if($sth=mysql_query($stat)){
		$num=mysql_num_rows($sth);
		echo "\n<p><a id=\"n{$ctry[0]}\"></a><b>{$ctry[1]}</b> - $num &nbsp; ( <a href=\"#top\">^{$i18['UP']}</a> )<br/>\n<span class=\"city\">\n";
		$comma="";
		while($row2=mysql_fetch_row($sth)){
			if($chac>=0){
				$row2[0]="<a href=\"?$lang_&amp;p=citylist&amp;n={$row2[1]}\">{$row2[0]}</a>";
			}
			echo "$comma ".$row2[0];
			$comma=",";
		}
		echo "</span></p>";
	}
}
?>