http {
	include mime.types;
	error_page 404 =404 /home/ebini/Documents/cc/webserv/www/404.html;

	server {
		listen localhost:6969;
		listen 8083;
		listen 8084;
		server_name localhost;
		root ./www;
		autoindex on;

		cgi py /usr/bin/python3;
		location test {
		}
		location A {
			root ./.;
			autoindex on;
		}
	}
}
