#!/bin/bash

print_help() {
   echo "Wrapper under docker API for Pendulum.
It encapsulates all necessary docker flags for working with Inclinometer and properly handles image versions.
https://github.com/CyberTDan/Pendulum

usage: docker.sh [build | pull | push | run | interactive | kill | help]

Commands:
build           Build docker image.
pull            Pull docker image.
push            Push docker image.
run             Run yelldozer container.
interactive     Run container in interactive mode.
kill            Kill all containers.
help            Print this message and exit"
}

setup_config() {
    TAG_NAME=v0.0.1
    DOCKERHUB_REPOSITOTY=trihlebdv/espcan
    parentdir="$(dirname "$(pwd)")"
    if uname -m | grep -q 'aarch64'; then
        TAG_NAME="$TAG_NAME""arm64"
    elif uname -m | grep -q 'x86_64'; then
        TAG_NAME="$TAG_NAME""amd64"
    else
        echo "unknown architecture"
        exit
    fi


    DOCKER_CONTAINER_NAME=$DOCKERHUB_REPOSITOTY:$TAG_NAME
    
    DOCKER_FLAGS="-it --rm --net=host                           \
                  -p 14557:14557/udp                            \
                  --privileged                                  \
                  -e DISPLAY=$DISPLAY                           \
                  -e QT_X11_NO_MITSHM=1                         \
                  -v /tmp/.X11-unix:/tmp/.X11-unix:rw           \
                  -v $parentdir:/root/esp/ESP_CAN"

    echo "TAG_NAME is" $TAG_NAME
    echo "DOCKERHUB_REPOSITOTY is" $DOCKERHUB_REPOSITOTY
    echo "Docker flags is" $DOCKER_FLAGS
}

build_docker_image() {
    setup_config
    docker build -t $DOCKER_CONTAINER_NAME ..
}

pull_docker_image() {
    setup_config
    docker pull $DOCKER_CONTAINER_NAME
}

push_docker_image() {
    setup_config
    docker push $DOCKER_CONTAINER_NAME
}

run() {
    setup_config
    xhost +local:docker
    docker run $DOCKER_FLAGS $DOCKER_CONTAINER_NAME /bin/bash
}

run_interactive() {
    setup_config
    xhost +local:docker
    sudo docker container run --rm -it $DOCKER_FLAGS -v /dev/bus/usb:/dev/bus/usb $DOCKER_CONTAINER_NAME /bin/bash
}

kill_all_containers() {
    docker kill $(docker ps -q)
}

cd "$(dirname "$0")"

if [ "$1" = "build" ]; then
    build_docker_image
elif [ "$1" = "pull" ]; then
    pull_docker_image
elif [ "$1" = "push" ]; then
    push_docker_image
elif [ "$1" = "run" ]; then
    run
elif [ "$1" = "interactive" ]; then
    run_interactive
elif [ "$1" = "kill" ]; then
    kill_all_containers
else
    print_help
fi
