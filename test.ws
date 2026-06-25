http{
	types
	{
		html text/html;
		php application/x-httpd-php;
	}
	client_max_body_size 42Mio;
	error_page 404 =301 /404.html;
	server{
		listen 80;
		server_name server1;
		root "/var/www/html";
		index index.html index.php;
		error_page 504 =305 /404.html;
		error_page 501 502 503 =301 /404.html;
		client_max_body_size 42;
		types
		{
			okay text/html;
			php application/x-httpd-php;
		}
		types
		{
			bite bite/image;
		}
		location uri
		{
			client_max_body_size 50;
			location uri2 {
				client_max_body_size 100;
			}
		}
	}
}