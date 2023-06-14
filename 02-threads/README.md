# Final Fractal Application

Interactive tool for exploring and rendering fractal images, specifically the Mandelbrot set.

## Features

This program is an enhanced version with several additional features and refinements compared to the initial code base:

- **Thread Implementation**: The most noticeable change is the introduction of multithreading. The rendering function RenderImage is now designed to be called in its own thread. This could potentially speed up the rendering process by utilizing multiple cores of the CPU simultaneously.

- **Struct Implementation**: The program introduces a struct, which is a custom data structure used to store all the necessary data for rendering the fractal. This includes the SDL surface to draw on, dimensions of the area to be drawn, zoom level, coordinates of the minimum x and y, maximum iterations for the fractal calculation, and level of anti-aliasing. The struct allows for a more organized way to pass multiple related data to the RenderImage function.

- **Anti-Aliasing**: The program now has the ability to perform anti-aliasing, which is a technique used to smooth jagged edges in digital images. The level of anti-aliasing can be toggled on or off by pressing the return key.

- **Controls**: The fractal can now be navigated and manipulated more interactively. It can be zoomed in and out using the page up and page down keys respectively. The cursor keys can be used to move the fractal around. The '+' and '-' keys can be used to increase or decrease the maximum number of iterations respectively. The number of iterations controls the level of detail in the fractal, with a higher number producing more detail.

- **Error Handling**: The program has improved error handling. If the SDL video cannot be initialized, or if the video mode cannot be set, the program will output an error message and exit.

Overall, these changes make the program more efficient, more interactive, and capable of producing higher quality fractal images.
