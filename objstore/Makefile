all: objstore.o ptree.o

objstore.o: objstore.c
	${COMPILER} -I${COMPILE_PATH} ${COMPILER_FLAGS} -c objstore.c

ptree.o: ptree.c
	${COMPILER} -I${COMPILE_PATH} ${COMPILER_FLAGS} -c ptree.c

clean:
	rm -f *.o
