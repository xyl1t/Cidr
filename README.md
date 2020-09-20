# Cidr
Cidr (pronounced "Sidr" and stands for "Simple Drawer" or "Simple Drawing") is (supposed to be) a simple library with which you can easily draw elementary stuff (simple geometric shapes, lines, etc and sprites). All it requires is a simple pixel color buffer.

#### At the moment Cidr has the following core features:
 * Lines 
 * Anti-aliased lines
 * Circles
 * Anti-aliased circles
 * Rectangles
 * Shaders (at the moment only for rectangles)
 * and much more™

#### Some planned features include
 * Triangles 
 * Shaders for all shapes
 * Triangles with interpolated color
 * Font rendering 
 * Sprites
 * Textured triangles
 * Making it header only

Here is a small demo of what you can already do with Cidr
![Cidr application demo](https://imgur.com/K4kjS5C.jpg)

# Building the demo
In order to build the example shown in the image just get all of the github files, and then run:
```bash
$ make
clang++ -std=c++17 -I include -O2 -c cidr.cpp -MMD -MF build/cidr.d -o build/cidr.o
clang++ -std=c++17 -I include -O2 -c color.cpp -MMD -MF build/color.d -o build/color.o
clang++ -std=c++17 -I include -O2 -c main.cpp -MMD -MF build/main.d -o build/main.o
clang++ -std=c++17 -I include -O2 -c point.cpp -MMD -MF build/point.d -o build/point.o
clang++ -std=c++17 -I include -O2 -c tensorMath.cpp -MMD -MF build/tensorMath.d -o build/tensorMath.o
clang++ -std=c++17 -I include -O2 -L lib -l SDL2-2.0.0 -lm build/cidr.o build/color.o build/main.o build/point.o build/tensorMath.o -o build/Cidr
$ ./build/Cidr
```

# Usage
For now, you'll have to include all of the `.cpp` and `.hpp` (not including `lib/` or `include/`) and copy-paste them into your project. I'm looking forward to making Cidr a header only library near it's final release :)

# Dependencies
All you need is a compiler that supports C++17!

# License 
This project is licensed under the [MIT License](https://www.tldrlegal.com/l/mit/)
