all: objstore.o ptree.o keylist.o

objstore.o: objstore.c
	${COMPILER} -I${COMPILE_PATH} ${COMPILER_FLAGS} -c objstore.c

keylist.o: keylist.c
	${COMPILER} -I${COMPILE_PATH} ${COMPILER_FLAGS} -c keylist.c

ptree.o: ptree.c
	${COMPILER} -I${COMPILE_PATH} ${COMPILER_FLAGS} -c ptree.c

clean:
	rm -f *.o
