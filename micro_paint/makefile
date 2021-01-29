YOUR_SRCS =	../minicro_paint.c
OUR_SRCS  =	our_micro_paint.c
GEN_SRCS  =	generate_example.cpp

OURS  =	our_micro_paint
YOURS =	micro_paint
GEN   =	gen

CFLAGS = -Wall -Wextra -Werror
LIBFLAGS = -lm

yours: ${YOURS}
${YOURS}: ${YOUR_SRCS}
	gcc ${YOUR_SRCS} -o ${YOURS} -D MICRO\
		${CFLAGS} ${LIBFLAGS}

ours: ${OURS}
${OURS}: ${OUR_SRCS}
	gcc ${OUR_SRCS} -o ${OURS} \
		${CFLAGS} ${LIBFLAGS}

${GEN}: ${GEN_SRCS}
	g++ ${GEN_SRCS} -o ${GEN} \
		${CFLAGS} ${LIBFLAGS}

all: ${YOURS} ${OURS} ${GEN}

clean:
	rm -f ${OURS} ${YOURS} ${GEN}

re: clean all


.PHONY: all re clean \
	yours ours \
