# Cidr
Cidr (pronounced "Sidr" which stands for "Simple Drawer" or "Simple Drawing") is (supposed to be) a simple library which you can use to easily draw simple stuff (simple geometric shapes such as lines squares etc, bitmaps and other simple stuff). All it requires is a simple pixel color buffer.

#### Cidr has following features
 * Lines (+ anti aliasing)
 * Circles (+ anti aliasing)
 * Rectangles 
 * Triangles 
 * Shaders for all shapes 
 * Bitmaps
 * Triangles with interpolated color 
 * Textured triangles
 * and much more™ 
 * Making it header only

Here is a small demo of what you can already do with Cidr
![Cidr application demo](https://i.imgur.com/Wtnibid.png)

## Building the demo (Mac only for now)
In order to build the example shown in the image just get all of the github files, and then run:
```bash
$ mkdir build
$ make
Compiling...
clang++ -std=c++17 -O2 -pipe -Wall -I include -L lib -l SDL2-2.0.0 -lm main.cpp -o build/Cidr
Compilation finished successfully
$ ./build/Cidr
```
You can pass an integer parameter to the program which will toggle the zoom.
For example this will launch the program with 2x zoom:
```bash
$ ./build/Cidr 2
```
In order to switch between shaders, you can press the number keys between 1 and 4 (other number keys will disable the shader).
With the arrow keys you can move the source location of the image and the textured triangles. By pressing keys from 5-8 you switch between "OutOfBound" modes and keys 9 and 0 switch between linear and bilinear interpolation.

## Usage
Just copy and paste the `cidr.hpp` file into your project and you're done!

## Dependencies
All you need is a compiler that supports C++17.

## License 
This project is licensed under the [MIT License](https://www.tldrlegal.com/l/mit/)
