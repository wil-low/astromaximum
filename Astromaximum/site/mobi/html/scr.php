<?php 
if(!isset($EXEC)) die("Access restricted");
$META_TITLE='Скриншоты';
$META_KEYWORDS='';
$META_DESCR='';

$images=array();
$out='';
for($i=0; $i<4; $i++){
	$out.="<tr>\n";
	for($j=0; $j<4; $j++){
        $n=$i*4+$j+1;
        $count=sprintf('%02d', $n);
        $img='/i/shot'.$count.'s.png';
		$out.="\t<td><a href=\"javascript:open_scr('$lang','$count')\"><img src=\"".$img.'" width="79" height="99" alt=""/></a></td>'."\n";
        array_push($images, "\"$img\"");
	}
	$out.="</tr>\n";
}
$imglist=implode(',', $images);

$META_HEAD_ADD= <<< EOF
<script type="text/javascript">
<!--
var imgs=new Array();
function preloadImages(){
    for(var i=0; i<preloadImages.arguments.length; i++){
        imgs[i]=new Image();
        imgs[i].src=preloadImages.arguments[i];
    }
}
preloadImages({$imglist});
//-->
</script>
EOF;
?>
<table class="gallery">
<?php echo $out ?>
</table>
