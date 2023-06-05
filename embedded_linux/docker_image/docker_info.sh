#!/bin/bash

DOCKER_WORKDIR="/home/cadtc/host_share_folder"
DOCKER_CONTER_NAME="cadtc-rockpi4b-20220814"
DOCKER_LOGIN_ID="cadtc"
#DOCKER_LOGIN_ID="root"

DOCKER_IMAGE_ID=13a0f10cda6b
DOCKER_REPOS_NAME=xlloss
DOCKER_IMAGE_NAME="${DOCKER_REPOS_NAME}:cadtc-rockpi4b"
#HOST_SHARE_FOLDER=/home/slash/work/for_cadtc/
HOST_SHARE_FOLDER=/tmp/


#################################
# start docker                ###
#################################

docker run --name ${DOCKER_CONTER_NAME} \
	-e DISPLAY \
	-v /tmp/.X11-unix:/tmp/.X11-unix \
	-v ${HOST_SHARE_FOLDER}:${DOCKER_WORKDIR} \
	-it --net=host \
	-u ${DOCKER_LOGIN_ID} \
	--hostname ${DOCKER_CONTER_NAME} \
	-w ${DOCKER_WORKDIR} ${DOCKER_IMAGE_ID} /bin/bash
