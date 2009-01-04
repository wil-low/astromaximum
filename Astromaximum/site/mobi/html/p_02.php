<?php
    $title='PayPal';
    $custom_content='-- примечания --';
    
if(!isset($EXEC)){ // don't show PayPal button when in popup
        

$cur_year=$GLOBALS['amax']['year'];
$price=$GLOBALS['amax']['price'];

$admin_email='dev@astromaximum.mobi';

$paypal_email='aivush_1217502939_biz@gmail.com';//$GLOBALS['amax']['mail_office'];

$item_name="Astromaximum $cur_year";
$item_price=substr($price,1).'.00';
$currency='USD';

$thisurl='http://'.$_SERVER['SERVER_NAME']."/?$lang_&p=paypal";

echo <<< EOF
<form method="post" action= "https://www.sandbox.paypal.com/cgi-bin/webscr">
<input type="hidden" name="cmd" value="_xclick"/>
<input type="hidden" name="business" value="$paypal_email"/>
<input type="hidden" name="item_name" value="$item_name"/>
<input type="hidden" name="item_number" value=""/>
<input type="hidden" name="amount" value="$item_price"/>
<input type="hidden" name="currency_code" value="$currency"/>
<input type="hidden" name="no_shipping" value="1"/>
<input type="hidden" name="notify_url" value="{$thisurl}&action=ipn"/>
<input type="hidden" name="return" value="{$thisurl}&action=success"/>
<input type="hidden" name="rm" value="2"/>
<input type="hidden" name="cancel_return" value="{$thisurl}&action=cancel"/>
<input type="image" src="https://www.sandbox.paypal.com/en_US/i/btn/btn_buynowCC_LG.gif" 
	width="122" height="47" border="0" name="submit" 
	alt="PayPal - The safer, easier way to pay online!"/>
</form>
EOF;

}else{ ?>

PayPal instruction

<?php } ?>