#!/bin/bash

docker-compose -f docker-compose.dev.yml down
rm -rf mysql_data
