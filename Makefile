
INCLUDEDIR=/usr/include/
LIBDIR=/usr/lib

SOURCE= scene.cpp sphere.cpp vector.cpp trace.cpp raycast.cpp include/InitShader.cpp

CC= g++

CFLAGS= -O3 -g -Wall -pedantic -DGL_GLEXT_PROTOTYPES

EXECUTABLE= rayT

LDFLAGS = -lGL -lglut -lGLEW -lXext -lX11 -lm

INCLUDEFLAG= -I. -I$(INCLUDEDIR) -Iinclude/
LIBFLAG= -L$(LIBDIR)

OBJECT= $(SOURCE:.cpp=.o)

all: $(OBJECT) depend
	$(CC) $(CFLAGS) $(INCLUDEFLAG) $(LIBFLAG)  $(OBJECT) -o $(EXECUTABLE) $(LDFLAGS)

depend:
	$(CC) -M $(SOURCE) > depend

$(OBJECT):
	$(CC) $(CFLAGS) $(INCLUDEFLAG) -c -o $@ $(@:.o=.cpp)

clean_object:
	rm -f $(OBJECT)

clean:
	rm -f $(OBJECT) depend $(EXECUTABLE)

include depend
