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
Creation date: 10/16/2018
End Header --------------------------------------------------------*/

Hours spent : 40h
There are 3 scenarios. First scenario has same colors and light types for each light spheres, Second scenario has different colors with same light types for each light spheres. Last, third scenario has different colors and light types.
You can select the scenarios you want to see, and also light types, shading types, texture projection types, move/stop spheres, add/remove spheres, light controllers (for each spheres), reloading shaders, and also the things from assignment 1 (load obj, normals).
Also, there is a texture which is loaded from ppm file.
You can move the camera using W, A, S, D and move the view direction using Up, Right, Left, Down direction keys.


-Done
** Phong Lighting, Phong Shading, Blinn Shading **
** Multiple light sources, change the number of lights in run-time **
** Only diffuse color for the light source **
** Point light, Directional light, Spotlight **
** Using uniform block for light informations **
** Read PPM file and use it as material diffuse, specular values **
** Texture Entity : toggle between vertex position & vertex normal to draw texture **
** Set uniform block the light information variables **
** Select specific lights and change number of active lights **
** Select light types in GUI **
** Toggle to move/stop the light rotation **
** Toggle between scenarios **
** multiple texture projection (planar, cylindrical, spherical) **
** Use 2* dot(N, L) * N - L formula rather than reflect function (All formulas are came from cs300 lecture)**

When I tried to map the texture in CPU, some parts are broken, especially when y = 0, it seems like the line cuts the object. Even when I tried to map with normal in CPU, this cutting line is curved a little bit. But when I calculate it in shader, it is really smooth.

About this application
I worked in Visual Studio 2017, and the project name is cs300_jungdae.chur_assg1. So in this directory, you can find cs300_jungdae.chur_assg1.sln file. If you run it, basically it is set as Release mode. Also you can execute it Ctrl+F5 or right side arrow button.
It will build the project.
I tested this application works in Edison line Y.

There are Source, ShaderCodes, models, lib, include folders and imgui.ini file in project folder. These are the compiling files below.
Camera.h      Render.h
Light.h       Shader.h
Camera.cpp    Render.cpp
main.cpp      Shader.cpp
imgui files & glad.c

The hours I spent
- Types of light (5h)
- Types of shading (4h)
- loading ppm (3h)
- Texture projecting (15h)
- Multiple lights (5h)
- Designing Scenario & GUI (5h)
- Uniforming the variables of light information (3h)

All parts are completed, and it was really helpful for me to learn lighting & texturing programming.