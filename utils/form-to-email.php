<?php

$email_from = 'me@kibk.ru';//<== update the email address
$email_subject = "NUCLEO";
$email_body = "Nucleo wants to say: \n";

foreach ($_POST as $param_name => $param_val) {
    // $email_body .= "Param: $param_name; Value: $param_val<br />\n";
    $email_body .= "$param_val\n";
}

$to = "nothingnope34@gmail.com";//<== update the email address
$headers = "From: $email_from \r\n";
// $headers .= "Reply-To: $visitor_email \r\n";
//Send the email!
mail($to,$email_subject,$email_body,$headers);
//done. redirect to thank-you page.
header('Location: ../thank-you.html');

// IMPORTANT
// Function to validate against any email injection attempts
function IsInjected($str)
{
  $injections = array('(\n+)',
              '(\r+)',
              '(\t+)',
              '(%0A+)',
              '(%0D+)',
              '(%08+)',
              '(%09+)'
              );
  $inject = join('|', $injections);
  $inject = "/$inject/i";
  if(preg_match($inject,$str))
    {
    return true;
  }
  else
    {
    return false;
  }
}

?>
