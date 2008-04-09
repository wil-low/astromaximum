<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Темы</title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<meta http-equiv="Cache-Control" content="max-age=30"/>
<link rel="stylesheet" type="text/css" href="style.css"/>
</head>
<body>
<div id="hdr" class="hdr"></div>
<div id="cont">
<?php
    include_once('dbconnect.php');
    $sth=mysql_query("SELECT DISTINCT year FROM locations ORDER BY year");
    $sess=session_name().'='.session_id();
    while($row=mysql_fetch_row($sth)){
            echo "<a href=\"geo.php?y=".$row[0]."&amp;$sess\">".$row[0]."</a><br/>";
    }
    mysql_free_result($sth);

?>
</div>
<div id="ftr"></div>
</body></html>