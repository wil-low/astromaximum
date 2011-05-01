<?php
	if(!isset($EXEC)) die("Access restricted");
	$level=0;
	$ITEMS_PER_PAGE = 10;
	if(isset($_GET['lvl'])){
		$level=$_GET['lvl'];
	}
	for($i=0; $i<=$level; $i++){
		$parm[$i]=0;
		if(isset($_GET["p$i"])){
			$parm[$i]=$_GET["p$i"];
		}
	}
	$offset = 0;
	if(isset($_GET['ofs'])){
		$offset=$_GET['ofs'];
	}

	$defyear=$GLOBALS['amax']['year'];
	if(isset($_GET['y'])){
		$defyear=$_GET['y'];
	}
    
    if($chac==-1){
        $defyear=$GLOBALS['amax']['year']-1;
    }
    
	if($level==3){
		$tries=($chac==1)? array(0, 100, 0): get_try_count(0);
		$is_allow_dl=$tries[1]!=0;
		$sc=$parm[2];
		$stat="SELECT cities.name, countries.name FROM cities,countries ".
            "WHERE cities.id=$sc and countries.id=country_id ".
            "ORDER BY countries.name,cities.name";
		$sth=mysql_query($stat);
		if($is_allow_dl && mysql_num_rows($sth)>0){
			$row = mysql_fetch_row($sth);
			echo sprintf('<i>%s</i> (%d)', "$row[0], $row[1]", $defyear)."<br/>\n";
			$str=midlet_create("geo", $defyear, $lang, $sc, "dl", 0);
			if(strlen($str)){
				$data_php=$_SERVER['SERVER_NAME'];
				if(strpos($data_php, "mobi") === false){
					$data_php.="/mobi";
				}
				dec_try_count(0, 1);
				echo "<a href=\"http://$data_php/data.php?t=$str\">{$i18['PHONE_DL']}</a>";
				echo tries_remained($tries[1]-1, $DLIM[1]);
				echo "<br/><span class=\"alert\">{$i18['VALID_LINKS']}</span>";
			}
		}
		else{
			echo 'City download limit exceeded';
		}
		return;
	}
	$entity='';
	if(isset($_GET['ent'])){
		$entity=urldecode($_GET['ent']);
		$entity=preg_replace('/[<>]/i', '', $entity);
	}
	$_POST["n$level"]=$entity;
	$lvl_title=array('Continent', 'Country', 'State', 'City');
	$res=db_query($level, $parm, $ITEMS_PER_PAGE, $offset * $ITEMS_PER_PAGE);
	$cnt=count($res[0]);
	$total=$res[2];

	$page_count = intval($total / $ITEMS_PER_PAGE);
	if ($page_count * $ITEMS_PER_PAGE < $total)
		++$page_count;
	$offset = pageNormalize ($offset, $page_count);
	$page_of_total = "Page 1 of 1";
	if ($page_count > 1) {
		$prev_page = pageNormalize ($offset - 1, $page_count);
		$next_page = pageNormalize ($offset + 1, $page_count);
		$page_of_total = make_anchor($level, $parm, "&lt;&lt;", $defyear, $prev_page, $entity);
		$page_of_total .= "Page " . ($offset + 1) . " of $page_count ";
		$page_of_total .= make_anchor($level, $parm, "&gt;&gt;", $defyear, $next_page, $entity);
	}
	echo "<p>$page_of_total</p>\n";
	echo "<p>";
	for($i=0; $i<$cnt; $i++){
		$newparam=$parm;
		$newparam[$level]=$res[0][$i];
		echo make_anchor($level+1, $newparam, $res[1][$i], $defyear, 0, $res[1][$i]);
		if ($i < $cnt - 1)
			echo "<br/>\n";
	}
	echo "</p>\n";

	function db_query($level, $parmarams, $limit, $offset)
	{
		global $cont, $level, $defyear, $subtitle, $entity;
		$total = 0;
        $year_='&amp;y='.$defyear;
		$i=0; $keys=$values=array();
		$limit_offset = sprintf (" LIMIT %s OFFSET %s", quote_smart($limit), quote_smart($offset));
		if($level==0){ // country
			$query=sprintf("SELECT * from countries ORDER BY name");
			$sth=mysql_query($query);
			$total = mysql_num_rows($sth);
			$query .= $limit_offset;
				//echo "$query<br/>";
			$sth=mysql_query($query);
			while($row = mysql_fetch_row($sth)){
				$keys[$i]=$row[0]; $values[$i]=$row[1];
				$i++;
			}
		}
		if($level==1){ // state
			$query=sprintf("SELECT DISTINCT states.id, states.name FROM states,countries WHERE ".
				"country_id=%s ORDER BY states.name",quote_smart($parmarams[0]));
			$sth=mysql_query($query);
			$total = mysql_num_rows($sth);
			$query .= $limit_offset;
	//			echo "$query<br/>";
			$sth=mysql_query($query);
			if(mysql_num_rows($sth)){
				while($row = mysql_fetch_row($sth)){
					$keys[$i]=$row[0]; $values[$i]=$row[1];
					$i++;
				}
			}
			else{
                addLevelNavItem('');
				$level++;
			}
		}
		if($level==2){ // city
			$andst='';
			if($parmarams[1]){
				$andst=sprintf(" AND state_id=%s",quote_smart($parmarams[1]));
			}
			$query=sprintf(
				"SELECT cities.id, cities.name FROM cities,countries".
				",locations WHERE country_id=%s AND countries.id=country_id".
				" AND city_id=cities.id %s AND year=%s". # year condition
				" ORDER BY cities.name",quote_smart($parmarams[0]), $andst, quote_smart($defyear));
			$sth=mysql_query($query);
			$total = mysql_num_rows($sth);
			$query .= $limit_offset;
				//echo "$query<br/>";
			$sth=mysql_query($query);
			while($row = mysql_fetch_row($sth)){
				$keys[$i]=$row[0]; $values[$i]=$row[1];
				$i++;
			}
		}
        addLevelNavItem($entity);
		return array($keys, $values, $total);
	}
	
	function make_anchor($level, $params, $text, $year, $offset, $entity)
	{
		global $sess, $lang_;
		$str="<a href=\"?$lang_&amp;p=geo&amp;lvl=$level&amp;y=$year&amp;$sess&amp;ent=".
			urlencode($entity)."&amp;ofs=$offset";
		for($j=0; $j<$level; $j++){
			$str.="&amp;p$j=$params[$j]";
		}
		return $str."\">$text</a> ";
	}

    function addLevelNavItem($text){
        global $level, $defyear, $parm;
		$str="geo&amp;lvl=$level&amp;y=$defyear&amp;ent=".urlencode($text);
		for($j=0; $j<$level; $j++){
			$str.="&amp;p$j=$parm[$j]";
		}
        addNavItem($str, $text, $level+3);
    }
	
	function pageNormalize ($pagenum, $max_pagenum) {
		if ($pagenum < 0)
			$pagenum = $max_pagenum - 1;
		else if ($pagenum > $max_pagenum - 1)
			$pagenum = 0;
		return $pagenum;
	}
?>
