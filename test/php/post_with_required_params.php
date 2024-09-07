<?php

// Simple endpoint that gives the same response for non-existing body params.

if (!isset($_POST['required']) || !isset($_POST['otherrequired'])) {
  http_response_code(400);
  echo "Required params missing";
  die(); 
}

echo "Hello!";
//usleep(250000);
if (isset($_POST['secret']) || isset($_POST['another_secret'])) {
  echo "You've found a secret!";
}