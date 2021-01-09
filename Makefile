COMPILER := gcc
COMPILE_PATH := ${shell pwd}/include
COMPILER_FLAGS := '-g -fPIC'
LIB_LINKER_FLAGS := -shared

LIB_OBJS := objstore/*.o

all: example
	make clean

.PHONY: example
example: libkvdb.so
	${COMPILER} example/benchmark.c -I${COMPILE_PATH} -Lbin/ -lkvdb -g -o bin/benchmark.out

libkvdb.so: objstore_objs
	${COMPILER} ${LIB_OBJS} ${LIB_LINKER_FLAGS} -o bin/libkvdb.so

objstore_objs:
	COMPILER=${COMPILER} COMPILE_PATH=${COMPILE_PATH}\
		COMPILER_FLAGS=${COMPILER_FLAGS} make -C objstore

clean:
	make -C objstore clean

distclean: clean
	rm -f bin/*.out bin/*.so