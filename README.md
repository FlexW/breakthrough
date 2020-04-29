# Breakout
## Description
This my version of the Breakout game tutorial from
[LearnOpenGL](https://learnopengl.com/In-Practice/2D-Game/Breakout).
I used a slightly different architecture and used OpenAl for Audio
instead of IrrKlang.

## Features
- 2D Sprite Renderer
- Particle System
- Post-Processing effects
- Collision Detection
- Audio

##  Build
```
mkdir build
cd build
cmake -G Ninja ..
ninja
```
## Play
```
cd build
./app/breakoutgl
```
