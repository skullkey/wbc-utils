

VGFONTPATH=/opt/vc/src/hello_pi/libs/vgfont
HELLOFONTPATH=/opt/vc/src/hello_pi/hello_font/

CPPFLAGS+= -I/usr/include -I ../c_library/common/ -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I$(VGFONTPATH)
LDFLAGS+= -L/usr/lib  -L$(VGFONTPATH) -lvgfont -lfreetype -lz
LDFLAGS+=-L/opt/vc/lib/ -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lm -lshapes

all: vgfont mavlink_omx_osd_SBS

vgfont:
	cp $(HELLOFONTPATH)/Vera.ttf .
	make -C $(VGFONTPATH)



%.o: %.c
	gcc -c -o $@ $< $(CPPFLAGS)

mavlink_omx_osd_SBS: main.o mavlink_parse.o render.o
	gcc -o $@ $^ $(LDFLAGS)


clean:
	rm -f mavlink_omx_osd *.o *~


