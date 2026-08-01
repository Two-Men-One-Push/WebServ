http {
	include mime.types;
	error_page 404 =404 /404.html;

	server {
		listen localhost:6969;
		listen 8083;
		listen 8084;
		server_name localhost;
		root /home/ethebaul/Documents/WebServ/www;
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
			editable on;
		}
	}
}
