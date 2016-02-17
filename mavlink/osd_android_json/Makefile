CPPFLAGS+= -I/usr/include -I ../../c_library/common/
LDFLAGS+= -L/usr/lib  -lz
LDFLAGS+=  -lpthread -lrt -lm

all: mavlink_json_osd

%.o: %.c
	gcc -c -o $@ $< $(CPPFLAGS)

mavlink_json_osd: main.o mavlink_parse.o render.o telemetry.o
	gcc -o $@ $^ $(LDFLAGS)

clean:
	rm -f mavlink_json_osd *.o *~
