http {
	include mime.types;
	error_page 404 =404 /404.html;
	error_page 502 =502 /errorPages/502.html;

	server {
		listen localhost:6969;

		root ./www;

		autoindex on;

		cgi py /usr/bin/python3;

		location cgi {
			max_body_size 10;
			error_page 502 =502 /errorPages/502cgi.html;
			cgi py /usr/bin/python3;
			editable on;
		}

		location A {
			editable on;
		}
	}

	server {
		listen 8007;

		root ./www;

		autoindex on;

		cgi py /usr/bin/python3;

		redirect http://localhost:8006;

		location cgi {
			error_page 404 =404 /index.html;
			cgi py /usr/bin/python3;
			editable on;
		}

		location A {
			editable on;
		}
	}
}
