# this only lasts until you restart your computer :)
sudo iptables -A OUTPUT -p tcp --tcp-flags RST RST --sport 80 -j DROP
sudo python3 ./replicate.py 192.168.0.15
