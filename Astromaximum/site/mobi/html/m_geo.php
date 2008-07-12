<?php
	if(!isset($EXEC)) die("Access restricted");
	$level=0;
	if(isset($_GET['lvl'])){
		$level=$_GET['lvl'];
	}
	for($i=0; $i<=$level; $i++){
		$parm[$i]=0;
		if(isset($_GET["p$i"])){
			$parm[$i]=$_GET["p$i"];
		}
	}
	if($level==10 and $parm[0]>=0 and $parm[0]<count($GLOBALS['amax']['demo_cities'])){
		$arr=explode(',', get_default_cities($GLOBALS['amax']['demo_cities']));
		if(isset($arr[$parm[0]])){
			$sc=$arr[$parm[0]];
			$defyear=$GLOBALS['amax']['year']-1;
			$stat="SELECT cities.name, countries.name FROM cities,countries WHERE cities.id=$sc and countries.id=country_id ORDER BY countries.name,cities.name";
			$sth=mysql_query($stat);
			if(mysql_num_rows($sth)>0){
				$row = mysql_fetch_row($sth);
				echo sprintf($i18['READY_CITIES'], "$row[0], $row[1]", $defyear)."<br/>\n";
				echo midlet_create("geo", $defyear, $lang, $sc, "dl", true);
			}
		}
		return;
	}
	if(!$user_ok){
		redirect("/");
	}
	$fd = fopen("continents.txt", 'r');
	while (!feof($fd)) {
		$buffer = fgets($fd, 4096);
		if(strpos($buffer, "\t")){
			list($key,$value)=explode("\t",$buffer);
			$cont[trim($value)]=$key;
		}
	}
	fclose($fd);
	$LVL_MAX=3;
	$defyear=$GLOBALS['amax']['year'];
	if(isset($_GET['y'])){
		$defyear=$_GET['y'];
	}
	if($level==4){
		$is_allow_dl=1;
		$tries=get_try_count(0);
		make_city($parm[3], $defyear);
		$sc=$parm[3];
		$stat="SELECT cities.name, countries.name FROM cities,countries WHERE cities.id=$sc and countries.id=country_id ORDER BY countries.name,cities.name";
		$sth=mysql_query($stat);
		if($is_allow_dl && mysql_num_rows($sth)>0){
			$row = mysql_fetch_row($sth);
			echo sprintf($i18['READY_CITIES'], "$row[0], $row[1]", $defyear)."<br/>\n";
			$str=midlet_create("geo", $defyear, $lang, $sc, "dl", true);
			if(strlen($str)){
				dec_try_count(0, 1);
				echo $str;
				echo tries_remained($tries[1]-1, $DLIM[1]);
			}
		}
		else{
			echo 'Вам не разрешено загружать города.';
		}
		return;
	}
	$entity='';
	if(isset($_GET['ent'])){
		$entity=urldecode($_GET['ent']);
	}
	$_POST["n$level"]=$entity;
	$lvl_title=array('Continent', 'Country', 'State', 'City');
	$res=db_query($level, $parm);
	for($i=0; $i<$level; $i++){
		echo make_anchor($i, $parm, $lvl_title[$i], $defyear).'&nbsp;';
	}
	if($entity){
		echo "$entity:<br/>";
	}
	$cnt=count($res[0]);
	for($i=0; $i<$cnt; $i++){
		$newparam=$parm;
		$newparam[$level]=$res[0][$i];
		echo make_anchor($level+1, $newparam, $res[1][$i], $defyear)."<br/>\n";	
	}
	
	function db_query($level, $parmarams)
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
			$query=sprintf("SELECT * from countries where continent=%s ORDER BY name", quote_smart($parmarams[0]));
	//			echo "$query<br/>";
			$sth=mysql_query($query);
			while($row = mysql_fetch_row($sth)){
				$keys[$i]=$row[0]; $values[$i]=$row[1];
				$i++;
			}
		}
		if($level==2){ // state
			$query=sprintf("SELECT DISTINCT states.id, states.name FROM states,countries WHERE ".
				"country_id=%s ORDER BY states.name",quote_smart($parmarams[1]));
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
			if($parmarams[2]){
				$andst=sprintf(" AND state_id=%s",quote_smart($parmarams[2]));
			}
			$query=sprintf(
				"SELECT cities.id, cities.name FROM cities,countries".
				",locations WHERE country_id=%s AND countries.id=country_id".
				" AND city_id=cities.id %s AND year=%s". # year condition
				" ORDER BY cities.name",quote_smart($parmarams[1]), $andst, quote_smart($defyear));
	//			echo "$query<br/>";
			$sth=mysql_query($query);
			while($row = mysql_fetch_row($sth)){
				$keys[$i]=$row[0]; $values[$i]=$row[1];
				$i++;
			}
		}
		return array($keys, $values);
	}
	
	function make_anchor($level, $parmarams, $text, $year)
	{
	  global $sess, $lang_;
		$str="<a href=\"?$lang_&amp;p=geo&amp;lvl=$level&amp;y=$year&amp;$sess&amp;ent=".urlencode($text);
		for($j=0; $j<$level; $j++){
			$str.="&amp;p$j=$parmarams[$j]";
		}
		return $str."\">$text</a>&nbsp;";
	}
	
	function make_city($id, $country, $defyear){
		echo "<p>".sprintf($i18['READY_CITIES'], "$row[1], $row[2]", $defyear)."</p>\n";
		$str=midlet_create("geo", $defyear, $lang, $sc, "mobi/dl", true);
		if(strlen($str)){
			dec_try_count(0, 1);
			echo $str;
			echo tries_remained($tries[1]-1, $DLIM[1]);
		}
	}
/*	
	function make_city($id, $defyear){
		global $DIR_FILES, $DIR_SOURCE;
		$dsrc="./$DIR_FILES";
		$ye=substr($defyear,-2);
		list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
		$srcdir="$dsrc/$fn";
	#	echo "$dsrc/$destfile";
		$cmd=find_perl()." ./dl/gen_amax.cgi geo- $defyear EN $id $dsrc/$fn.r nomessjar";
		$ret=0;
		exec($cmd, $outp, $ret);
		if($ret){				
			echo "$cmd: $ret";
			echo implode('<br/>',$outp);
			exit;
		}
		else{
			if(!add_file($fn, "geo")){
				echo mysql_error();
				exit;
			}
			
			$data_php=dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
			if(!strpos($data_php, "mobi")){
				$data_php.="/mobi";
			}
			echo "<a href=\"http://$data_php/data.php?t=$fn\">Скачать</a>";
		}
	}
*/
?>