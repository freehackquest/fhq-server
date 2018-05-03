# lxd

## Install

```
$ sudo apt install lxd
$ sudo service lxd start
$ sudo lxd init
```

## Init

```
$ sudo lxd init
```

On ask: "Would you like LXD to be available over the network (yes/no)?": *yes*

After this:

 - Please open fhq-web-admin and change password for lxd to your password 

## Generate cerfiticates:

```
$ sudo mkdir -p /etc/fhq-server/lxd
$ sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout /etc/fhq-server/lxd/client.key -out /etc/fhq-server/lxd/client.crt

```

Help: Next command for registry certificate in lxd (or will be authomaticly by fhq server): 

```
curl --insecure \
	-X POST \
	-d '{"type": "client", "password": "your_password","name":"client_cert"}' \
	--cert /etc/fhq-server/lxd/client.crt \
	--key /etc/fhq-server/lxd/client.key \
	--url "https://127.0.0.1:8443/1.0/certificates"
```
