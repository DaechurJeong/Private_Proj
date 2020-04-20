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
Creation date: 11/13/2018
End Header --------------------------------------------------------*/

Hours spent : 32h
This project is showing skybox with environment mapping and the scene is drawn by the frame buffer object. FBO save 6 sides of frames as texture and attach it to center object. Therefore, nearby objects can be reflected and refracted onto the center object. User can change the type which is reflect and refract, also both of them. Moreover, user can change the color of center object, and the number of around objects. You also can move the camera using W, A, S, D and move the view direction using Up, Right, Left, Down direction keys.

-Done
** Render 6 textures using FBO **
** Calculate reflection/refraction vectors **
** Environment mapping using reflection/refraction vectors **
** Create skybox **
** Put fraction materials in GUI from 0 to 100 **
** Design GUI to show as user wants **
** Mix the refraction and reflection **
** Add phong modeling (simple lights) **

About difficulties
At first, I was not really familiar with FBO, so understanding how FBO works takes a lot of times. but after recognize how to work FBO properly, I could explain about FBO to co-workers in GAM class.

About this application
I worked in Visual Studio 2017, and the project name is cs300_jungdae.chur_assg1. So in this directory, you can find cs300_jungdae.chur_assg1.sln file. If you run it, basically it is set as Release mode. Also you can execute it Ctrl+F5 or right side arrow button.
It will build the project. I tested this application works in Edison line Y.

There are Source, ShaderCodes, models, lib, include folders, .tga files and imgui.ini file in project folder. These are the compiling files below.
Camera.h      Camera.cpp
Shader.h      Shader.cpp
Render.h      Render.cpp
main.cpp	  Light.h
imgui files & glad.c 

The hours I spent
- Read PDF to know what to do, and re-read ppt from lectures (2h)
- Rendering skybox (4h)
- Using FBO (8h)
- Refactor from previous assignment (1h) (remove un-used one)
- Calculate reflectoin vector (2h)
- Calculate refraction vector (4h)
- Mix reflection and refraction vector (1h)
- Match environment mapping smoothly (6h)
- Design GUI (2h)
- Add phong modeling (2h)

All parts are completed if I understand phong modeling properly, and it was really helpful for me to learn drawing using FBO, reflection/refraction.