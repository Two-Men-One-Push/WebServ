server
{
	listen 80;
	location /
	{
		include test2.ws;
	}
}