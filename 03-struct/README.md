# Final Fractal Application

Interactive tool for exploring and rendering fractal images, specifically the Mandelbrot set.

## Features

The third improvement of the code includes an additional layer of abstraction and organization through the use of a structure to package the rendering data together for easier management and readability. The data it contains includes screen details, pixel ranges, zoom, minimum X and Y values, the maximum number of iterations, and anti-aliasing details.

Here are the key changes:

- **Struct Introduction**: This new structure has been created to hold all data required for rendering an image. It has fields for the screen, X and Y coordinates, width, height, zoom level, minimum X and Y values, maximum iterations, and anti-aliasing.

- **Image Render Modification**: The function now takes a pointer to the struct as an argument instead of multiple individual parameters. This change improves the readability and maintainability of the code.

- **Thread Optimization**: In the creation of threads within the main function, each thread now receives a struct containing all the data it needs to perform its task. This data is packaged into an array of structs, allowing each thread to handle its portion of the image rendering task.

- **Flow Modification**: The main function has been updated to initialize and fill an array of structs before passing them to the threads. This way, the required data is conveniently packaged and easily accessible by each thread during execution.

- **Error Checking**: Instructions for proper usage have been added in the main function for better user guidance.

> The third version of the code improves upon the first by making it more organized and easier to maintain and understand.
