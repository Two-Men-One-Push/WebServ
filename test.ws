http{
	server{
		listen 80;
		location uri
		{
			location uri2 {
				listen 8080;
			}
		}
	}
}