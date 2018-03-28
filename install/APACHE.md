# APACHE

* [CONFIGURING](install/CONFIGURING.md)
* [HOW-TO-BUILD](install/HOW-TO-BUILD.md)


## Configure proxing websockets


```
$ sudo a2enmod proxy
$ sudo a2enmod proxy_http
$ sudo a2enmod proxy_connect
$ sudo a2enmod proxy_wstunnel
$ sudo a2enmod headers
````

## Configure proxy websocket (for http)

*if you get reposrts with problems with port from users*

```
<VirtualHost *:80>
	
	...
	
	ProxyRequests Off
	ProxyPreserveHost On
	ProxyVia full

	ProxyPass "/api-ws/" "ws://localhost:1234/"
</VirtualHost>
```

## Configure proxy websocket (for https)

*if you get reposrts with problems with port from users*

```
<VirtualHost *:443>
	...
	
	SSLProxyEngine on
	ProxyRequests Off
	ProxyPreserveHost On
	ProxyVia full

	ProxyPass "/api-wss/" "wss://localhost:4613/"
	
</VirtualHost>
```
