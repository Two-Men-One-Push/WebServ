http {
	include mime.types;
	error_page 404 =404 /404.html;

	server {
		listen localhost:8006;

		root ./www;

		autoindex on;
		index index.html;

		cgi py /usr/bin/python3;

		redirect http://localhost:8007;

		location cgi {
			error_page 404 =404 /index.html;
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
		index index.html;

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
