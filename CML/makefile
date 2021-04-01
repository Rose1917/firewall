SRC_FILES:=$(wildcard *.c)
HEAD_FILES:=$(wildcard include/*.h)
OBJ_FILES:=$(SRC_FILES:%.c=bin/%.o)
FLAGS= -w -g
TARGET=firewall
TARGET_PATH=/usr/sbin/
main:${OBJ_FILES} 
	cc -W -o $(TARGET_PATH)$(TARGET) ${OBJ_FILES}  $(FLAGS)
bin/%.o : %.c 
	cc -c $(FLAGS) $< -o $@
run:
	./$(TARGET)
clean:
	rm -f  $(TARGET) bin/*.o
debug: 
	gdb ./$(TARGET)
count:
	wc -l $(SRC_FILES) $(HEAD_FILES)
sql:
	./mysql.sh
test:
	./test.sh
remake:clean main
