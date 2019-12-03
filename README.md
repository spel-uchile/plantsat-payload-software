# PlantSat payload flight software

## Prepare db

1. Install postgresql and create the database

```shell script
    # Install an enable postgres
    sudo apt-get install postgresql
    sudo systemctl enable postgresql
    sudo systemctl start postgresql
    
    # Create database and user
    cd SUCHAI_Flight_Software/sandbox
    sh database_setup.sh
    cd -
  
    ## Enable remote access to db
    # Add the following line to  /etc/postgresql/<version>/main/postgresql.conf
    listen_addresses='*'
    # Add the following lines to /etc/postgresql/<version>/main/pg_hba.conf
    host    all   all   0.0.0.0/0   md5 #ipv4 range
    host    all   all   ::0/0       md5 #ipv6 range
    # Restart postgres
    sudo systemctl restart postgresql
```

2. Enable i2c

```shell script
    sudo raspi-config
    5. Interfacing options -> I2C -> YES
    6. Interfacing options -> SPI -> YES
```

## Build

1. Download and configure the SUCHAI Flight Software

```shell script
    sh install.sh
```
2. Build the payload software

```shell script
    mkdir build
    cd build
    cmake ..
    make
```
