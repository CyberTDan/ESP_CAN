# ESP_CAN

## Development toolKit 

All tools required for building and flashing ESP32 installed in doker.
To build docker run from ESP_CAN directory:

```
./scripts/docker.sh build
```

To run docker image run:

```
./scripts/docker.sh run
```

To flash program to ESP connect it before running docker.
To check wich serial port is used run:

```
ls /dev | grep -i ttyUSB
```

To build and flash run:

```
idf.py -p /dev/ttyUSB<port_number> build flash monitor
```

sudo chown -R $USER ESP_CAN/