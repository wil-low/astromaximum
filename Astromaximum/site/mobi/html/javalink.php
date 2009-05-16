<?php
if(!isset($EXEC)) die("Access restricted");
if(!isset($_GET['id'])){
    javalink_gone();
    return;
}
$dig=$_GET['id'];
$stat=sprintf(
	"SELECT type FROM files WHERE id='%s' AND end_tm>NOW() AND NOT deleted", quote_smart($dig));
$sth=mysql_query($stat);
if(mysql_affected_rows() != 1){
    javalink_gone();
    return;
}
$lnk1 = "/mobi/data.php?d=$dig";
$lnk2 = "/mobi/data.php?r=$dig";
$jarsize=fsize_human("dl/files/$id.r");
$jadsize=fsize_human("dl/files/$id.d");

echo <<< EOF
Here are your links:<br/>
<a href="$lnk1">JAD ($jadsize)</a><br/>
<a href="$lnk2">JAR ($jarsize)</a><br/>
EOF;

function javalink_gone()
{
    echo "No file";
}
