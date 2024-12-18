# Monte Carlo Simulator for the estimation of PI
## Installation (Windows) - 
- Clone the repo
- Download Raylib Library
- Open main.cpp
- Press ```Ctrl + Shift + B``` or Run Build task and an executable will be created with the name of main.exe in file explorer (```Alt + Shift + R```) 
- Run that by double clicking

or

- Run this commmand
```bash
g++ -o main *.cpp -Wall -std=c++14 -D_DEFAULT_SOURCE -Wno-missing-braces -g -O0 C:/raylib/raylib/src/raylib.rc.data -I. -IC:/raylib/raylib/src -IC:/raylib/raylib/src/external -L. -LC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP && ./main
```
### Benchmarks - 

```bash
Total Dots: 27815
Dots Inside Circle: 21789
Dots Outside Circle: 6026
Estimated Pi: 3.13342
Error:0.260238%
--------------------------------------------
Most Accurate PI during Simulation was 3.14159
With Error Percentage of 7.5891e-06%
```
After almost 27000 random dots and continuosly fluctuating accuracy, the most accurate this program was 3.14159 which has only 0.000007% error