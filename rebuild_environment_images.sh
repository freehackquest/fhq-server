#!/bin/bash

TAG=v$(date +"%Y-%m-%d")

docker build --rm --tag sea5kg/fhq-server-build-environment:$TAG -f Dockerfile.build-environment .
docker build --rm --tag sea5kg/fhq-server-build-environment:latest -f Dockerfile.build-environment .
docker build --rm --tag sea5kg/fhq-server-release-environment:$TAG -f Dockerfile.release-environment .
docker build --rm --tag sea5kg/fhq-server-release-environment:latest -f Dockerfile.release-environment .
