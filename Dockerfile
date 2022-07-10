ARG UBUNTU_VERSION=20.04

FROM ubuntu:${UBUNTU_VERSION}
LABEL description="ESP_CAN"
LABEL maintainer="trihlebdv@gmail.com"
SHELL ["/bin/bash", "-c"]

#  setup time zone
ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

#  insall required system packages
RUN apt-get update                          &&  \
    apt-get upgrade -y                      &&  \ 
    apt-get install -y  git python3-pip python-is-python3  \
    wget make libncurses-dev flex bison gperf python3 python3-serial \
    python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

#  create esp workdir
WORKDIR /root/esp

#  setup ESP toolKit
RUN wget https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-97-gc752ad5-5.2.0.tar.gz && \
    tar -xzf xtensa-esp32-elf-linux64-1.22.0-97-gc752ad5-5.2.0.tar.gz && \
    rm xtensa-esp32-elf-linux64-1.22.0-97-gc752ad5-5.2.0.tar.gz
ENV PATH="/root/esp/xtensa-esp32-elf/bin::${PATH}"

#  download ESP-IDF software 
RUN git clone -b v4.4.1 --recursive https://github.com/espressif/esp-idf.git
RUN /root/esp/esp-idf/install.sh all
ENV IDF_PATH=/root/esp/esp-idf

RUN python -m pip install --user -r $IDF_PATH/requirements.txt


COPY ./scripts/entrypoint.sh /
ENTRYPOINT ["/entrypoint.sh"]
