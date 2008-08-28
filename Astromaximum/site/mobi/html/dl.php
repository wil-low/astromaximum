<?php
if(!isset($EXEC)) die("Access restricted");
lang_load("mobi/html");
$step=1;
$max_cities=5;
$table_vsize=18;
$current_year=$GLOBALS['amax']['year'];
$chac=check_access();
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

<script type="text/javascript">
<!--	
function showc(country,state){
	frm=document.forms.namedItem("main");
	if(frm.elements.namedItem('cid').value!=country || frm.elements.namedItem('stateid').value!=state){
		frm.elements.namedItem('stateid').value=state;
		frm.elements.namedItem('cid').value=country;
		frm.submit();
	}
}
function generate(country){
	lst=findObj("chkcit");
	ind=lst.selectedIndex;
	if(ind<0){
		alert("<?php echo $i18['SELCITY_ALERT']?>");
		return;
	}
	if(confirm("<?php echo $i18['SELCITY_GENERATE']?>:\n"+lst.item(ind).text+", "+country+"?")){
		frm=document.forms.namedItem("main");
		frm.elements.namedItem("sc").value=lst.item(ind).value;
		frm.elements.namedItem("Action").value=1;
		frm.submit();
	}
}
function highlight_gen(lb){
	if(lb.selectedIndex<0) return;
	btn=findObj('genbtn');
	btn.style.background="url('i/btn_on.png')";
	btn.style.fontWeight="bold";
}
-->
</script>
<h4></h4>
<div style="position: absolute;top: 198px;left: 345px;width:660px;">
<form method="post" action="/?<?php echo $lang_ ?>&amp;p=dl" name="main">
<table class="colorlist">
<tr><th><b><?php echo "{$i18['STEP']} ".$step++ ?></b>.
<?php
// First listbox
	$cnum=0; $lb1='';
	if(isset($_POST['cid'])){
		$cnum=$_POST['cid'];
	}
	$sth=mysql_query("SELECT countries.id, countries.name FROM countries ORDER BY countries.name");
	while($row=mysql_fetch_row($sth)){
		if(!$cnum){
			$cnum=$row[0];
			$_POST['cid']=$cnum;
		}
		$selflag='';
		if($row[0]==$cnum){
			$cur_country=$row[1];
			$selflag=' selected="selected"';
		}
		$lb1.="<option value=\"".$row[0].'"'.$selflag.">".$row[1]."</option>\n";
	}
?>
<select name="y_sel" style="height:auto; width:auto;" onchange="document.forms.namedItem('main').submit()">
<?php
$y_now=date("Y");
for($i=0; $i<3; $i++){
	$yy=$y_now-$i;
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
	$stat=sprintf("SELECT DISTINCT states.id, states.name FROM states,".
		"countries WHERE country_id=%s ORDER BY states.name",quote_smart($cnum));
	$sth=mysql_query($stat);
	$cur_state=''; $lb2='';
	$allst=$i18['ALL_STATES'];
	$statenum=0;
	if(isset($_POST['stateid'])){
		$statenum=$_POST['stateid'];
	}
	$selflag='';
	if(!$statenum){
		$selflag=' selected="selected"';
	}
	$state_count=mysql_num_rows($sth);

	$lb2.="<option value=\"0\" $selflag>&gt;&gt;".$allst."&lt;&lt;</option>\n";
	while($row = mysql_fetch_row($sth)){
		$selflag='';
		if($row[0]==$statenum){
			$cur_state=$row[1];
			$selflag=' selected="selected"';
		}
		$lb2.="<option value=\"$row[0]\"$selflag>$row[1]</option>\n";
	}
	mysql_free_result($sth);
	$gen_prop=" onclick=\"generate('$cur_country')\"";
	$btnlbl=$i18['GET_DATA'];
	
	if(!$tries[1]){ // limit exceeded
		echo "\n<input type=\"hidden\" name=\"rmore\"/>";
		$gen_prop=" onclick=\"this.form.elements.namedItem('Action').value=1; this.form.submit()\"";
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
<select size="<?php echo $table_vsize ?>" onchange="showc(item(selectedIndex).value,0);" class="lb">
<?php echo $lb1 ?>
</select>
</td>
<td><!-- 2nd listbox -->
<?php
 echo "<select size=\"$table_vsize\" onchange=\"showc($cnum,item(selectedIndex).value);\" class=\"lb\">";
 echo $lb2
?>
</select>
</td>
<?php
	$andst='';
	if($statenum){
		$andst=sprintf(" AND state_id=%s",quote_smart($statenum));
	}
	$stat=sprintf(
		"SELECT cities.id, cities.name FROM cities,countries".
		",locations". # year condition
		" WHERE country_id=%s AND countries.id=country_id".
		" AND city_id=cities.id %s AND year=%s". # year condition
		" ORDER BY cities.name",quote_smart($cnum), $andst, quote_smart($defyear));
	$sth = mysql_query($stat);
?>
<td>
<select id="chkcit" size="<?php echo $table_vsize ?>" class="lb" onchange="highlight_gen(this)">
<?php
	while($row = mysql_fetch_row($sth)){
		echo "<option value=\"$row[0]\">$row[1]</option>\n";
	}
	mysql_free_result($sth);
	
?>
</select>
<input type="hidden" name="Action" value=""/>
<input type="hidden" name="cid" value=""/>
<input type="hidden" name="stateid" value="0"/>
<input type="hidden" name="sc" value="<?php echo $sc ?>"/>
</td>
</tr>
</table>
</form>
<p><?php echo sprintf($i18['DL_BEFORE_INSTALL'], anchor('buy'))?></p>
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
		$stat="SELECT cities.id, cities.name, countries.name FROM cities,countries WHERE cities.id IN ($sc1) and countries.id=country_id ORDER BY countries.name,cities.name";
		return mysql_query($stat);
	}
	return null;
}
?>