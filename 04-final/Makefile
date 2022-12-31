APPNAME = fractal
OBJECTS = fractal.cxx

DEBUG = -g

fractal : $(OBJECTS)
	g++ -o $(APPNAME) $(OBJECTS) `sdl-config --cflags --libs`

clean :
	rm -f $(OBJECTS)
	rm -f $(APPNAME)
	rm -f *~
