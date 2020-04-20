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

Hours spent : 37h
It's the framework of the Octree and BSP tree. Moreover, there are several toggle GUI to see the different types of tree. You can press the toggle buttons of each type of tree, and you can turn it off if you press the button again.
I highly recommend to run this application with release mode. It will take some time to run the application because of many calculations of BSP tree. To save the time, I set 3 teapots at first, and you can add the object using Add Object button.
You can move the camera using W, A, S, D and move the view direction using Up, Right, Left, Down direction keys. Also. you should move the up, down, right and left keyboard to rotate the camera view.

-Done
** All things about previous assignment **
** Create Adaptive Octree using triangles **
** Octree terminates if nodes have less than 300 triangles **
** Create BSP Tree using triangles **
** BSP Tree terminates if leaf nodes have less than 300 triangles or over max depth which is 10 **
** Display different colors per triangles in Octree and BSP tree **
** Generate object randomly using GUI toggle **

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
- Build adaptive Octree (4h)
- Color partitioning according to triangle (3h)
- Build BSP tree (4h)
- Color partitioning according to leaf nodes (6h)
- Some optimization to minimize compile time (2h)
- Add object randomly with random position (3h)
- Make GUI to toggle easily (3h)

I couldn't do the 'Bottom-up construction' in Bounding Volume hierarchy. I spend too much time to build the other stuff, so I could not implement it.