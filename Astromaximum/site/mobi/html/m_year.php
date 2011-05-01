<?php
	if(!isset($EXEC)) die("Access restricted");
    $where='';
    addNavItem('year', 'cities', 2);
    $subtitle=$i18['MOBI_YEAR_H'];
    if($chac==-1){ // demo - previous year only
        $where='WHERE year='.($GLOBALS['amax']['year']-1);
    }
    $sth=mysql_query("SELECT DISTINCT year FROM locations $where ORDER BY year DESC");
    while($row=mysql_fetch_row($sth)){
        echo "<a href=\"?$lang_&amp;p=geo&amp;y=".$row[0]."&amp;ent=".$row[0]."&amp;$sess\">".$row[0]."</a> \n";
    }
    
?>
