#include <math.h>
#include <iostream>
#include <SDL/SDL.h>
#include <pthread.h>

using namespace std;

const float max_square = 3.0;

/*
	@fhildeb1
	
	Da wir als Input nur eine Datentyplose Speicheraddresse
	uebergeben, müssen wir alle benötigten Daten zum Rendern
	in ein Struct packen. Dieses können wir dann als ein
	kompaktes Element Speichern und via Pointer darauf verweisen.
*/
struct renderdaten{
	SDL_Surface* screen; 
	int x0;
	int y0;
	int width; 
	int height;
	float zoom; 
	float min_x;
	float min_y; 
	int max_iterations; 
	int anti_aliasing;
};


inline double sqr(double v)
{
  return v*v;
} // sqr

inline void SetPixel(SDL_Surface* screen, 
		     short x, short y, 
		     unsigned char red, unsigned char green, unsigned char blue)
{
  unsigned char* pixel = &(((unsigned char*)(screen->pixels))[x*3+y*screen->pitch+0]);

  *pixel++ = red;
  *pixel++ = green;
  *pixel = blue;
} // SetPixel

int HSVToRGB(float h, float s, float v, float& r, float& g, float& b)
{
  int i;
  float f, p, q, t;
  float red[2], green[2], blue[2];
  
  if(s == 0) {
      r = v; g = v; b = v;
      return 0;
  } // if
  else {
      if(h == 360) h = 0;
      
      i = (int)h/60;
      f = ((int)h % 60)/60.0;
      
      switch(i)	{
      case 0:
	red[0] = 255;
	green[0] = 0;
	blue[0] = 0;
	
	red[1] = 255;
	green[1] = 255;
	blue[1] = 0;
	break;
	
      case 1:
	red[0] = 255;
	green[0] = 255;
	blue[0] = 0;
	
	red[1] = 0;
	green[1] = 255;
	blue[1] = 0;
	break;
	
      case 2:
	red[0] = 0;
	green[0] = 255;
	blue[0] = 0;

	red[1] = 0;
	green[1] = 255;
	blue[1] = 255;
	break;
	
      case 3:
	red[0] = 0;
	green[0] = 255;
	blue[0] = 255;

	red[1] = 0;
	green[1] = 0;
	blue[1] = 255;
	break;
	
      case 4:
	red[0] = 0;
	green[0] = 0;
	blue[0] = 255;

	red[1] = 255;
	green[1] = 0;
	blue[1] = 255;
	break;
	
      case 5:
	red[0] = 255;
	green[0] = 0;
	blue[0] = 255;

	red[1] = 255;
	green[1] = 0;
	blue[1] = 0;
	break;
      } // case

      r = f*red[1] + (1-f)*red[0];
      g = f*green[1] + (1-f)*green[0];
      b = f*blue[1] + (1-f)*blue[0];

      r = s*r + (1-s)*255;
      g = s*g + (1-s)*255;
      b = s*b + (1-s)*255;

      r *= v;
      g *= v;
      b *= v;

      return 0;
    }		
} // HSVToRGB

int calc_gray(float cx, float cy, int max_iter)
{
  float square = 0;
  int iter = 0;

  float x, y, xt, yt;

  x = 0;
  y = 0;

  while((square <= max_square) && (iter < max_iter)) {

    xt = x*x - y*y + cx;
    yt = 2*x*y + cy;

    x = xt;
    y = yt;

    square = x*x + y*y;

    iter = iter + 1;
  } // while

  return iter;
}

void* RenderImage(void* renderinput){
	/*
		@fhildeb1
		Hier wird die uebergebene Speicheradresse 
		uebergeben. Aus dem Datentyp void-Pointer
		wird jetzt ein renderdaten-Pointer welcher
		die Objekte des Structs zeigt.
		
		input->zoom ist äquivalent zu (*input).zoom
		
		Erläuterung: Da input ein Zeiger und keine Variable
		ist, können wir nicht mit input.zoom darauf zugreifen sondern
		müssen mittels eines Pointers auf den Pointer "input" den
		Wert herauslesen.
	*/
	renderdaten* input = (renderdaten*) renderinput; 
  const float factor = input->zoom/min((float)input->screen->w, (float)input->screen->h);

  for(float y = input->y0; y < input->y0+input->height; y++) {    
    for(float x = input->x0; x < input->x0+input->width; x++) {

      float hue = 0.0;

      for(float dy = -input->anti_aliasing; dy <= input->anti_aliasing; dy++) {
	for(float dx = -input->anti_aliasing; dx <= input->anti_aliasing; dx++) {

	  float cx;
	  float cy;
	  
	  cy = input->min_y + (y+(dy+(float)input->anti_aliasing)/(2.0*(float)input->anti_aliasing+1))*factor;
	  cx = input->min_x + (x+(dx+(float)input->anti_aliasing)/(2.0*(float)input->anti_aliasing+1))*factor;
	  
	  hue += 360*log(calc_gray(cx, cy, input->max_iterations))/log(input->max_iterations);
	  
	} // for	    
      } // for
      
      hue /= sqr(2.0*input->anti_aliasing+1);

      float r, g, b;
      
      float value = 1.0;
      
      if(hue > 359) value = 0.0;
      
      HSVToRGB(hue, 1.0, value, r, g, b);

      // b = g = r = hue*255/360;
      
      SetPixel(input->screen, x, y, r, g, b);
	  
    } // for	    
  } // for
} // RenderImage
  
int main(int argc, char** argv)
{
	/*@fhildeb Thread anlegen*/
	pthread_t fractalThread;
   SDL_Surface *screen;
   SDL_Event   event;
	/*@fhildeb Struct anlegen*/
	renderdaten uebergabe;	
	
    int max_iterations = 100;
    int anti_aliasing = 0;

    float zoom_alt;
    float zoom = 2.0;
    float min_x = -1.5;
    float min_y = -1.0;

    if(argc < 3) {
      cout << "USAGE: fractal WIDTH HEIGHT" << endl << endl;
      cout << "move:                    cursor keys" << endl;
      cout << "zoom:                    page up/down" << endl;
      cout << "anti-aliasing no/off:    return" << endl;
      cout << "in-/decrease iterations: +/-" << endl; 
	  cout << "quit application: 		Q" << endl << endl;
      return -1;
    } // if

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
	cerr << "Could not initialize SDL video" << endl;
	exit(-1);
    } // if

    int WIDTH = atoi(argv[1]);
    int HEIGHT = atoi(argv[2]);

    screen = SDL_SetVideoMode(WIDTH, HEIGHT, 24, SDL_SWSURFACE);

    if(!screen) {
	cerr << argv[0] << "could not SetVideoMode() - abort" << endl;
	exit(-1);
    } // if

    bool redraw = true;

    for(;;) {

      SDL_WaitEvent(&event);

      switch(event.type) {
	
      case SDL_MOUSEMOTION:
	break;
	
      case SDL_MOUSEBUTTONDOWN:
	break;
	
      case SDL_MOUSEBUTTONUP:
	break;
	
      case SDL_KEYDOWN:
	
	switch(event.key.keysym.sym) {

	case 13: // return-key
	  if(anti_aliasing == 0) anti_aliasing = 1;
	  else anti_aliasing = 0;
	  redraw = true;
	  break;

	case '+':
	  max_iterations += 500;
	  redraw = true;
	  break;

	case '-':
	  if(max_iterations > 500)
	    max_iterations -= 500;
	  redraw = true;
	  break;

	case 281: // page-key
	  zoom /= 1.2;
          min_x += zoom*0.1;
	  min_y += zoom*0.1;	  
	  redraw = true;
	  break;

	case 280: // page-key
	  zoom *= 1.2;
	  min_x -= zoom*0.1;
	  min_y -= zoom*0.1;	  
	  redraw = true;
	  break;

	case 274: // cursor-key up
	  min_y += 0.1*zoom;
	  redraw = true;
	  break;

	case 273: // cursor-key down
	  min_y -= 0.1*zoom;
	  redraw = true;
	  break;

	case 275: // cursor-key right
	  min_x += 0.1*zoom;
	  redraw = true;
	  break;

	case 276: // cursor-key left
	  min_x -= 0.1*zoom;
	  redraw = true;
	  break;

	case 27: // ESC
	case 113: // q
	  goto exit_app;
	  break;
	  
	default:
	  break;
	} // switch
      } // switch

      if(redraw == true) {

	/*@fhildeb1 Alle ermittelten Werte dem Struct zuweisen*/
	uebergabe.screen = screen;
	uebergabe.x0 = 0;
	uebergabe.y0 = 0;
	uebergabe.width = WIDTH;
	uebergabe.height = HEIGHT;
			
	uebergabe.zoom = zoom;
	uebergabe.min_x = min_x;
	uebergabe.min_y = min_y;
	uebergabe.max_iterations = max_iterations;
	uebergabe.anti_aliasing = anti_aliasing;
	
	// Erläuterung: screen:    Datenstruktur mit Fenster-Infos
	//              screen->w: Breite des Fensters
	//              screen->h: Höhe des Fensters
	
	//              zoom:      Zoom-Faktor in das Fraktal
	//              min_x/y:   Linker oberer Rand des Fraktals
	//              max_iterations: Maximale Anzahl Folgeglieder der Reihe.
	//                              Wenn die Elemente danach nicht unendlich sind,
	//                              hat die Reihe konvergiert
	//              anti-aliasing: 0 = keines, 1 = 2x2 Subpixel, 2 = 3x3 Subpixel, ...
	

	
	/*@fhildeb Adresse des Structs übergeben und Funktion in einem
	eigenen Thread aufrufen und ausführen. Dannach warten bis der
	Thread fertig berechnet hat*/
	pthread_create(&fractalThread,NULL,RenderImage,&uebergabe);
	pthread_join(fractalThread,NULL);
	
	// Fenster neu zeichnen, von linkem oberen Punkt (hier 0,0) der Breite screen->w, der Höhe screen->h
	SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
	
	redraw = false;
      } // if

    } // for

 exit_app:

    SDL_FreeSurface(screen);
} // main
