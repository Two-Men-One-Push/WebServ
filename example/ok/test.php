#!/usr/bin/env php-cgi
<?php
$method = $_SERVER["REQUEST_METHOD"] ?? "GET";
$query = $_SERVER["QUERY_STRING"] ?? "";
$ctype = $_SERVER["CONTENT_TYPE"] ?? "";
$clen = $_SERVER["CONTENT_LENGTH"] ?? "0";

// Read POST body
$body = "";
if ($method === "POST") {
	$body = file_get_contents("php://stdin", false, null, 0, (int) $clen);
}

// Parse params
$params = [];
parse_str($query, $params);
if (
	$method === "POST" &&
	strpos($ctype, "application/x-www-form-urlencoded") !== false
) {
	$post_params = [];
	parse_str($body, $post_params);
	$params = array_merge($params, $post_params);
}

header("Content-Type: text/html");
?>
<!DOCTYPE html>
<html><head><title>CGI Test</title></head><body>
<h1>CGI Debug</h1>

<h2>Request</h2>
<table border="1" cellpadding="4">
  <tr><td>Method</td><td><?= htmlspecialchars($method) ?></td></tr>
  <tr><td>Query String</td><td><?= htmlspecialchars($query) ?></td></tr>
  <tr><td>Content-Type</td><td><?= htmlspecialchars($ctype) ?></td></tr>
  <tr><td>Content-Length</td><td><?= htmlspecialchars($clen) ?></td></tr>
  <?php if ($body): ?>
  <tr><td>Body</td><td><?= htmlspecialchars($body) ?></td></tr>
  <?php endif; ?>
</table>

<?php if (!empty($params)): ?>
<h2>Parsed Parameters</h2>
<table border="1" cellpadding="4">
  <?php foreach ($params as $k => $v): ?>
  <tr><td><?= htmlspecialchars($k) ?></td><td><?= htmlspecialchars(
	$v,
) ?></td></tr>
  <?php endforeach; ?>
</table>
<?php endif; ?>

<h2>Environment Variables</h2>
<table border="1" cellpadding="4">
<?php
$cgi_vars = [
	"SERVER_NAME",
	"SERVER_PORT",
	"SERVER_PROTOCOL",
	"REQUEST_METHOD",
	"REQUEST_URI",
	"SCRIPT_NAME",
	"PATH_INFO",
	"QUERY_STRING",
	"CONTENT_TYPE",
	"CONTENT_LENGTH",
	"HTTP_HOST",
	"HTTP_USER_AGENT",
	"HTTP_ACCEPT",
];
foreach ($cgi_vars as $var) {
	$val = $_SERVER[$var] ?? "<not set>";
	echo "<tr><td>" .
		htmlspecialchars($var) .
		"</td><td>" .
		htmlspecialchars($val) .
		"</td></tr>\n";
}
?>
</table>

</body></html>
