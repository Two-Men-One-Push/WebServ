http {
	include mime.types;
	error_page 400 =500 /home/cyber/Documents/WebServ/www/400.html;
	
	server {
		listen 80;
		server_name localhost;
		root /home/cyber/Documents/WebServ/www;

		index index.html;
		location A {

			autoindex on;
			location B {
				cgi	.py python;
				location C {

					location D {

						location E {

						}
					}
				}
			}
		}
	}
}