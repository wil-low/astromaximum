<?php
	if(!isset($EXEC)) die("Access restricted");
	foreach($_GLOBALS['amax']['demo_cities'] as $i=>$city){
		echo '<a href="?'.$lang_.'&amp;p=geo&amp;lvl=10&amp;p0='.$i."&amp;$sess\"/> ".$city.'</a><br/>';
	}
?>
