<?php
$defyear=2008;

if(isset($_GET['ajax'])){
    $ajax=$_GET['ajax'];
    $EXEC=1;
	include_once('../config.php');
    include_once("../lang.php");
    lang_load("../html");
    include_once("../dbconnect.php");
    if(!isset($_GET['cid']) || !isset($_GET['stateid'])) echo "dfgsregsr";
    $cid=intval($_GET['cid']); $stateid=intval($_GET['stateid']);
    $arr=array();
    if(!$ajax){
        $stat="SELECT countries.id, countries.name FROM countries ORDER BY countries.name";
    }
    
    if($ajax==1){
        array_push($arr, "0=".$i18['ALL_STATES']);
        $stat=sprintf("SELECT DISTINCT states.id, states.name FROM states,".
            "countries WHERE country_id=%s ORDER BY states.name",quote_smart($cid));
    }
    
    if($ajax==2){
        $andst='';
        if($stateid){
            $andst=sprintf(" AND state_id=%s",quote_smart($stateid));
        }
        $stat=sprintf(
            "SELECT cities.id, cities.name FROM cities,countries".
            ",locations". # year condition
            " WHERE country_id=%s AND countries.id=country_id".
            " AND city_id=cities.id %s AND year=%s". # year condition
            " ORDER BY cities.name",quote_smart($cid), $andst, quote_smart($defyear));
    }    
    $out="";
	header('Cache-Control: no-cache');
	$sth=mysql_query($stat); $ii=0;
	while($row=mysql_fetch_row($sth)){
		array_push($arr, $row[0].'='.$row[1]);
		$ii++;
	}
	$out.=implode("\t", $arr);
    echo $out;
    $fout=fopen("/tmp/1.txt","w");
    fwrite($fout, $out);
    fclose($fout);
    return;
}

if(!isset($EXEC)) die("Access restricted");
lang_load("mobi/html");
$step=1;
$max_cities=5;
$table_vsize=18;
$current_year=$GLOBALS['amax']['year'];
$chac=0;//check_access();
if($chac==-1 or $chac==1){
	reg_warning($i18['PAGE_DLCIT']);
	return;
}
include_once('mobi/amtools.php');
$tries=get_try_count(0);
global $DLIM;
$is_allow_dl=($tries[1]!=0);
$defyear=$current_year;
if(isset($_POST['y_sel'])){
	$defyear=$_POST['y_sel'];
	if($defyear>$current_year){
		$defyear=$current_year;
	}
}
$sc=',';
if(isset($_POST['sc'])){
	$sc=$_POST['sc'];
}
$act=0;
if(isset($_POST['Action'])){
	$act=$_POST['Action'];
}
if(strlen($act)){
	if(!$tries[1]){
		if(isset($_POST['p_captcha'])){ // check captcha
			if(is_captcha($_POST['p_captcha'])){
				$stat=sprintf("UPDATE customers SET city_limit=city_limit+1, dlcount1=$DLIM[1] WHERE id=%d",
					$_SESSION['uid']);
				if(mysql_query($stat)){
					$tries=get_try_count(0);
					$is_allow_dl=($tries[1]!=0);
				}
				else{
					echo "Error:".mysql_error();
				}
			}
			else{
				echo <<<KCAP1
<h4>{$i18['REQUEST_MORE_H']}</h4>
{$i18['CAPTCHA_WRONG']}<br/><a href="{$_SERVER['REQUEST_URI']}">{$i18['BACK']}</a>
KCAP1;
				return;
			}
		}
		if(isset($_POST['rmore'])){ // requesting more cities
			$param=session_name().'='.session_id();
			$desc=sprintf($i18['REQUEST_MORE_DESC'], $DLIM[1]);
			echo <<< KCAP
<h4>{$i18['REQUEST_MORE_H']}</h4>			
<p>$desc</p>
<form id="pwdrestore" action="{$_SERVER['REQUEST_URI']}" method="post">
<p>{$i18['CAPTCHA_PROMPT']}</p>
<p><img src="mobi/kcaptcha?$param" alt="Captcha">
<input name="p_captcha" type="text"/>
</p>
<input name="Action" type="submit" value="OK"/>
</form>			
KCAP;
		return;
		}
	}
	if(isset($_POST['sc'])){
		$sth=get_selected_cities('sc');
		if($is_allow_dl && mysql_num_rows($sth)>0){
			$row = mysql_fetch_row($sth);
			echo "<p>".sprintf($i18['READY_CITIES'], "$row[1], $row[2]", $defyear)."</p>\n";
			$str=midlet_create("geo", $defyear, $lang, $sc, "mobi/dl", true);
			if(strlen($str)){
				dec_try_count(0, 1);
				echo "$str";
				if($tries[1]>=0){
					echo "<br/><br/>".tries_remained($tries[1]-1, $DLIM[1]);
				}
				echo "<br/><br/><a href=\"{$_SERVER['REQUEST_URI']}\">{$i18['BACK']}</a>";
			}
		}
		else{
			echo 'Вам не разрешено загружать города. Обратитесь в <a href="#">службу поддержки</a>.';
		}
		return;
	}
}
//print_r($_REQUEST);
?>

<h4></h4>
          
<div style="position: absolute;top: 198px;left: 345px;width:660px;">
<form method="post" action="/?lang=ru&amp;p=dl" name="main">
<table class="colorlist">
<tr><th><b>Шаг 1</b>.
<select name="y_sel" style="height:auto; width:auto;" onchange="document.forms.namedItem('main').submit()">
<option value="2008" selected="selected">2008</option>
<option value="2007">2007</option>
<option value="2006">2006</option>
</select>
</th>
<th colspan="2">
<span style="white-space: nowrap; font-size:11px">
</span>
</th>
</tr>
<tr>
<th><b>Шаг 2</b>. 
Страна</th>
<th><b>Шаг 3</b>. 
Регион</th>
<th>
<span class="bums">
<input id="genbtn" type="button" value="генерировать" onclick="generate('Armenia')"/>
</span> 
</th>
</tr>
<tr>
<td><!-- 1st listbox -->
<select name="countries" size="18" onchange="showc2(1,item(selectedIndex).value);" class="lb">
</select>
</td>
<td><!-- 2nd listbox -->
<select name="states" size="18" onchange="showc2(2,item(selectedIndex).value);" class="lb">
</select>
</td>
<td>
<select name="cities" id="chkcit" size="18" class="lb" onchange="highlight_gen(this)">
</select>
<input type="hidden" name="Action" value=""/>
<input type="hidden" name="cid" value="0"/>
<input type="hidden" name="stateid" value="0"/>
<input type="hidden" name="sc" value=","/>
<input type="hidden" name="lang" value="<?php echo $lang ?>"/>
</td>
</tr>
</table>
</form>
<div id="status">---</div>
</div>
<script src="/json2.js" type="text/javascript"></script>            
<script src="/dl.js" type="text/javascript"></script>            

<?php
function get_selected_cities($param)
{
	global $sc;
	$sc=',';
	if(isset($_POST[$param])){
		$sc=$_POST[$param];
	}
	$sc1=trim($sc,",");
	if($sc1){
		$stat="SELECT cities.id, cities.name, countries.name FROM cities,countries WHERE cities.id IN ($sc1) and countries.id=country_id ORDER BY countries.name,cities.name";
		return mysql_query($stat);
	}
	return null;
}
?>