# KronoEngine
C++ OpenGL Renderer

This is an adaptation of the renderer i developed during my last college year.

A lot of code could be improved, for the moment remains almost as in May 2015. The only important change is replace SFML, now it uses GLFW for the OpenGL context.

This was an engine programmed to satisfy the college video game requirements, it is not a generic engine.

Requirements
-----------------------
	* Visual Studio 2015
	* Visual C++ 2015

How to use the engine
-----------------------
1. Clone and open the Visual Studio solution.
2. Right click on KronoEngine project and set it as the StartUp project.
3. Build and run (only Debug / Release x86).
4. The Main.cpp file has a start example, feel free to change parameters.

How to use the OBJ to KM converter
-----------------------
1. Set the KronoMeshTool as the StartUp project.
2. Build it (only Debug / Release x86).
3. Copy the .OBJ files where KronoMeshTool.exe is.
4. Run the KronoMeshTool.exe and select if the meshes have to be centered or not.
5. The .KM meshes are copied in the output folder.

OBJ mesh export properties using Blender
-----------------------
* Selection Only
* Apply Modifiers
* Include UVs
* Triangulate Faces
* Forward: Z Forward