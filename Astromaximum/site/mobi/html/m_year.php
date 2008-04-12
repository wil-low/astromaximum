<?php
	if(!isset($EXEC)) die("Access restricted");
	if($user_ok){
		$sth=mysql_query("SELECT DISTINCT year FROM locations ORDER BY year");
		while($row=mysql_fetch_row($sth)){
			echo "<a href=\"?$lang_&amp;p=geo&amp;y=".$row[0]."&amp;$sess\">".$row[0]."</a><br/>";
		}
	}
?>
