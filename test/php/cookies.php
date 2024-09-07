<?php

// Simple endpoint that expects headers.

if (!isset($_COOKIE['PHPSESSID'])) {
  echo "No cookie!";
  die();
}
if (!isset($_COOKIE['LANG'])) {
  echo "No cookie!";
  die();
}

echo "Hello!";
if (isset($_GET['secret']) || isset($_GET['another_secret'])) {
  echo "You've found a secret!";
}