<?php 
if(!isset($EXEC)) die("Access restricted");

$current_year=$GLOBALS['amax']['year'];
$uri=htmlentities($_SERVER['REQUEST_URI']);

$msg=$i18['REGFORM_REQF'];

$email=$email2=$nick=$paymode=$model='';

if($chac==3){ // unpaid
    show_payment_instructions(0);
    echo '<hr><a href="contacts/m=paid">'.$i18['CONTACTS_00'].'</a>';
    return;
}

function alert($str){
    global $i18;
    return '<span class="alert">'.$i18[$str].'</span>';
}

function present($key){
    return isset($_POST[$key]) && strlen(trim($_POST[$key]))>0;
}

if(present('email1') && present('email2') && present('nick') && present('p_captcha')
    && present('agree') && strcmp($_POST['agree'], 'on') == 0){ 

    $email=substr($_POST['email1'], 0, 50);
    $email2=substr($_POST['email2'], 0, 50);
    $realname=substr($_POST['nick'], 0, 50);

    do{    
        if(strcmp($email, $email2) != 0){
            $msg=alert('REGFE_EM_NOMATCH'); break;
        }
        if(!check_email_address($email)){
            $msg=alert('REGFE_EM_BAD'); break;
        }
//TODO: email duplicates prohibited???        
        $stat=sprintf("SELECT id FROM customers where email=%s LIMIT 1",
            quote_smart($email));
        $sth=mysql_query($stat);
        if(!$sth){
            echo mysql_error().": >$stat<";
        }
        if(mysql_num_rows($sth)==1){
            $msg=alert('REGFE_EM_EXISTS'); break;
        }
        if(!is_captcha($_POST['p_captcha'])){
            $msg=alert('CAPTCHA_WRONG'); break;
        }
// create new customer with key in place of password
        $passkey=$realname.$model.date("l dS of F Y h:i:s A").mt_rand();
        $passkey=pwd_convert2(pwd_convert1($email, $passkey));
        
        $stat=sprintf("INSERT INTO customers (realname,email,hash,subscr_date,".
            "paymode_id,active,dlcount0,dlcount1,dlcount2) values ".
            "(%s,%s,%s,NOW(),%d,0,0,0,0)",
            quote_smart($realname),
            quote_smart($email),
            quote_smart($passkey),
            2
        );
        if(!mysql_query($stat)){
            echo mysql_error(); break;
        }
        $mail=confirmation_send($email, $realname, $passkey);
        if($mail->ErrorInfo){
            echo 'Error: '.$mail->ErrorInfo;
        }
        else{
            echo $i18['INSTR_SENT'];
//            show_payment_instructions($paymode);
        }
        return;
        
    }while(0);
}

if($chac!=-1 and $chac!=1){
	$y_now=$current_year;
	$out='';
	for($i=$current_year-1; $i>=$GLOBALS['amax']['min_demo_year']; $i--){
		$out.="<option value=\"$i\"";
		if($i==$current_year) $out.=" selected=\"selected\"";
		$out.=">$i</option>\n";
	}
	$year=$current_year;
	
	$tries=get_try_count(0);
	
	if ($tries[0] == 0)
		show_payment_instructions(0);
	else {
		global $DLIM;
		$dl_key=0;
		$is_allow_dl=($tries[0]!=0);
		if(isset($_POST["yagree"])){ // past year request
			$year=(int)$_POST["yagree"];
			if($year>=$current_year){
				return;
			}
			$is_allow_dl=($tries[2]!=0);
			$dl_key=2;
		}
		echo "<h4>".sprintf($i18['DLOAD4YEAR'], $year)."</h4>";
		if(isset($_POST["agree"])){
			if($is_allow_dl){
				$sc=get_default_cities($GLOBALS['amax']['def_cities']); 
				echo tries_remained($tries[$dl_key]-1, $DLIM[$dl_key]);
				$str=midlet_create("tb", $year, $lang, $sc, "mobi/dl", 2);
				if(strlen($str)){
					dec_try_count(0, $dl_key);
					echo $str;
				}
			}
			else{
				echo sprintf($i18['NO_CALENDAR_DL'], '<a href="#">');
			}
			echo "<p><a href=\"$uri\">{$i18['BACK']}</a></p>";        
			return;
		}
		echo "<form action=\"$uri\" method=\"post\">\n";
		echo '<input type="hidden" name="demo" value="0"/>';
		$str=($tries[0]==1)? $i18['ONE_MORE_COPY']."<br/><br/>": '';
		echo dload_tries_prompt($tries, 0, $str, sprintf($i18['CONFIRM_TRIAL'], $lang));
		echo "</form>";
	}
	echo "<br/><br/><br/>\n";
	echo "<form action=\"$uri\" method=\"post\">\n";
    echo '<input type="hidden" name="demo" value="1"/>';
	echo "<h4>".sprintf($i18['DLOAD4YEAR'],"<select name=\"yagree\">$out</select>")."</h4>";
	echo dload_tries_prompt($tries, 2, '', $i18['GENERATE?']);
	echo "</form>";
    return;
}

include_once("mobi/ipblock.php");
//$msg=allow_ip('buy', false);
//echo $msg;
//if($msg) return;

$agree=dload_prompt(sprintf($i18['CONFIRM_TRIAL'], $lang), false);
$sess=session_name().'='.session_id();

// TODO: maintain these options when changing dic_paymode table!

$link['02'] = "http://$lang.wikipedia.com/wiki/PayPal";

$langwiki = strcmp($lang, 'ru') == 0 ? '' : "$lang:";
$link['04'] = "/wiki/doku.php/{$langwiki}bill";

$out='';
foreach($GLOBALS['amax']['paymodes'] as $key){
    $key2=sprintf('%02d', $key);
    $out.='<li>'.$i18['PAYMENT_'.$key2].
		' <a href="'.$link[$key2].'" target="_blank"><img src="/i/info.png" width="12" height="12" alt="?"/></a></li>';
}
$desc=sprintf($i18['REGFORM_DESC'], "<ul>$out</ul>");
echo <<< EOF
$desc
<form id="regform" action="$uri" method="post">
EOF;
/*
<p>{$i18['REGFORM_PAYMODE']}</p>
<p class="no_border">
foreach($GLOBALS['amax']['paymodes'] as $key){
    $key2=sprintf('%02d', $key);
    echo paymode_radio($i18['PAYMENT_'.$key2], 2, $key2, true);
}
*/
echo <<< EOF
</p>
<input type="hidden" name="demo"/>
<p>{$i18['REGFORM_EMAIL_1']}<br/>
<input type="text" name="email1" size="25" value="$email"/><br/>
{$i18['REGFORM_EMAIL_2']}<br/>
<input type="text" name="email2" size="25" value="$email2"/></p>
<p>{$i18['REGFORM_NAME']}<br/>
<input type="text" name="nick" size="25" value="$nick"/></p>
<p><img src="/mobi/kcaptcha?$sess" alt="Captcha"/>
<input type="text" name="p_captcha" size="8"/></p>
$agree
</form>
EOF;

return;

?>
<?php include_once('paypal.php') ?>
<?php include_once('bank.php') ?>
<!--
<p>Пожалуйста, заполните форму регистрации:
<form id="regform" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post">
<p><input type="text" name="fullname"> Полное имя*</p>
<p><input type="text" name="email"> Email* </p>
<input type="submit" name="reg_submit" value="Зарегистрироваться">  
</form>
-->
<?php

function dload_tries_prompt($arr, $key, $str, $prompt){
	global $DLIM;
	$disabled=false;
	$num=$arr[$key];
	$rem='';
	if($num!=-1){
		$rem=tries_remained($num, $DLIM[$key]);
	}
	if(!$num){
		$num='<span class="alert">'.$num."</span>";
		$disabled=true;
	}
	return $rem."<br/>".$str.dload_prompt($prompt, $disabled);
}

function paymode_radio($text, $paymode_id, $popup_id, $is_selected){
    global $lang_;
    $chk = $is_selected ? ' checked="checked"' : '';
    return <<< EOF
<input type="radio" name="paymode" value="{$paymode_id}"{$chk}/> {$text}&nbsp;
<a href="/popup.php?{$lang_}&amp;n={$popup_id}" target="_blank"><u>?</u></a><br/>
EOF;
}
?>
