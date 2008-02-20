<?php
lang_load("mobi/html");
$step=1;
$max_cities=5;
$table_vsize=18;
$current_year=get_year();
$chac=check_access();
if($chac==-1 or $chac==1){
	reg_warning("Загрузка городов");
	return;
}
$defyear=date("Y");
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
if(strlen($act) && isset($_POST['sc'])){
	$sth=get_selected_cities('sc');
	if(strlen($sth)>0){
		$row = mysql_fetch_row($sth);
		echo "$defyear!=$current_year";
		return;
		echo "<p>".sprintf($i18['READY_CITIES'], "$row[1], $row[2]", $defyear)."</p>\n";
		include_once('mobi/amtools.php');
		echo midlet_create("geo", $defyear, $lang, $sc, "mobi/dl");
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
		alert("Выберите город из списка");
		return;
	}
	if(confirm("Сгенерировать город:\n"+lst.item(ind).text+", "+country+"?")){
		frm=document.forms.namedItem("main");
		frm.elements.namedItem("sc").value=lst.item(ind).value;
		frm.elements.namedItem("Action").value=1;
		frm.submit();
	}
}
function highlight_gen(){
	btn=findObj('genbtn');
	btn.style.background="url('i/btn_on.jpg')";
	btn.style.color="rgb(0,0,0)";
}
-->
</script>
<h4></h4>
<div style="position: absolute;top: 198px;left: 345px;width:660px;">
<form method="post" action="/?<?php echo $lang ?>&amp;p=dl" name="main">
<table class="geo">
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
<th colspan="2" style="font-size:12px">
<span style="white-space: nowrap;"><?php echo sprintf($i18['LOAD_LEFT'], 5, $max_cities) ?></span>
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
?>
</th>
<th>
<span class="bums">
<input id="genbtn" type="button" onclick="generate('<?php echo $cur_country ?>')" value="<?php echo $i18['GET_DATA']?>"/>
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
<select id="chkcit" size="<?php echo $table_vsize ?>" class="lb" onchange="highlight_gen()">
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
<p>Перед загрузкой городов <?php echo anchor('buy') ?>календарь соответствующего года</a> должен быть установлен на Вашем телефоне.</p>
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