<?php
if(!isset($EXEC)) die("Access restricted");
include_once("mobi/amtools.php");
include_once("mobi/ipblock.php");
$msg=allow_ip('demo',false);
echo $msg;
if($msg) return;
if($chac!=-1){
	$uri=htmlentities($_SERVER['REQUEST_URI']);
	$prev_year=$GLOBALS['amax']['year']-1;
	if(isset($_POST['p_captcha'])){
		$captcha=$_POST['p_captcha'];
		if(is_captcha($captcha)){
            if(isset($_POST["email"]) && check_email_address($_POST["email"])){
                $email=$_POST['email'];
                $sc=get_default_cities($GLOBALS['amax']['def_cities'][$lang]); 
                $link_text=midlet_create("demo", $prev_year, $lang, $sc, "mobi/dl", 0);
                if(!$link_text){
                    echo '<p><span class="alert">Error</span></p>';
                }
                else{
                    $message=file_get_contents("mobi/dl/source/demo.mail");
                    $message=str_replace('[site]', $GLOBALS['amax']['mail_site'], $message);
                    $message=str_replace('[javalink]', $link_text, $message);
                    $mail=mailtext_w_attach($email, '', 'Astromaximum demo - download link', $message);
                    if(!$mail->ErrorInfo){
                        echo "<p>".sprintf($i18['DEMO_LINK_SENT'], $email)."</p>";
                        echo "<p><a href=\"$uri\">{$i18['BACK']}</a></p>";
                        return;
                    }
                    else{
                        echo '<p><span class="alert">'.$mail->ErrorInfo."</span></p>";
                    }
                }
            }
            else{  # wrong email
                echo "<p><span class=\"alert\">{$i18['DEMO_EMAIL_WRONG']}</span></p>";
            }
		}
		else{
			echo "<p><span class=\"alert\">{$i18['CAPTCHA_WRONG']}</span></p>";
		}
	}
	$sess=session_name().'='.session_id();
	echo $i18['DEMO_HEADER'];
    $off_email=$GLOBALS['amax']['mail_office'];
    $hidden_email=hide_email($off_email);
	$emailing=sprintf($i18['DEMO_EMAILING'], $hidden_email, $off_email);
	$msg_dlcity=sprintf($i18['DEMO_DLCITY'], '<a href="dl" target="_blank">', $prev_year);
	echo <<<EOF1

<form action="demo" method="post">
<p>{$emailing} <input name="email" type="text" size="25"/></p>
<p>{$i18['CAPTCHA_PROMPT']}</p>
<p><img src="/mobi/kcaptcha?$sess" alt="Captcha">
<input name="p_captcha" type="text" size="8"/>
</p>
<p><span class="fine">{$i18['DEMO_CALGEN']}:</span><br/><br/>
<!--<input type="radio" name="agree" value="demo" style="width:auto; border: 0px" checked="checked"/> -->
<b>ASTROMAXIMUM</b> {$i18['DEMO_DEMO']} <b>{$prev_year}</b><br/><br/>
<input type="button" class="ok_on" value="OK" onclick="form.submit()"/>
<br/><br/></p>
<p></p>
<p>{$msg_dlcity}</p>

EOF1;
return;
}
// guest user warning
$press_enter=sprintf($i18['DEMO_ENTER'],$i18['LOG_IN']);
$anchor_email = fill_input_str('ilog', $GLOBALS['amax']['demo_email']);
$anchor_pass = fill_input_str('ipwd', $GLOBALS['amax']['demo_pass']);

echo <<<EOF2
<p>{$i18['DEMO_NOTREG']}</p>
<ul>
<li>{$i18['LOGIN']}:
$anchor_email<br/><br/></li>
<li>{$i18['PWD']}:
$anchor_pass</li>
</ul>
<p>$press_enter</p>
EOF2;
?>
