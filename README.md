# PMC_Asteroids 
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ready](https://img.shields.io/badge/ready-1%25-lightgrey.svg)](https://github.com/Abergard/PMC_Asteroids/tree/pmc)

Pimp my code - Asteroids old school project

## The base idea of PimpMyCode
All PMC_ projects was founded to take old school project and refactor them to achieve few goals:
 - to be buildable on many platform by using cmake
 - code should be cleaner and more expansible
 - if the project use only one of the graphic library (OpenGL/DirectX), should support both

## Branches:
 - master: for base version of project
 - pmc: for ongoing works to make a code be cleaner & better

## How to build
For now it supports only MSVC compilation. Generation, compilation and run example below
```
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" ..
cmake --build .
bin\Debug\asteroids.exe
```
