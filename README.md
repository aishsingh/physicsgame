Physics Game  [![Build Status](https://travis-ci.org/aishsingh/physicsgame.svg?branch=master)](https://travis-ci.org/aishsingh/physicsgame) [![Coverage Status](https://coveralls.io/repos/github/aishsingh/physicsgame/badge.svg?branch=master)](https://coveralls.io/github/aishsingh/physicsgame?branch=master) [![Licence](https://img.shields.io/badge/licence-GPLv3-blue.svg)](LICENSE)
============
A poorly named game that will soon become a fully functional android app.  

![alt tag](http://i.imgur.com/ozDV9rD.gif "Gameplay preview")  

## Aim
The aim of this project was to experiment and develop a physics engine entirely from scratch.  
The project covers the following areas:  

* Objects inherit motion attributes (velocity, acceleration, etc)  
* Objects are effected by gravity from all angles  
* Polygon collision detection  

I also wanted to experience Graphical Programming with the use of OpenGL 2 and its rendering pipeline.  

## Frameworks
__AndroidNDK:__ Call native C++ through JNI  
__OpenGL ES 2:__ Surface rendering (Bare-bones version for mobile devices)  
__GLM:__ Provide OpenGL math functions  
__libzip:__ Extract APK to make assets readable natively  
__libpng:__ Read image assets natively  
__googletest:__ Unit testing natively
