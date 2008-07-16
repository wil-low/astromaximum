<?php 
if(!isset($EXEC)) die("Access restricted");
$META_TITLE='Скриншоты';
$META_KEYWORDS='';
$META_DESCR='';
?>
<p><table class="gallery">
<?php 
for($i=0; $i<4; $i++){
	echo "<tr>\n";
	for($j=0; $j<4; $j++){
		$count=sprintf("%02d", $i*4+$j);
		echo "\t<td><a href=\"javascript:open_scr('$lang', '$count')\"><img src=\"/i/shot$count".'s.png" width="79" height="99" border="0" alt="" align="top"></a></td>'."\n";
	}
	echo "</tr>\n";
}
?>
</table></p>
