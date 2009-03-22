<?php
$defyear=2008;

if(isset($_GET['ajax'])){
    $ajax=$_GET['ajax'];
    $EXEC=1;
	include_once('../config.php');
    include_once("../lang.php");
    lang_load("../html");
    include_once("../dbconnect.php");
	header('Content-Type: text-javascript;charset=UTF-8');
	header('Cache-Control: no-cache');
	$cid =0; $stateid=0;
    if(isset($_GET['cid']))
    	$cid=intval($_GET['cid']);
	if(isset($_GET['stateid']))
    	 $stateid=intval($_GET['stateid']);
    if(isset($_GET['y']))
    	$defyear=intval($_GET['y']);
    $arr=array();
    if(!$ajax){
        $stat="SELECT countries.id, countries.name FROM countries ORDER BY countries.name";
    }
    
    if($ajax==1){
        array_push($arr, '[0,"'.$i18['ALL_STATES'].'"]');
        $stat=sprintf("SELECT DISTINCT states.id, states.name FROM states,".
            "countries WHERE country_id=%s ORDER BY states.name",quote_smart($cid));
    }
    
    if($ajax==2){
        $andst='';
        if($stateid){
            $andst=sprintf(" AND state_id=%s",quote_smart($stateid));
        }
        $stat=sprintf(
            "SELECT cities.id, cities.name FROM cities,countries,locations".
            " WHERE country_id=%s AND countries.id=country_id".
            " AND city_id=cities.id %s".
            " AND locations.city_id=cities.id AND year=%d".
            " ORDER BY cities.name",
            quote_smart($cid), $andst, quote_smart($defyear));
//        die($stat);
	}    
    $out='{"content":[';
	$sth=mysql_query($stat); $ii=0;
	while($row=mysql_fetch_row($sth)){
		array_push($arr, '['.$row[0].',"'.$row[1].'"]');
		$ii++;
	}
	$out.=implode(',', $arr);
	$out.=']}';
    echo $out;
    $fout=fopen("/tmp/1.txt","w");
    fwrite($fout, $out);
    fclose($fout);
    return;
}

if(!isset($EXEC)) die("Access restricted");
lang_load("mobi/html");
$META_CUSTOMSCR='/dl.js'; $META_CUSTOMFUNC='dl_init()';
$META_HEAD_ADD = <<< EOF
<script type="text/javascript" src="/jquery-1.2.6.min.js"></script>
EOF;
$step=1;
$max_cities=5;
$table_vsize=18;
$current_year=$GLOBALS['amax']['year'];
if($chac==-1){
	reg_warning($i18['PAGE_DLCIT']);
	return;
}

if($chac==3){ // unpaid
    show_payment_instructions(0);
    return;
}

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
if($chac==1){ // demo - previous year only
    $defyear=$current_year-1;
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
				$stat=sprintf("UPDATE customers SET city_limit=city_limit+1, ".
                    "dlcount1=$DLIM[1] WHERE id=%d", $_SESSION['uid']);
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
<form id="pwdrestore" action="dl" method="post">
<p>{$i18['CAPTCHA_PROMPT']}</p>
<p><img src="/mobi/kcaptcha?$param" alt="Captcha">
<input name="p_captcha" type="text" size="6"/>
</p>
<input name="Action" type="submit" value="OK" class="ok_on"/>
</form>			
KCAP;
		return;
		}
	}
	if(isset($_POST['sc'])){
		$sth=get_selected_cities('sc');
		if($is_allow_dl && mysql_num_rows($sth)>0){
			$row = mysql_fetch_row($sth);
			$str=midlet_create("geo", $defyear, $lang, $sc, "mobi/dl", 2);
			if(strlen($str)){
				dec_try_count(0, 1);
				echo "<p>".sprintf($i18['READY_CITIES'], "$row[1], $row[2]", $defyear)."</p>\n";
				echo "$str";
				if($tries[1]>=0){
					echo "<br/><br/>".tries_remained($tries[1]-1, $DLIM[1]);
				}
			}
			else{
				echo sprintf($i18['ERROR_CITYGEN'], "$row[1], $row[2]", $defyear);			
			}
			echo "<br/><br/><a href=\"dl\">{$i18['BACK']}</a>";
		}
		else{
			echo 'Вам не разрешено загружать города. Обратитесь в <a href="#">службу поддержки</a>.';
		}
		return;
	}
}
//print_r($_REQUEST);
?>
<script type="text/javascript">
<!--   
function generate(){
	lst=findObj("chkcit");
	ind=lst.selectedIndex;
	if(ind<0){
		alert("<?php echo $i18['SELCITY_ALERT']?>");
		return;
	}
	var lbc=document.main.countries;
	country=lbc.item(lbc.selectedIndex).text;
	if(confirm("<?php echo $i18['SELCITY_GENERATE']?>:\n"+lst.item(ind).text+", "+country+"?")){
		frm=document.forms.namedItem("main");
		frm.elements.namedItem("sc").value=lst.item(ind).value;
		frm.elements.namedItem("Action").value=1;
		frm.submit();
	}
}
//-->
</script>
<div>
<form method="post" action="dl" name="main">
<table class="colorlist">
<tr><th><b><?php echo "{$i18['STEP']} ".$step++ ?></b>.
<select name="y_sel" style="height:auto; width:auto;" 
	onchange="document.forms.namedItem('main').submit()">
<?php
$y_now=$current_year;
for($i=$y_now; $i>=$GLOBALS['amax']['min_demo_year']; $i--){
	$yy=$i;
    if($chac==1 and $yy!=$defyear) continue;
	echo "<option value=\"$yy\"";
	if($yy==$defyear) echo " selected=\"selected\"";
	echo ">$yy</option>\n";
}
?>
</select>
</th>
<th colspan="2">
<span style="white-space: nowrap; font-size:11px">
<?php
	if($tries[1]!=-1){ 
		echo sprintf($i18['LOAD_LEFT'], $DLIM[1]-$tries[1], $DLIM[1]);
	} 
?>
</span>
</th>
</tr>
<tr>
<th><b><?php echo "{$i18['STEP']} ".$step++ ?></b>. 
<?php
	echo $i18['H_COUNTRY'];
?>
</th>
<th><b><?php echo "{$i18['STEP']} ".$step++ ?></b>. 
<?php 
	echo $i18['H_STATE'];
	$gen_prop=" onclick=\"generate()\"";
	$btnlbl=$i18['GET_DATA'];
	
	if(!$tries[1]){ // limit exceeded
		echo "\n<input type=\"hidden\" id=\"rmore\" name=\"rmore\"/>";
		$gen_prop=" style=\"background:url('/i/btn_on.png'); font-weight:bold;\" ".
            "onclick=\"this.form.elements.namedItem('Action').value=1; this.form.submit()\"";
		$btnlbl=$i18['REQUEST_MORE'];
	}
?>
</th>
<th>
<span class="bums">
<input id="genbtn" type="button" value="<?php echo "$btnlbl\"$gen_prop"?>/>
</span> 
</th>
</tr>
<tr>
<td><!-- 1st listbox -->
<select id="countries" name="countries" size="18" onchange="showc(1,this)" class="lb">
</select>
</td>
<td><!-- 2nd listbox -->
<select name="states" size="18" onchange="showc(2,this)" class="lb">
</select>
</td>
<td><!-- 3rd listbox -->
<select name="cities" id="chkcit" size="18" class="lb" onchange="highlight_gen(true)">
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
<p><?php echo sprintf($i18['DL_BEFORE_INSTALL'], anchor( ($chac==1) ? 'demo':'buy' ))?></p>
</div>
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
		$stat="SELECT cities.id, cities.name, countries.name FROM cities,countries ".
            "WHERE cities.id IN ($sc1) and countries.id=country_id ".
            "ORDER BY countries.name,cities.name";
		return mysql_query($stat);
	}
	return null;
}
?>