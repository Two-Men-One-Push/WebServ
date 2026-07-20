http {
	#include mime.types;
	error_page 404 =404 /home/ebini/Documents/cc/webserv/www/4045.html;

	server {
		listen localhost:6969;
		listen localhost:6767;
		server_name localhost;
		root /home/cyber/Documents/WebServ/www;

		index index.html;
		autoindex on;
		cgi html chrome/;
		location A {
			cgi html chromeA;
		}
	}

}
