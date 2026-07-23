http {
	include mime.types;
	error_page 404 =404 /home/ebini/Documents/cc/webserv/www/404.html;

	server {
		listen localhost:6969;
		listen 8083;
		listen 8084;
		server_name localhost;
		root /home/ebini/Documents/cc/webserv/www;

		index index.html;
		autoindex on;
		cgi py /opt/pyenv/shims/python3;
		location test {
			redirect 301 /index.html;
		}
		location chess {
			redirect 301 /cgi/test.py;
		}
		location A {
			cgi html chromeA;
		}
	}

}
