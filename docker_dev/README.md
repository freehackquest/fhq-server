# Run docker for dev

## Start

use a command or script:

```
$ docker-compose -f docker-compose.dev.yml up --build
```

## console run

```
$ docker exec -it docker_dev_fhqdev_1 bash
# cd /root/fhq-server.git/fhq-server 
# ./build_simple.sh
# ./fhq-server -wd ../docker_dev/data start
```