<?php
if(!isset($EXEC)) die("Access restricted");
$cur_year=$GLOBALS['amax']['year'];
$price=$GLOBALS['amax']['price'];

$admin_email='dev@astromaximum.mobi';

$paypal_email='aivush_1217502939_biz@gmail.com';//$GLOBALS['amax']['mail_office'];

$item_name="Astromaximum $cur_year";
$item_price=substr($price,1).'.00';
$currency='USD';

/*
echo "<pre>";
print_r ($_POST);
echo "</pre>";
echo "<br/>$item_name, $item_price, $currency <br/>";
*/

$thisurl='http://'.$_SERVER['SERVER_NAME']."/?$lang_&p=paypal";

if(isset($_GET['mode']) && ($_GET['mode'] =='success')){ // successful payment

//  validate trasaction
    $postdata='';
    foreach ($_POST as $key=>$value) $postdata.=$key."=".urlencode($value)."&";
    $postdata .= "cmd=_notify-validate"; 
    $curl = curl_init("https://www.sandbox.paypal.com/cgi-bin/webscr");
    curl_setopt ($curl, CURLOPT_HEADER, 0); 
    curl_setopt ($curl, CURLOPT_POST, 1);
    curl_setopt ($curl, CURLOPT_POSTFIELDS, $postdata);
    curl_setopt ($curl, CURLOPT_SSL_VERIFYPEER, 0); 
    curl_setopt ($curl, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt ($curl, CURLOPT_SSL_VERIFYHOST, 1);
    $response = curl_exec ($curl);
    curl_close ($curl);
    if ($response != "VERIFIED") die("You should not do that ...");
    
// check for receiver and transaction type and exit if no

    if ($_POST['receiver_email'] != $paypal_email || $_POST["txn_type"] != "web_accept")
        die("You should not be here ...");

// check if this transaction was not processed before 

    $r = mysql_query("SELECT order_id FROM paypal_orders WHERE txn_id='".$_POST["txn_id"]."'");
    list($duplicate) = mysql_fetch_row($r);
    mysql_free_result($r);
    if ($duplicate) die ("I feel like I met you before ...");       

// check payment attributes

    if($item_name != $_POST['item_name'] ||
        $item_price != $_POST['mc_gross'] ||
        $currency != $_POST["mc_currency"]){
            event_send("IPN error", "Payment amount mismatch\r\nTransaction ID: ".$_POST["txn_id"]);
            die("Out of money? Please contact ".$admin_email);
    }
    
    if(strtolower($_POST['payment_status']) != 'completed'){
        if($_POST['pending_reason'] != 'intl'){ // we're not in USA
            die ('Payment status is: '.$_POST['payment_status'].', reason: '. $_POST['pending_reason']);
        }
    }

// create order at last

    $order_date = date("Y-m-d H:i:s",strtotime ($_POST["payment_date"])); 

    $stat = sprintf("INSERT INTO paypal_orders SET 
        txn_id      = %s,
        order_date  = %s,
        order_total = %s,
        payer_id    = %s,
        payer_email = %s,
        item_name   = %s, 
        first_name  = %s,
        last_name   = %s,
        street      = %s, 
        city        = %s, 
        state       = %s, 
        zip         = %s, 
        country     = %s",
    qsmart('txn_id'), quote_smart($order_date), quote_smart($price),
    qsmart('payer_id'), qsmart('payer_email'), qsmart('item_name'), qsmart('first_name'),
    qsmart('last_name'), qsmart('address_street'), qsmart('address_city'), qsmart('address_state'), 
    qsmart('address_zip'), qsmart('address_country'));
//    echo $stat;
    if(!mysql_query($stat)) echo mysql_error();
    
    $order_id = mysql_insert_id();
    event_send("New order", "New order\r\nOrder ID: ". $order_id."\r\nTransaction ID: "
        .$_POST["txn_id"]);
    
    echo "Thank you, your payment is accepted. We will contact you by email after checkout and provide username and password";
    return;
}

if(isset($_GET['mode']) && ($_GET['mode'] == 'cancel')){ // cancelled
    echo "Payment cancelled";
    return;    
}

echo <<< EOF
<h4 style="font-size:12px;">1. Оплатить через PayPal:</h4>
<form method="post" action= "https://www.sandbox.paypal.com/cgi-bin/webscr">
<input type="hidden" name="cmd" value="_xclick"/>
<input type="hidden" name="business" value="$paypal_email"/>
<input type="hidden" name="item_name" value="$item_name"/>
<input type="hidden" name="item_number" value=""/>
<input type="hidden" name="amount" value="$item_price"/>
<input type="hidden" name="currency_code" value="$currency"/>
<input type="hidden" name="no_shipping" value="1"/>
<input type="hidden" name="notify_url" value="{$thisurl}"/>
<input type="hidden" name="return" value="{$thisurl}&mode=success"/>
<input type="hidden" name="rm" value="2"/>
<input type="hidden" name="cancel_return" value="{$thisurl}&mode=cancel"/>
<input type="image" src="https://www.sandbox.paypal.com/en_US/i/btn/btn_buynowCC_LG.gif" width="122" height="47" border="0" name="submit" alt="PayPal - The safer, easier way to pay online!">
</form>
EOF;

function qsmart($post_key)
{
    if(!isset($_POST[$post_key])) 
	return "''";
    else
	return quote_smart($_POST[$post_key]);
}
?>
