# Final Fractal Application

Interactive tool for exploring and rendering fractal images, specifically the Mandelbrot set.

## Features

This final version of the fractal rendering program includes substantial improvements:

- **Threaded Rendering**: One of the most significant updates in this version of the program is the incorporation of multi-threaded rendering. The program creates multiple threads, with each rendering a part of the image. This speeds up the rendering process by utilizing multiple cores of the CPU.

- **Mutex Implementation**: A pthread mutex is added to ensure that the image rendering process is thread-safe. The mutex locks and unlocks around the SDL function to prevent simultaneous access and potential data race conditions.

- **User-Defined Thread Count**: The program now accepts the number of threads to use for rendering as a command-line argument. If no value is provided, it defaults to using 2 threads.

- **Thread-Specific Structs**: A struct is defined for each thread to hold its specific rendering information. The struct contains details such as the starting point, width, height, zoom level, minimum x and y values, maximum iterations, and anti-aliasing.

- **Image Rendering Per Thread**: The RenderImage function now accepts a void pointer argument, which is cast to a pointer. This function uses the data in the struct to render the specific part of the image assigned to each thread. After rendering, it updates the corresponding area on the screen.

- **Program Flow Improvements**: The main function has been significantly updated to handle multi-threaded rendering. It now divides the screen among the threads and initializes the structs for each of them. It then creates the threads, starts them with the RenderImage function, and waits for all threads to complete their tasks.

- **Dynamic Width Assignment**: The program now dynamically calculates the width each thread should render based on the total number of threads.

> The final version of the fractal rendering program effectively implements multi-threaded rendering to make full use of multiple CPU cores and thereby significantly improves the rendering speed. It also ensures thread-safety by using a mutex to prevent simultaneous access to shared data.
