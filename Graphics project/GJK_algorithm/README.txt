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
Creation date: 02/24/2019
End Header --------------------------------------------------------*/

Hours spent : 20h
It's the framework of the GJK algorithm. Moreover, there are several toggle GUI to see the different types of tree. You can press the toggle buttons of each type of tree, and you can turn it off if you press the button again.
If you press space bar, the sphere shoot to the look-at vector and when it collide with leaf node of each tree, it will change the color. To distinguish with Top-down and Bottom-up, I changed color to red when the top-down tree activated and blue for bottom-up.
For Octree, it also changes color to red.
I highly recommend to run this application with release mode. It will take some time to run the application because of many calculations.
You can move the camera using W, A, S, D and move the view direction using Up, Right, Left, Down direction keys. Also. you should move the up, down, right and left keyboard to rotate the camera view.

-Done
** GJK algorithm for Top-down tree and Bottom-up tree **
** GJK algorithm for Octree **
** Change color of bullet and leaf node when it collide with leaf node **
** Generate bullet object **

About this application
I worked in Visual Studio 2017, and the project name is cs300_jungdae.chur_assg1. I started from the last assignment project in CS350.
In this directory, you can find cs300_jungdae.chur_assg1.sln file. If you run it, basically it is set as Release mode. Also you can execute it Ctrl+F5 or right side arrow button. It will build the project. I HIGHLY RECOMMEND TO RUN WITH RELEASE MODE instead of debug mode.
Because, it has lots of calculations, so some algorithms drop down the frame per seconds.
I tested this application works in Edison line Y.

There are Source, ShaderCodes, models, lib, include folders and imgui.ini file in project folder. These are the compiling files below.
Camera.h      Render.h
Light.h       Shader.h
Camera.cpp    Render.cpp
main.cpp      Shader.cpp
imgui files & glad.c

The hours I spent
- Basic GJK algorithm (10h)
- Generate bullet object (1h)
- GJK algorithm for Top-down tree (1h)
- GJK algorithm for Bottom-up tree (1h)
- GJK algorithm for Octree (3h)
- Changing color of bullet and leaf node (3h)
- Connect whole thing with GUI (1h)

I couldn't do the 'Bottom-up construction' in Bounding Volume hierarchy. I spend too much time to build the other stuff, so I could not implement it.