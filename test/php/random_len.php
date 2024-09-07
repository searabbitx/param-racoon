<?php

// Endpoint that gives the same response for non-existing params +- 5 random chars.

echo "Hello!";

// random chars
$rand = rand(0, 5);
$randChars = "";
for ($i = 0; $i < $rand; $i++) {
  $randChars .= 'x';
}
echo $randChars;

//usleep(50000);
if (isset($_GET['secret']) || isset($_GET['another_secret'])) {
  echo "You've found a secret!";
}