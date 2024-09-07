<?php

// Simple endpoint that reflects current url.

$currentUrl = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] === 'on' ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";
echo "<h1>Hello!</h1>";
echo "<p><a href=\"$currentUrl\">current url</a></p>";
//usleep(250000);
if (isset($_GET['secret']) || isset($_GET['another_secret'])) {
  echo "You've found a secret!";
}

echo "<p>Bye! <a href=\"$currentUrl\">current url</a></p>";