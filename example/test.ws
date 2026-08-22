http {
	include mime.types;
	error_page 404 =404 /errorPages/404.html
	error_page 502 =502 /errorPages/502.html;

	server {
		listen localhost:6969;
		listen 8007;
		timeout 50s;
		root ./example/www;

		autoindex on;

		cgi py /usr/bin/python3;

		location cgi {
			alias ./ok;
			error_page 502 =502 /errorPages/502cgi.html;
			cgi py /usr/bin/python3;
			editable on;
		}

		location A {
			editable on;
		}

		location errorPages {
			alias ./error;
		}
	}
}
