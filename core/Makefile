TARGET  := redking.a

SRCS :=   	src/model/range_spedup_final.cpp \
			src/model/model.cpp \
			src/synth/additive_synth.cpp

CCFLAGS = -g -O3 -ffast-math -march=native -mtune=native -fPIC -Isrc -g -O2
LDFLAGS = 
LIBS    = -ljellyfish -lsndfile -lportaudio -lm 
PYTHON_INCLUDE = -I/usr/include/python2.7/

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
	$(AR) $(ARFLAGS) ${TARGET} ${OBJS} ${COBJS}
	swig -c++ -python swig/redking.i
	g++ ${CCFLAGS} -c swig/redking_wrap.cxx ${PYTHON_INCLUDE}
	g++ -shared ${OBJS} redking_wrap.o ${LIBS} -o swig/_redking.so
	cp swig/_redking.so ../python
	cp swig/redking.py ../python
	#	g++ -shared ${OBJS} redking_wrap.o ${LIBS} -o redking.so

${OBJS}: %.o: %.cpp %.dep
	${CC} ${CCFLAGS} -o $@ -c $<

${DEPS}: %.dep: %.cpp Makefile
	${CC} ${CCFLAGS} -MM $< > $@

clean:: cleandeps
	-rm -f *~ src/*.o src/*/*.o ${TARGET}

cleandeps::
	-rm -f src/*.dep src/*/*.dep

distclean:: clean
	-rm -rf config.status autom4te.cache config.log Makefile
