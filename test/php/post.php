<?php

// Simple endpoint that gives the same response for non-existing body params.

echo "Hello!";
//usleep(250000);
if (isset($_POST['secret']) || isset($_POST['another_secret'])) {
  echo "You've found a secret!";
}