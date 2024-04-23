kill -9 $(pgrep -f "ctn2")
sleep 3
./ctn2 &
tail -f /var/log/syslog