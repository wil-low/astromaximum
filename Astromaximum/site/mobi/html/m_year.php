<?php
	if(!isset($EXEC)) die("Access restricted");
    $where='';
    if($chac<0) return;
    addNavItem('year', 'cities', 2);
    $subtitle=$i18['MOBI_YEAR_H'];
    if($chac==1){ // demo - previous year only
        $where='WHERE year='.($GLOBALS['amax']['year']-1);
    }
    $sth=mysql_query("SELECT DISTINCT year FROM locations $where ORDER BY year");
    while($row=mysql_fetch_row($sth)){
        echo "<a href=\"?$lang_&amp;p=geo&amp;y=".$row[0]."&amp;$sess\">".$row[0]."</a><br/>";
    }
    
?>
