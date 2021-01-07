COMPILER := gcc
COMPILE_PATH := ${shell pwd}/include
COMPILER_FLAGS := '-g -fPIC'
LIB_LINKER_FLAGS := -shared

ALL_OBJS := objstore/*.o

all: libkvdb.so
	make clean

libkvdb.so: objstore_objs
	${COMPILER} ${ALL_OBJS} ${LIB_LINKER_FLAGS} -o bin/libkvdb.so

objstore_objs:
	COMPILER=${COMPILER} COMPILE_PATH=${COMPILE_PATH}\
		COMPILER_FLAGS=${COMPILER_FLAGS} make -C objstore

clean:
	make -C objstore clean

distclean: clean
	rm -f bin/*.out bin/*.so