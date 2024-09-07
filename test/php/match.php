<?php

// Simple endpoint that gives different content but the same length for existing and non existing params.

echo "Hello!";
//usleep(250000);
if (isset($_GET['secret']) || isset($_GET['another_secret'])) {
  echo "You've found a secret!";
} else {
  echo "You've found a nothing";
}