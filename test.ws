http{
	error_page 403 404 /404.html;
	error_page 500 =200 /101.html;
	cgi .py .pyc /usr/bin/python3;
	types {text/html .html .bite;}
	server{
		listen 80;
		root /var/www/html;
		server_name localhost;
		index index.html index.htm;
		autoindex on;
		upload_path /var/www/html;
		location A {
			location B {
				location C {
					location D {
					}
				}
			}
		}
		location E {
		}
	}
}