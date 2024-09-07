<?php

$entityBody = file_get_contents('php://input');
$json = json_decode($entityBody);

echo "Hello!";
if (isset($json->secret) || isset($json->another_secret)) {
  echo "You've found a secret!";
}