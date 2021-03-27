# docker-compose example for fhq-server

## Reguiremts

* docker
* docker-compose

## Start

For start just run this:

```
$ cd ~
$ git clone https://github.com/freehackquest/fhq-server
$ cd ./contrib/docker_compose_example
$ docker-compose up
```

## Configurations

1. Default port used 801 / 4431 (look in docker-compose)
2. Admin Panel (Login: `admin`; Password: `admin`): http://localhost/admin/?settings=
3. Change settings in 
    * "Server folders"
    * "Mail Settings"
