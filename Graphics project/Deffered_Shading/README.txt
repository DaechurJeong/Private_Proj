/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: README.txt
Purpose: Please read it before execute project.
Language: MSVC C++
Platform: VS2017, Windows
Project: jungdae.chur_CS300_1
Author: Charlie Jung, jungdae.chur
Creation date: 01/31/2019
End Header --------------------------------------------------------*/

Hours spent : 20h
Using gbuffer and deferred shading, the final color of several objects are calculated the  with lighting. Moreover, it checks the light volume to optimize the calculations.
You can move the camera using W, A, S, D and move the view direction using Up, Right, Left, Down direction keys. Also. you should move the up, down, right and left keyboard to rotate the camera view.

-Done
** Implement the Deferred Shading pipeline for rendering objects **
** Generate 4 G-buffers that contains position, normal, specular, diffuse **
** Draw objects on G-buffer, then light pass. Then draw the quad on G-buffer **
** Light datas are passed through uniform blocks **
** Make toggle using GUI to see 4 different G-buffer **
** Implement Light Volume optimization using light spheres **
** Change the movement of light spheres **
** Adjust the new light calculation with radius to Dir, Point, Spot lights **

About this application
I worked in Visual Studio 2017, and the project name is cs300_jungdae.chur_assg1. I started from the project 2 in CS300, so I couldn't change the name of project.
So in this directory, you can find cs300_jungdae.chur_assg1.sln file. If you run it, basically it is set as Release mode. Also you can execute it Ctrl+F5 or right side arrow button. It will build the project.
I tested this application works in Edison line Y.

There are Source, ShaderCodes, models, lib, include folders and imgui.ini file in project folder. These are the compiling files below.
Camera.h      Render.h
Light.h       Shader.h
Camera.cpp    Render.cpp
main.cpp      Shader.cpp
imgui files & glad.c

The hours I spent
- Refactoring the project from CS300 (4h)
- Implement G-buffer to render (10h)
- Implement Deferred shading for objects (4h)
- Make GUI to toggle easily (2h)

All parts are completed, also extra credit, and it was really helpful for me to learn deferred shading and G-buffer.