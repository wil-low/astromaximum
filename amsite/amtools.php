<?php
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

$UNZIP="unzip %s -d %s -x *META-INF* > /dev/null";
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

function create_jar($year, $ids, $named_midlet){
	global $DIR_FILES, $DIR_SOURCE, $UNZIP, $ZIP;
	$mypath=dirname(__FILE__);
	$dfil="$mypath/$DIR_FILES";
	$dsrc="$mypath/$DIR_SOURCE";
	$ids=trim($ids,',');
	$ye=substr($year,-2);
	list($dir,$fn)=amtools_random($ye, $dsrc,'.r');
	$srcdir="/tmp/$fn";
	mkdir($srcdir);
	
	$server="http://".$_SERVER['SERVER_NAME'];
	$stat=sprintf("SELECT DISTINCT cities.name, data FROM cities, locations ".
		"WHERE cities.id IN (%s) AND city_id=cities.id AND year=%s".
		" ORDER BY cities.name",$ids,$year);
//	print $stat;
	$sth = mysql_query($stat);
	$i=0;
	$midlet_name='';
	while($row = mysql_fetch_row($sth)){
		$data[$i++]=$row[1];		
		if($named_midlet){
			$midlet_name=$row[0];
		}
	}
	mysql_free_result($sth);
	$infile=fopen("$dsrc/template.jad","rb");
	$template = fread($infile, 1000000);
	fclose($infile);
	if($named_midlet){
		$fname="$midlet_name'$ye";
	}
	else{
		$code="-".substr($fn,-4);
		$fname="Cities'$ye$code";
	}
	$template=str_replace('<NAME>', $fname, $template);
#	$jad=~s/<DESC>/$desc/isg;
	$template=str_replace('<JAR>', "$fname.jar", $template);
#	echo $template;

	$cmd=sprintf($UNZIP, "$dsrc/template.zip", "$dsrc/$fn");
	exec($cmd);
	$inf=fopen("/tmp/$fn.MF", 'wb');
	fwrite($inf, $template);
	fclose($inf);
	join_datafiles2($year, "$dsrc/$fn/locations.dat", $data);
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
	$template=preg_replace('/(MIDlet-Jar-URL: ).+?\n/is',"$1http://astromaximum.de/mobi/data.php?r=$fn\n", $template);
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

?>
