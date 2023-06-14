# Final Fractal Application

Interactive tool for exploring and rendering fractal images, specifically the Mandelbrot set.

## Features

The initial code set is a basic implementation.

> It utilizes the Simple DirectMedia Layer (SDL) library to create a graphical window and display the rendered fractal images.

- **Fractal Calculation**: The program includes a function calc_gray for calculating the fractal. This function takes a point in the complex plane and the maximum number of iterations as inputs, and returns the number of iterations needed for the point to escape a circle of a certain radius. This result is used to determine the color of the corresponding pixel in the rendered image.

- **Coloring**: The HSVToRGB function converts hue, saturation, and value (HSV) to red, green, and blue (RGB). The SetPixel function then applies this color to a pixel on the SDL surface.

- **User Interaction**: The main function sets up an SDL window and enters a loop where it waits for user events such as keyboard inputs. It supports basic interactions such as zooming in and out, increasing or decreasing the maximum number of iterations, and quitting the application.

- **SDL Surface Management**: The program initializes the SDL video mode, creates an SDL surface for drawing, and frees this surface before exiting.

- **Simplicity**: The program is single-threaded and does not use advanced techniques such as anti-aliasing. It does not use data structures to organize the various parameters needed for rendering. Instead, these parameters are passed directly to the RenderImage function.

This initial version serves as a foundational base for further improvements and refinements in the fractal rendering process. Future enhancements might include features such as multi-threading, implementation of anti-aliasing, and introduction of more sophisticated data structures for managing rendering parameters.
