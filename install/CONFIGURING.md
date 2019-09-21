# CONFIGURING

## Database

### Generate and remember

* Database name (default: fhq)
* Database user (default: fhq_u)
* Database user password (default: fhq_pwd_db)

### MySQL database modules:

```
$ sudo apt-get install mysql-server
$ sudo apt-get install mysql-client
```

### Create database by sql-queries

```
> CREATE DATABASE `fhq` CHARACTER SET utf8 COLLATE utf8_general_ci;
> CREATE USER 'fhq_u'@'localhost' IDENTIFIED BY 'fhq_pwd_db';
> GRANT ALL PRIVILEGES ON fhq.* TO 'fhq_u'@'localhost' WITH GRANT OPTION;
> FLUSH PRIVILEGES;
```

## Files & Directories 

### Create directories

```
$ sudo mkdir /var/log/fhq-server
$ sudo mkdir /usr/share/fhq-server
$ sudo mkdir /etc/fhq-server
```

### Copy config example

Default ports:

* 1234 - ws (http)
* 4613 - wss (https)


```
$ sudo cp ../fhq-server/misc/push_to_ppa/install_files/fhq-server.conf.sample /etc/fhq-server/fhq-server.conf
```

### Edit config 

Please look and change file /etc/fhq-server/fhq-server.conf

