all:	build/process

build/process: src/process.c build/file_utils.o build/common.o
	cc -g -Wall -Iinclude $^ -o $@

build/file_utils.o: src/file_utils.c include/file_utils.h include/common.h
	cc -g -Wall -Iinclude -c $< -o $@

build/common.o: src/common.c include/common.h
	cc -g -Wall -Iinclude -c $< -o $@

clean:
	rm -fr build/*
