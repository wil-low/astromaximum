<?php
	include_once('dbconnect.php');
	$fd = fopen("continents.txt", 'r');
	while (!feof($fd)) {
		$buffer = fgets($fd, 4096);
		list($key,$value)=explode("\t",$buffer);
		$cont[trim($value)]=$key;
	}
	fclose($fd);
	$LVL_MAX=3;
	$level=0;
	if(isset($_GET['lvl'])){
		$level=$_GET['lvl'];
	}
	$defyear=date('Y');
	if(isset($_GET['y'])){
		$defyear=$_GET['y'];
	}
	for($i=0; $i<=$level; $i++){
		$p[$i]=0;
		if(isset($_GET["p$i"])){
			$p[$i]=$_GET["p$i"];
		}
	}
	if($level==4){
		include_once('amtools.php');
		global $DIR_FILES, $DIR_SOURCE;
		$dsrc="./$DIR_FILES";
		$ye=substr($defyear,-2);
		list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
		$srcdir="$dsrc/$fn";
	#	echo "$dsrc/$destfile";
		$cmd="./dl/gen_amax.cgi geo- $defyear EN $p[3] $dsrc/$fn.r nomessjar";
		$ret=0;
		exec($cmd, $outp, $ret);
		if($ret){				
			echo "$cmd: $ret";
			echo implode('<br/>',$outp);
		}
		else{
			$data_php=dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
			if(!strpos($data_php, "mobi")){
				$data_php.="/mobi";
			}
			header("Location: http://$data_php/data.php?t=$fn");
			exit();
		}
	}
	$entity='';
	if(isset($_GET['ent'])){
		$entity=urldecode($_GET['ent']);
	}
	$_POST["n$level"]=$entity;
	$lvl_title=array('Continent', 'Country', 'State', 'City');
	echo "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN" "http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<?php
	$res=db_query($level, $p);
?>
<title><?php echo "Select $lvl_title[$level]" ?></title>
<meta http-equiv="content-type" content="application/xhtml+xml; charset=UTF-8"/>
<meta http-equiv="Cache-Control" content="max-age=0"/>
<link rel="stylesheet" type="text/css" href="style.css"/>
</head>
<body>
<div class="hdr">
<?php
	for($i=0; $i<$level; $i++){
		echo make_anchor($i, $p, $lvl_title[$i], $defyear).'&nbsp;';
	}
?>
</div>
<div id="cont">
<?php
	if($entity){
		echo "$entity:<br/>";
	}
	$cnt=count($res[0]);
	for($i=0; $i<$cnt; $i++){
		$newparam=$p;
		$newparam[$level]=$res[0][$i];
		echo make_anchor($level+1, $newparam, $res[1][$i], $defyear)."<br/>\n";	
	}
?>
</div>
<div id="ftr">
<pre>
<?php
//	print_r($_GET);
//	print_r($_POST);
?>
</pre>
</div>
</body>
</html>

<?php
	function db_query($level, $params)
	{
		global $cont, $level, $defyear;
		$i=0; $keys=$values=array();
		if($level==0){ // continent
			foreach ($cont as $value => $key) {
				$keys[$i]=$key; $values[$i]=$value;
				$i++;
			}
		}
		if($level==1){ // country
			$query=sprintf("SELECT * from countries where continent=%s ORDER BY name", quote_smart($params[0]));
//			echo "$query<br/>";
			$sth=mysql_query($query);
			while($row = mysql_fetch_row($sth)){
				$keys[$i]=$row[0]; $values[$i]=$row[1];
				$i++;
			}
		}
		if($level==2){ // state
			$query=sprintf("SELECT DISTINCT states.id, states.name FROM states,countries WHERE ".
				"country_id=%s ORDER BY states.name",quote_smart($params[1]));
//			echo "$query<br/>";
			$sth=mysql_query($query);
			if(mysql_num_rows($sth)){
				while($row = mysql_fetch_row($sth)){
					$keys[$i]=$row[0]; $values[$i]=$row[1];
					$i++;
				}
			}
			else{
				$level++;
			}
		}
		if($level==3){ // city
			$andst='';
			if($params[2]){
				$andst=sprintf(" AND state_id=%s",quote_smart($params[2]));
			}
			$query=sprintf(
				"SELECT cities.id, cities.name FROM cities,countries".
				",locations WHERE country_id=%s AND countries.id=country_id".
				" AND city_id=cities.id %s AND year=%s". # year condition
				" ORDER BY cities.name",quote_smart($params[1]), $andst, quote_smart($defyear));
//			echo "$query<br/>";
			$sth=mysql_query($query);
			while($row = mysql_fetch_row($sth)){
				$keys[$i]=$row[0]; $values[$i]=$row[1];
				$i++;
			}
		}
		return array($keys, $values);
	}
	
	function make_anchor($level, $params, $text, $year)
	{
		$str="<a href=\"".$_SERVER['PHP_SELF']."?lvl=$level&amp;y=$year&amp;ent=".urlencode($text);
		for($j=0; $j<$level; $j++){
			$str.="&amp;p$j=$params[$j]";
		}
		return $str."\">$text</a>&nbsp;";
	}
?>