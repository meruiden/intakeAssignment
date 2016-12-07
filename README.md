# intakeAssignment

I am making this for my intake assignment.

How to build:<br>

Windows: <br>
	  
	  Open the intakeAssignment.sln in the vs2015 folder.<br>
	  You dont need to install anything at all. Everything is pre linked and included
	  
Unix: <br>

make sure you have installed the following libraries: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;GLEW        http://glew.sourceforge.net/ <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SDL2        https://www.libsdl.org/download-2.0.php <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SDL2_ttf    https://www.libsdl.org/projects/SDL_ttf/ <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SDL2_image  https://www.libsdl.org/projects/SDL_image/ <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SDL2_mixer  https://www.libsdl.org/projects/SDL_mixer/ <br>
	  
	  cd cmake
      mkdir -p build
      cd build
      cmake -Wno-dev ..
      make
      ./project
<br><br>
Copyright (c) Corné Steenhuis. All Rights Reserved.
