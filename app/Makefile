TARGET  := redking
TARGET_LIB := redking.a

SRCS :=   	src/model/range_spedup_final.cpp \
			src/model/model.cpp \
			src/synth/additive_synth.cpp \
	 		src/main.cpp

CCFLAGS = -g -O3 -ffast-math -march=native -mtune=native -Isrc -g -O2
LDFLAGS = 
LIBS    = -ljellyfish -lsndfile -lportaudio -lm 

CC = g++
AR = ar
OBJS      := ${SRCS:.cpp=.o}
DEPS      := ${SRCS:.cpp=.dep}
XDEPS     := $(wildcard ${DEPS})
.PHONY: all clean distclean
all:: ${TARGET}

ifneq (${XDEPS},)
include ${XDEPS}
endif

${TARGET}: ${OBJS} ${COBJS}
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS}

${OBJS}: %.o: %.cpp %.dep
	${CC} ${CCFLAGS} -o $@ -c $<

${DEPS}: %.dep: %.cpp Makefile
	${CC} ${CCFLAGS} -MM $< > $@

library: ${OBJS} ${COBJS}
	$(AR) $(ARFLAGS) ${TARGET_LIB} ${OBJS} ${COBJS}

clean:: cleandeps
	-rm -f *~ src/*.o src/*/*.o ${TARGET}

cleandeps::
	-rm -f src/*.dep src/*/*.dep

distclean:: clean
	-rm -rf config.status autom4te.cache config.log Makefile
