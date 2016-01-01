CC=gcc
CXX=g++
RM=rm -f

CPPFLAGS+= -I/usr/include -I ../c_library/common

SRCS=main.cc NazaDecoderLib.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: nazareader

nazareader: $(OBJS)
	$(CXX) -o nazareader $(OBJS)

nazareader.o: main.cc NazaDecoderLib.cc

NazaDecoderLib.o: NazaDecoderLib.h NazaDecoderLib.cc

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) nazareader
