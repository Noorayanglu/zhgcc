ip link set can1 name can2
ip link set can0 name can1
insmod csm330a.ko

ifconfig can0 down
ip link set can0 type can bitrate 250000
ifconfig can0 up
ifconfig can1 down
ip link set can1 type can bitrate 500000
ifconfig can1 up
