APPNAME = fractal
OBJECTS = fractal.o

SDL_INC = -I/usr/local/include/SDL
INCLUDES =  $(SDL_INC) -I/usr/local/include

LIBS = -lpthread -ldl -L/usr/lib -lz -lSDL -L/usr/X11R6/lib -lX11 -lXext

DEBUG = -g

fractal : $(OBJECTS)
	g++ -o $(APPNAME) $(OBJECTS) $(LIBS)

fractal.o : fractal.cxx
	g++ -c $(DEBUG) $(INCLUDES) fractal.cxx  

clean :
	rm -f $(OBJECTS)
	rm -f $(APPNAME)
	rm -f *~
