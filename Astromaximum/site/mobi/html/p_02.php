<?php
    $title='PayPal';
    $custom_content='-- примечания --';
    
    if(!isset($EXEC)){ // don't show PayPal button when in popup    
?>
Paypal button

<?php }else{ ?>

PayPal instruction

<?php } ?>