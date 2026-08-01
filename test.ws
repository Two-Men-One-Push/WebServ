http {
	include mime.types;
	error_page 404 =404 /404.html;

	server {
		listen localhost:6969;
		listen 8083;
		listen 8084;

		server_name localhost;
		root ./www;

		index /index.html;

		cgi py /usr/bin/python3;

		location A {
			editable on;
		}
	}
}
