http{
	server{
		listen 80;
		root /var/www/html;
		location ///1// {
			location ///2/// {
				location ///uri3/// {
					location ///uri4/// {
					}
				}
			}
		}
		location "///1/5/2../../..////" {
		}
	}
}