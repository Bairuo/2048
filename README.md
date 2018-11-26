## opengl-based  2048 Enhanced

[//]: # (Begin current test results)

A 2048 Enhanced Edition

![Screenshot](https://github.com/Bairuo/2048/raw/master/meta/game.jpg)

Develop: January 2018

### Dependencies
opengl,  glew, glfw, freetype, boost

If you are on windows, we have prepared most binary files(libs) of these dependencies for you

note: your graphics driver have to support glsl core 330

if not, upgrade driver first

### Build
|Windows|Linux|
|:--:|:--:|
|[![BuildStatus](https://ci.dot.net/job/dotnet_roslyn/job/dev15.7.x/job/ubuntu_16_debug/badge/icon)](https://ci.dot.net/job/dotnet_roslyn/job/dev15.7.x/job/ubuntu_16_debug/)|[![BuildStatus](https://ci.dot.net/job/dotnet_roslyn/job/dev15.7.x/job/mac_debug/badge/icon)](https://ci.dot.net/job/dotnet_roslyn/job/dev15.7.x/job/mac_debug/)|

For windows, you can use cmake to generate a vs solution first(Visual Studio 2017 is suggested, which test pass)

Note you have to assign your boost path

![Screenshot](https://github.com/Bairuo/2048/raw/master/meta/cmake.jpg)

Or if you have a codeblock on your device, you can use build/2048.workspace directly

For linux, just:

```
cd build
cmake .
make
```

### Plus Function/Tech

* 2048 AI(1024 can be achieved at most time)
* Undo and Redo
* History Playback
* User login and Score Rank
* Fluent Animation of card born, move, merge

### Design

Game design is from HUST IS Curriculum Design, January 2018

Full marks on in-site inspect, 96 in final(including report text)