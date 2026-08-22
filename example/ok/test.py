#!/usr/bin/env python3
import os
import sys


def parse_query_string(qs):
    if not qs:
        return {}
    params = {}
    for pair in qs.split("&"):
        if "=" in pair:
            k, v = pair.split("=", 1)
            params[k] = v
    return params


method = os.environ.get("REQUEST_METHOD", "GET")
query = os.environ.get("QUERY_STRING", "")
ctype = os.environ.get("CONTENT_TYPE", "")
clen = os.environ.get("CONTENT_LENGTH", "0")

# Read POST body if any
body = ""
if method == "POST":
    try:
        length = int(clen) if clen else 0
        body = sys.stdin.read(length)
    except Exception:
        body = ""

params = parse_query_string(query)
if method == "POST" and "application/x-www-form-urlencoded" in ctype:
    params.update(parse_query_string(body))

# CGI response — headers first, then blank line, then body
print("Content-Type: text/html", end="\r\n")
print(end="\r\n")
print("<!DOCTYPE html>")
print("<html><head><title>CGI Test</title></head><body>")
print("<h1>CGI Debug</h1>")

print("<h2>Request</h2>")
print("<table border='1' cellpadding='4'>")
print(f"<tr><td>Method</td><td>{method}</td></tr>")
print(f"<tr><td>Query String</td><td>{query}</td></tr>")
print(f"<tr><td>Content-Type</td><td>{ctype}</td></tr>")
print(f"<tr><td>Content-Length</td><td>{clen}</td></tr>")
if body:
    print(f"<tr><td>Body</td><td>{body}</td></tr>")
print("</table>")

if params:
    print("<h2>Parsed Parameters</h2>")
    print("<table border='1' cellpadding='4'>")
    for k, v in params.items():
        print(f"<tr><td>{k}</td><td>{v}</td></tr>")
    print("</table>")

print("<h2>Environment Variables</h2>")
print("<table border='1' cellpadding='4'>")
cgi_vars = [
    "CONTENT_TYPE",
    "CONTENT_LENGTH",
    "SERVER_NAME",
    "SERVER_PORT",
    "SERVER_PROTOCOL",
    "REQUEST_METHOD",
    "SCRIPT_NAME",
    "PATH_INFO",
    "QUERY_STRING",
    "HTTP_HOST",
    "HTTP_USER_AGENT",
    "HTTP_ACCEPT",
]
for var in cgi_vars:
    val = os.environ.get(var, "<not set>")
    print(f"<tr><td>{var}</td><td>{val}</td></tr>")
print("</table>")

print("</body></html>")
