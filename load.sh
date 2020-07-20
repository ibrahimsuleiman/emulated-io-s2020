#!/bin/bash

sudo insmod egpio_driver.ko
sudo mknod /dev/egpio0 c 42 0
sudo mknod /dev/egpio1 c 42 1
sudo mknod /dev/egpio2 c 42 2
sudo mknod /dev/egpio3 c 42 3


