# ImageColorizer

Web application designed to colorize grayscale images using algorithm described in A. Levin D. Lischinski and Y. Weiss Colorization using Optimization. ACM Transactions on Graphics, Aug 2004 for coloring grayscale images.

Frontend and backend is written in pure C++20 with the help of Wt, OpenCV, Eigen3 and CSS.

Original algorithm is published [here](https://www.cs.huji.ac.il/w~yweiss/Colorization).\
Implementation used by this project is based on [this repository](https://github.com/lightalchemist/colorize-image).

User may colorize uploaded grayscale image by drawing colorful scribbles (as shown below). Algorithm then fills regions of the original image with similar intensity (brigthness) using the color of user's scribbles. We recommend using this application for rather small images, due to long processing times of larger ones. 

# Demonstration
|<img width="1440" alt="image1" src="https://github.com/TymoteuszPilarz/ImageColorizer/assets/122737837/ed6e9602-e887-4470-9050-9760580f3ea8">|
|:--:|
| <b>Fig. 1 Users Scribbles - Editor </b>|
|<img width="1440" alt="image2" src="https://github.com/TymoteuszPilarz/ImageColorizer/assets/122737837/f58f3340-30fd-4c29-87c0-3d7c862e10b4">|
| <b>Fig. 2 - Users Scribbles - Color picker </b>|
|<img width="1440" alt="image3" src="https://github.com/TymoteuszPilarz/ImageColorizer/assets/122737837/29d9da7a-12c4-4a86-b3c0-7bd37729cd28">|
| <b>Fig. 3 - Final result</b>|


# Building and running

Appropriate CMake files are already provided. To build the project, Wt, OpenCV and Eigen3 are requred to be installed first. After successfull compilation, the following command can be used to run the program:

--docroot <project_directory> --http-address 0.0.0.0 --http-port 8080

where <project_directory> needs to be replaced with the project path.

Image processing can take up to several dozen minutes depending on the image size and used setting, so it may be necessary to modify wt_config.xml. Global configuration is typically located at /etc/wt/ (check Wt documentation for more details).

```<timeout>seconds</timeout>``` - this property can be increased to prevent session from terminating while the image is still being processesed.

```<max-request-size>size</max-request-size>``` - max request size, wchich can be altered in order to upload larger images (e.g. 4k resolution)
