dmesg -C
firewall start
firewall append --chain OUTCOME --dip 127.0.0.1 --target DROP
firewall log
