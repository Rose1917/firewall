TARGET_NAME = firewall
SRC_FILES:=$(wildcard *.c)
HEAD_FILES:=$(wildcard include/*.h)
OBJ_FILES:=$(SRC_FILES:%.c=%.o)
obj-m += $(TARGET_NAME).o
firewall-objs := main.o rule.o handle.o chain.o util.o log.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
install:
	sudo insmod $(TARGET_NAME).ko
remove:
	sudo rmmod $(TARGET_NAME)
show:
	sudo dmesg|tail -10
check:
	lsmod|grep  "firewall" 
count:
	wc -l $(SRC_FILES) $(HEAD_FILES)
test:
	./test.sh
reinstall:remove install
