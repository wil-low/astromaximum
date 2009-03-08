<?php
if(!isset($EXEC)) die("Access restricted");

if (empty($_GET['action'])){
	$_GET['action']='';
	if($chac != 3){
		die ('Access restricted');
	}
}  

$cur_year=$GLOBALS['amax']['year'];
$price=$GLOBALS['amax']['price'];

//$paypal_email='aivush_1217502939_biz@gmail.com';
$paypal_email=$GLOBALS['amax']['paypal_email'];

$item_name="Astromaximum $cur_year";
$item_price='0.01'; //substr($price,1).'.00';
$currency='USD';

/*
echo "<pre>";
print_r ($_POST);
echo "</pre>";
echo "<br/>$item_name, $item_price, $currency <br/>";
*/
$thisurl='http://'.$_SERVER['SERVER_NAME']."/?$lang_&p=paypal";

require_once('mobi/paypal/paypal.class.php');  // include the class file
$p = new paypal_class;             // initiate an instance of the class
$p->paypal_url = $GLOBALS['amax']['paypal_url'].'/cgi-bin/webscr';

switch ($_GET['action']) {

   case 'success':      // Order was successful...
   
      // This is where you would probably want to thank the user for their order
      // or what have you.  The order information at this point is in POST 
      // variables.  However, you don't want to "process" the order until you
      // get validation from the IPN.  That's where you would have the code to
      // email an admin, update the database with payment status, activate a
      // membership, etc.  
 
      echo "<h3>Thank you for your order.</h3>";
      echo "We will contact you by email after checkout and provide username and password.<br/><br/>";
      foreach ($_POST as $key => $value) { echo "$key: $value<br>"; }
      
      // You could also simply re-direct them to another page, or your own 
      // order status page which presents the user with the status of their
      // order based on a database (which can be modified with the IPN code 
      // below).
      
      break;
      
   case 'cancel':       // Order was canceled...

      // The order was canceled before being completed.
 
      echo "The order was canceled.";
      
      break;
      
   case 'ipn':          // Paypal is calling page for IPN validation...
   
      // It's important to remember that paypal calling this script.  There
      // is no output here.  This is where you validate the IPN data and if it's
      // valid, update your database to signify that the user has payed.  If
      // you try and use an echo or printf function here it's not going to do you
      // a bit of good.  This is on the "backend".  That is why, by default, the
      // class logs all IPN data to a text file.
      
      if ($p->validate_ipn()) {
          
         // Payment has been recieved and IPN is verified.  This is where you
         // update your database to activate or process the order, or setup
         // the database with the user's order details, email an administrator,
         // etc.  You can access a slew of information via the ipn_data() array.
  
         // Check the paypal documentation for specifics on what information
         // is available in the IPN POST variables.  Basically, all the POST vars
         // which paypal sends, which we send back for validation, are now stored
         // in the ipn_data() array.
  
         // For this example, we'll just email ourselves ALL the data.
         $subject = 'Recieved IPN Payment';
         $body =  "An instant payment notification was successfully recieved\n";
         $body .= "from ".$p->ipn_data['payer_email']." on ".date('m/d/Y');
         $body .= " at ".date('g:i A')."\n\nDetails:\n";
         
         foreach ($p->ipn_data as $key => $value) { $body .= "\n$key: $value"; }

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
	    qsmart('txn_id'), quote_smart($order_date), quote_smart($item_price),
	    qsmart('payer_id'), qsmart('payer_email'), qsmart('item_name'), qsmart('first_name'),
	    qsmart('last_name'), qsmart('address_street'), qsmart('address_city'), qsmart('address_state'), 
	    qsmart('address_zip'), qsmart('address_country'));
	 //    echo $stat;
	    if(!mysql_query($stat)) echo mysql_error();
	    
	    $order_id = mysql_insert_id();
	 $body .= "\nOrder id: $order_id";
         event_send($subject, $body);
      }
      break;
 }     
/*
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
    qsmart('txn_id'), quote_smart($order_date), quote_smart($item_price),
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
*/

function qsmart($post_key)
{
	global $p;
	if(!isset($p->ipn_data[$post_key])) 
		return "''";
	else
		return quote_smart($p->ipn_data[$post_key]);
}
?>
