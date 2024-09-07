<?php

// Simple endpoint that expects headers.

if (!isset(getallheaders()["X-Required-Header"])) {
  echo "No header!";
  die();
}
if (!isset(getallheaders()["X-Other-Required-Header"])) {
  echo "No header!";
  die();
}

echo "Hello!";
if (isset($_GET['secret']) || isset($_GET['another_secret'])) {
  echo "You've found a secret!";
}