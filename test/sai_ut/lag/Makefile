CC = gcc
CC_FLAGS = -I /usr/include/sai -L /usr/lib
LIBS = -lsai

all: sai_lag_unit_test

sai_lag_unit_test: sai_lag_unit_test.c
	${CC} -o $@ ${CC_FLAGS} $< ${LIBS}

simple_test: simple_test.c
	${CC} -o $@ ${CC_FLAGS} $< ${LIBS}

clean:
	rm -rf sai_lag_unit_test simple_test
