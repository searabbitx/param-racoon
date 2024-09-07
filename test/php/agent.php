<?php

if ($_SERVER['HTTP_USER_AGENT'] != 'TestAgent') {
  http_response_code(400);
  die();
}

echo "Hello!";
//usleep(50000);
if (isset($_GET['secret']) || isset($_GET['another_secret'])) {
  echo "You've found a secret!";
}