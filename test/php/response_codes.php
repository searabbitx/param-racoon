<?php

// Endpoint that gives the same response code for non-existing params.

echo "Hello!";
//usleep(50000);
if (isset($_GET['secret']) || isset($_GET['another_secret'])) {
  http_response_code(200);
} else {
  http_response_code(400);
}