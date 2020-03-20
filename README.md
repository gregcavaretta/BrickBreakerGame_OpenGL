# BrickBreakerGame_OpenGL

This is a simple brick breaker game using OpenGL (FreeGLUT) in C++. 

![image](https://i.ibb.co/b2whrHg/photo-image-design-200108.png)

This program uses 3 classes. There was no need to break this up into multiple class/header files.

Brick class 
    - Coordinates of each brick to be drawn
    - A value (Score) of each brick 
 
Ball class 
    - Size of the ball
    - Direction of where the ball is moving
    - Bounds of the ball (Used for hit colliders with other objects)
    
Game class
    - Vector of Ball class (Used for N number of balls)
    - Vector of Brick class (Used for N number of bricks)
    - Current score
    - Current lives
    - Min/Max of bricks to be rendered


Last update was Dec 10, 2019. 
