http {
	include mime.types;
	error_page 404 =404 /home/cyber/Documents/WebServ/www/404.html;
	
	server {
		listen 80;
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