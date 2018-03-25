# CONFIGURING

## Database

### Generate and remember

* Database name (default: freehackquest)
* Database user (default: freehackquest_u)
* Database user password (default: freehackquest_password_database)

### MySQL database modules:

```
$ sudo apt-get install mysql-server
$ sudo apt-get install mysql-client
```

### Create database by sql-queries

```
> CREATE DATABASE `freehackquest` CHARACTER SET utf8 COLLATE utf8_general_ci;
> CREATE USER 'freehackquest_u'@'localhost' IDENTIFIED BY 'freehackquest_password_database';
> GRANT ALL PRIVILEGES ON freehackquest.* TO 'freehackquest_u'@'localhost' WITH GRANT OPTION;
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

```
$ sudo cp ../CONFIGURING/conf.ini.example /etc/fhq-server/conf.ini
```

### Edit config 

Please look and change file /etc/fhq-server/conf.ini

