<?php

// Simple endpoint that gives the same response for non-existing params.

echo "Hello!";
usleep(250000);
if (isset($_GET['secret'])) {
  echo "You've found a secret!";
}