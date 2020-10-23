YOUR_SRCS =	mini_paint.c
OUR_SRCS  =	our_mini_paint.c
GEN_SRCS  =	generate_example.cpp

OURS  =	our_mini_paint
YOURS =	mini_paint
GEN   =	gen

CFLAGS = -Wall -Wextra -Werror
LIBFLAGS = -lm

${YOURS}: ${YOUR_SRCS}
	gcc ${YOUR_SRCS} -o ${YOURS} \
		${CFLAGS} ${LIBFLAGS}

${OURS}: ${OUR_SRCS}
	gcc ${OUR_SRCS} -o ${OURS} \
		${CFLAGS} ${LIBFLAGS}

${GEN}: ${GEN_SRCS}
	g++ ${GEN_SRCS} -o ${GEN} \
		${CFLAGS}

all: ${YOURS} ${OURS} ${GEN}

clean:
	rm -f ${OURS} ${YOURS} ${GEN}

re: clean all


.PHONY: all re clean