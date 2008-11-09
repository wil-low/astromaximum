<?php 
if(!isset($EXEC)) die("Access restricted");
$current_year=$GLOBALS['amax']['year'];
$chac=check_access();
$uri=htmlentities($_SERVER['REQUEST_URI']);

$msg=$i18['REGFORM_REQF'];

$email=$email2=$nick=$paymode=$model='';

function present($key){
    return isset($_POST[$key]) && strlen(trim($_POST[$key]))>0;
}

function alert($str){
    global $i18;
    return '<span class="alert">'.$i18[$str].'</span>';
}

if(present('email1') && present('email2') && present('nick') && present('p_captcha')
    && present('paymode') && present('model') && present('agree') &&
    strcmp($_POST['agree'], 'on') == 0){ 

    $email=$_POST['email1'];
    $email2=$_POST['email2'];
    $nick=$_POST['nick'];
    $paymode=$_POST['paymode'];
    $model=$_POST['model'];

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
// create new customer w/o password
        $passkey=$nick.$model.date("l dS of F Y h:i:s A").mt_rand();
        $passkey=pwd_convert2(pwd_convert1($email, $passkey));
        
        $stat=sprintf("INSERT INTO customers (realname,email,hash,subscr_date,".
            "paymode_id,model,active) values (%s,%s,%s,CURRENT_DATE,%d,%s,0)",
            quote_smart($nick),
            quote_smart($email),
            quote_smart($passkey),
            quote_smart($paymode),
            quote_smart($model)
        );
        if(!mysql_query($stat)){
            echo mysql_error(); break;
        }
        $mail=confirmation_send($email, $nick, $passkey);
        echo $mail->ErrorInfo.'<br/>'.$i18['INSTR_SENT'];
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
			$str=midlet_create("tb", $year, $lang, $sc, "mobi/dl", true);
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
	$prompt=sprintf($i18['CONFIRM_TRIAL'], $lang_);
	$str=($tries[0]==1)? $i18['ONE_MORE_COPY']."<br/><br/>": '';
	echo dload_tries_prompt($tries, 0, $str, $prompt);
	echo "</form>";
	echo "<br/><br/><br/>\n";
	echo "<form action=\"$uri\" method=\"post\">\n";
    echo '<input type="hidden" name="demo" value="1"/>';
	echo "<h4>".sprintf($i18['DLOAD4YEAR'],"<select name=\"yagree\">$out</select>")."</h4>";
	echo dload_tries_prompt($tries, 2, '', $i18['GENERATE?']);
	echo "</form>";
    return;
}
$agree=dload_prompt(sprintf($i18['CONFIRM_TRIAL'], $lang_), false);
$sess=session_name().'='.session_id();

// TODO: maintain these options when changing dic_paymode table!

echo <<< EOF
<h4>{$i18['REGFORM_H']}</h4>
<form id="regform" action="$uri" method="post">
<p>$msg</p>
<input type="hidden" name="demo"/>
<p>{$i18['REGFORM_EMAIL_1']}<br/>
<input type="text" name="email1" size="25" value="$email"/><br/>
{$i18['REGFORM_EMAIL_2']}<br/>
<input type="text" name="email2" size="25" value="$email2"/></p>
<p>{$i18['REGFORM_NAME']}<br/>
<input type="text" name="nick" size="25" value="$nick"/></p>
<p><img src="mobi/kcaptcha?$sess" alt="Captcha">
<input type="text" name="p_captcha" size="8"/></p>
<p>{$i18['REGFORM_PAYMODE']}</p>
<p class="no_border">
<input type="radio" name="paymode" value="2" checked="checked"/>PayPal<br/>
<input type="radio" name="paymode" value="4"/>{$i18['REGFORM_OTP']}<br/>
<input type="radio" name="paymode" value="5"/>{$i18['REGFORM_RAIFF']}<br/>
</p>
<p>{$i18['REGFORM_MODEL']}<br/>
<input type="text" name="model" size="25" value="$model"/></p>
$agree
</form>
EOF;

/*
echo "<form action=\"$uri\" method=\"post\">\n";
$prompt=sprintf($i18['CONFIRM_TRIAL'], $lang_);
echo "<br/>".$str.dload_prompt($prompt, $disabled);
echo "</form>";
                                      */
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
?>
