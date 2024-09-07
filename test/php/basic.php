<?php

// Simple endpoint that gives the same response for non-existing params.

echo "Hello!";
//usleep(50000);
if (isset($_GET['secret']) || isset($_GET['another_secret'])) {
  echo "You've found a secret!";
}