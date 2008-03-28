<?php
$DEF_CITIES=array('m.Olympos');
$DEMO_CITY=array('London');
function find_perl(){
	$perl="/opt/lampp/bin/perl";
	if(!file_exists($perl)){
		$perl="/usr/bin/perl";
	}
	return $perl;
}

function amtools_random($ye,$path, $ext){
	do{
		$id=''; $flag=1;
		for($i=0; $i<12; $i++){
			$id.=(int)(rand(0,9));
		}
		$id=$ye.$id;
		$fn="$path/$id$ext";
		if($ext){
			if(is_file($fn)){
				$flag=0;
			}
		}
		else{
			if(is_dir($fn)){
				$flag=0;
			}
		}
	}while(!$flag);
	return array($fn, $id);
}

$UNZIP="unzip %s -d %s -x *META-INF* "; # > /dev/null
$UNTAR="tar xvf %s -C %s";
$ZIP="fastjar %s ";

function jar($jarpath, $out, $manifest, $srcdir)
{
	return sprintf("%s/fastjar cvfm %s %s -C %s .", $jarpath, $out, $manifest, $srcdir);
}

function join_datafiles2($year, $destfile, $a_data) # year, destfile, data_listref
{
	$outf=fopen($destfile,'wb');
	$count=0;
	$data='';
	$a_len=array();
	foreach ($a_data as $i => $value) {
    	$data.=$value;
    	$a_len[$count]=strlen($value);
		$count++;
 	}
	fwrite($outf, pack('n',$year));
	fwrite($outf, pack('n',$count));
	foreach ($a_len as $i => $value) {
		fwrite($outf, pack('n',$value));
	}
	fwrite($outf, $data);
	fclose($outf);
}

function rm_all($dir)
{
	foreach(glob("$dir/*.*") as $fname){
		unlink($fname);
	}
	rmdir($dir);
}

function select_cities($year, $ids, $destfile){
	$ids=trim($ids,',');
	$dir=dirname($destfile);
	if(!is_dir($dir)) mkdir($dir);
	$stat=sprintf("SELECT DISTINCT cities.name, data FROM cities, locations ".
		"WHERE cities.id IN (%s) AND city_id=cities.id AND year=%s".
		" ORDER BY cities.name",$ids,$year);
#	echo $stat;
	$sth = mysql_query($stat);
	$i=0;
	while($row = mysql_fetch_row($sth)){
		$data[$i++]=$row[1];		
		if(!isset($midlet_name)){
			$midlet_name=$row[0];
		}
	}
	mysql_free_result($sth);
#	echo $destfile;
	join_datafiles2($year, $destfile, $data);
	return $midlet_name;
}

function create_jar($year, $ids, $template_jar, $isdemo, $midlet_name, 
		$main_class, $common_file){
	global $DIR_FILES, $DIR_SOURCE, $UNZIP, $ZIP;
	$mypath=dirname(__FILE__);
	$dfil="$mypath/$DIR_FILES";
	$dsrc="$mypath/$DIR_SOURCE";
	$ye=substr($year,-2);
	list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
	$srcdir="$dsrc/$fn";
//	mkdir($srcdir);
//	echo $srcdir;
	$server="http://".$_SERVER['SERVER_NAME'];
	$locfile='locations.dat';
	if($isdemo) $locfile='l.dat';
	$midlet_name_supposed=select_cities($year, $ids, "$dsrc/$fn/$locfile");
#	echo "<p>$dsrc/$fn/$locfile";
#	exit;
	$infile=fopen("$dsrc/template.jad","rb");
	$template = fread($infile, 1000000);
	fclose($infile);
	if(strcmp($midlet_name,'')){
		$fname="$midlet_name_'$ye";
	}
	else{
		$code="-".substr($fn,-4);
		$fname="Cities'$ye$code";
	}
	$template=str_replace('<NAME>', $fname, $template);
#	$jad=~s/<DESC>/$desc/isg;
	$template=str_replace('<JAR>', "$fname.jar", $template);
	$template=str_replace('<MAINCLASS>', $main_class, $template);
	$cmd=sprintf($UNZIP, $template_jar, "$dsrc/$fn");
	exec($cmd);
	$inf=fopen("/tmp/$fn.MF", 'wb');
	fwrite($inf, $template);
	fclose($inf);
	if(is_file($common_file)){
		$infile=fopen($common_file,"rb");
		$cfile = fread($infile, 1000000);
		fclose($infile);
		$comfile='common.dat';
		if($isdemo) $comfile='c.dat';
		$inf=fopen("$dsrc/$fn/$comfile", 'wb');
		fwrite($inf, $cfile);
		fclose($inf);
	}
	$inf=fopen("$dsrc/icons/".substr($year,-1).".png", 'rb');
	$icon=fread($inf,5000);
	fclose($inf);
	$inf=fopen("$dsrc/$fn/icon.png", 'wb');
	fwrite($inf,$icon);
	fclose($inf);
	$cmd=jar($mypath, "$dfil/$fn.r", "/tmp/$fn.MF", "$dsrc/$fn");
//	echo $cmd;
	exec($cmd);
//	usleep(500000);
//	emit_nav2();
//	exit();
	$inf=fopen("$dfil/$fn.d", 'wb');
	$asize= filesize("$dfil/$fn.r");
	$template.="MIDlet-Jar-Size: $asize\n";
	fwrite($inf, $template);
	fclose($inf);
	$server=$_SERVER['SERVER_NAME'];
	$template=preg_replace('/(MIDlet-Jar-URL: ).+?\n/is',"$1http://$server/mobi/data.php?r=$fn\n", $template);
	$inf=fopen("$dfil/$fn.t", 'wb');
	fwrite($inf, $template);
	fclose($inf);
	exec("rm -R $dsrc/$fn");
	unlink("/tmp/$fn.MF");
/*	
	my $sql='INSERT INTO files (id, type, user_id, end_tm) VALUES';
	foreach (('r','d','t')){
		$sql.=" ($fn, \'$_\', ".$userid.", NOW()+ INTERVAL 2 HOUR),";
	}
	$sql=~s/,$//is;
	$sth = $dbh->prepare($sql)|| die $dbh->errstr;
	$sth->execute|| die $dbh->errstr;
	$sth->finish;
*/	
	return $fn;
}

function is_mobile(){
    return 1;
// Lightweight device detection http://dev.mobi/node/472
    $mobile_browser = '0';

    if(preg_match('/(up.browser|up.link|mmp|symbian|smartphone|midp|wap|phone)/i',
        strtolower($_SERVER['HTTP_USER_AGENT']))){
        $mobile_browser++;
        }

    if((strpos(strtolower($_SERVER['HTTP_ACCEPT']),'application/vnd.wap.xhtml+xml')>0) or 
        ((isset($_SERVER['HTTP_X_WAP_PROFILE']) or isset($_SERVER['HTTP_PROFILE'])))){
        $mobile_browser++;
        }

    $mobile_ua = strtolower(substr($_SERVER['HTTP_USER_AGENT'],0,4));
    $mobile_agents = array(
        'w3c ','acs-','alav','alca','amoi','audi','avan','benq','bird','blac',
        'blaz','brew','cell','cldc','cmd-','dang','doco','eric','hipt','inno',
        'ipaq','java','jigs','kddi','keji','leno','lg-c','lg-d','lg-g','lge-',
        'maui','maxo','midp','mits','mmef','mobi','mot-','moto','mwbp','nec-',
        'newt','noki','oper','palm','pana','pant','phil','play','port','prox',
        'qwap','sage','sams','sany','sch-','sec-','send','seri','sgh-','shar',
        'sie-','siem','smal','smar','sony','sph-','symb','t-mo','teli','tim-',
        'tosh','tsm-','upg1','upsi','vk-v','voda','wap-','wapa','wapi','wapp',
        'wapr','webc','winw','winw','xda','xda-');

    if(in_array($mobile_ua,$mobile_agents)){
        $mobile_browser++;
        }
    if (strpos(strtolower($_SERVER['ALL_HTTP']),'OperaMini')>0) {
        $mobile_browser++;
        }
    if (strpos(strtolower($_SERVER['HTTP_USER_AGENT']),'windows')>0) {
        $mobile_browser=0;
    }
    if (strpos(strtolower($_SERVER['HTTP_USER_AGENT']),'iemobile')>0) {
                    $mobile_browser++;
    }
    return $mobile_browser;
}

function get_default_cities($arr){
	$sth=mysql_query("SELECT id FROM cities WHERE name in ('".implode("','", $arr)."')");
	$ids='';
	while($row=mysql_fetch_row($sth)){
		$ids.="$row[0],";
	}
	mysql_free_result($sth);
	return substr($ids, 0, -1);
}

function midlet_create($type, $year, $lang, $param, $path2gen){ // out - string with links
	global $DIR_FILES, $DIR_SOURCE, $i18;

	$timeout_offset=-24;
	$timeout_mins=2880;

	$str='';
	$perl=find_perl();
	$dsrc="mobi/$DIR_FILES";
	$ye=substr($year,-2);
	list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
	$srcdir="$dsrc/$fn";
	$is_cal=true;
#	echo "$dsrc/$destfile";
	if(strcmp($type, "geo")==0){
		$cmd="$perl $path2gen/gen_amax.cgi geo- $year $lang $param $dsrc/$fn.r nomessjar";
	}
	else{
		$is_cal=true;
		if(!preg_match("/^(demo|tb)$/is", $type)){
			return;
		}
		$cmd="$perl $path2gen/gen_amax.cgi $type $year $lang \"$param\" $dsrc/$fn.r $timeout_offset $timeout_mins nomessjar";
	}
	$ret=0;
	exec($cmd, $outp, $ret);
	if($ret){				
		$str.=$cmd.'<br/>';
		$str.=implode('<br/>',$outp);
	}
	else{
//		if(strcmp($type, "demo")){
//		$data_php=dirname(dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']));
//		header("Location: http://$data_php/data.php?d=$fn");
			if(!add_file($fn, $type{0}." $year $lang")){
				$str.=mysql_error();
				return $str;
			}
//		}
		$id=$fn;
		$data_php="http://".dirname($_SERVER['SERVER_NAME'].$_SERVER['SCRIPT_NAME']);
		if(!strpos($data_php, "mobi")){
			$data_php.="/mobi";
		}
		$url=$data_php.'/data.php?r='.$id;
		$str.="<h4>{$i18['PC_DL']}:</h4>";
		$str.="<a href='$url'>JAR</a>";
		$url=str_replace("?r", "?d", $url);
		$str.=" <a href='$url'>JAD</a><br>";

#				$url=str_replace("?d", "?t", $url);
#				echo "<h4>{$i18['PHONE_DL']}:</h4>";
#				echo "<a href='$url'>JAD</a><br>";
		
		$str.="<br><font color='red'>{$i18['VALID_LINKS']}</font><br><br>";
//		$str.="<a href={$_SERVER['REQUEST_URI']}>{$i18['BACK']}</a>";
	}
	return $str;
}

function record_in_range($table, $tm){
	$stat=sprintf("SELECT * FROM %s WHERE %s BETWEEN start AND end LIMIT 1", $table, quote_smart($tm));
	$sth=mysql_query($stat);
	if(!$sth){
		echo mysql_error();
		return 0;
	}
	$res=mysql_num_rows($sth);
	if($res and strcmp($table, "_sundgr")==0){
		$res=mysql_fetch_row($sth);
		$res=$res[2];
	}
//	echo "$stat\n";
	mysql_free_result($sth);	
	return $res;
}


function pwd_send($to, $login, $realname, $dl_count, $city_count, $pwd){
	$subject = 'Astromaximum.de - new password';
/*	$message = <<<EOF
<html><head>
  <title>Astromaximum.de - new password</title>
</head>
<body>
<p>Dear $realname,</p>

<p>You requested a new password for access to 
<a href="http://astromaximum.de/">http://astromaximum.de/</a> 
<br/>Your credentials are now as follows:</p>
<ul>
<li>login: $login</li>
<li>password:  $pwd</li>
</ul>	
<p>Number of Astromaximum copies to download: $dl_count</p>
<p>Number of cities to download: $city_count</p>

<p>This mail was generated automatically, there is no need to reply.</p>

<p>Thank you for using our service.</p>
<hr/>
EOF;

$rusmsg= <<<EOF1
<p>Уважаемый $realname,</p>

<p>Вы запросили новый пароль для доступа на сайт 
<a href="http://astromaximum.de/">http://astromaximum.de/</a>
<br/>Для входа на сайт наберите:</p>
<ul>
<li>логин:   $login</li>
<li>пароль:  $pwd</li>
</ul>
<p>Разрешено загрузить копий мидлета на текущий год: $dl_count</p>
<p>Разрешено загрузить городов: $city_count</p>

<p>Это письмо сгенерировано автоматически, нет нужды отвечать на него.</p>

<p>Спасибо за использование нашего сервиса.</p>
EOF1;
*/
	$message=file_get_contents("mobi/dl/source/pwdrestore.mail");
	$message=str_replace('<realname>', $realname, $message);
	$message=str_replace('<login>', $login, $message);
	$message=str_replace('<pwd>', $pwd, $message);
	$message=str_replace('<dl_count>', $dl_count, $message);
	$message=str_replace('<city_count>', $city_count, $message);
	$headers = 'From: robot@astromaximum.de' . "\r\n" .
	    'X-Mailer: PHP';
	return mail($to, $subject, $message, $headers);
}
?>
