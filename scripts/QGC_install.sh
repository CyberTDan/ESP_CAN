#!/bin/bash

CURRENT_DIR=$PWD

cd ~/Download

wget https://d176tv9ibo4jno.cloudfront.net/latest/QGroundControl.AppImage && chmod +x QGroundControl.AppImage

sudo apt-get remove modemmanager -y && \
    sudo apt install gstreamer1.0-plugins-bad gstreamer1.0-libav \
    gstreamer1.0-gl libqt5gui5 libfuse2 -y 

cd $CURRENT_DIR