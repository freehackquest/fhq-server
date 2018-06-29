# Tests for fhq-server


## Install python requirements

	`sudo pip install -r requirements.txt`



## Set the settings

Create test database by sql-queries:

	> CREATE DATABASE `test` CHARACTER SET utf8 COLLATE utf8_general_ci;
	> GRANT ALL PRIVILEGES ON test.* TO 'freehackquest_u'@'localhost' WITH GRANT OPTION;
	> FLUSH PRIVILEGES;

Change the name of the database for the tests in file /etc/freehackquest-backend/conf.ini

	name=test

And if not exists test account "admin" with password "admin" then change setings for connection in file backend-tests/classbook/lib.py

## Requirements

Tests are only run with python 3.6.
Install pytest

	$ sudo apt install python-pip python3-pytest
	$ pip3 install asyncio
	$ pip3 install websockets
	$ pip3 install PyMySQL

## Run tests

	$ cd tests
	$ python3 -m pytest .
