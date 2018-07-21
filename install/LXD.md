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

 - Please open fhq-web-admin and change `lxd_mode` to `enabled`

## Generate certificates:

```
$ sudo mkdir -p /etc/fhq-server/lxd
$ sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout /etc/fhq-server/lxd/client.key -out /etc/fhq-server/lxd/client.crt

```

Help: Next command for registry certificate in lxd (It is necessary to enter a password from the LXD.):

```
$ fhq-server -mclxd
```
