//---------------------------------------------------------
// file:	C_Processing.h
// author:	Justin Chambers, Daniel Hamilton
// brief:	Main header file for the C_Processing interface
//
// USE:
// Include ONLY this header when building application with C_Processing libraries.
//
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
// CTRL + SHIFT + G to go to any file when highlighted
#include "CP_Export.h"  // Internal C_Processing (ignore)
#include "CP_Color.h"   // Creating, editing colors
#include "CP_Image.h"   // Creating, displaying images
#include "CP_Input.h"   // Reading input from the mouse or keyboard
#include "CP_Math.h"    // All the math functions you could ever want
#include "CP_Random.h"  // Random number generation (with ranges)
#include "CP_Noise.h"   // Gaussian noise and related
#include "CP_Setting.h" // Canvas settings (size, color, stroke, camera)
#include "CP_Shape.h"   // Draw shapes (rect, ellipse, etc)
#include "CP_Sound.h"   // Load, modulate and play sounds on channels
#include "CP_Text.h"    // Display text


// A pointer to a function of the form: void foo(void)
typedef void(*FunctionPtr)();

// Run your program given an initialization function and a looping update function. This is
// the default entry point for a C Processing program.
DLL_EXPORT void Run(FunctionPtr init, FunctionPtr update);

// Terminate will complete the current frame update and then terminate the program
DLL_EXPORT void Terminate();

// Get the position of the mouse. These will be globally available by using
// mouseX and mouseY in your code to access the position of the mouse.
DLL_EXPORT float getmouseX();
DLL_EXPORT float getmouseY();
#define mouseX getmouseX()   // Use "mouseX" to access the current mouse position
#define mouseY getmouseY()   // Use "mouseY" to access the current mouse position

// Get the previous position of the mouse. These will be globally available 
// using pmouseX and pmouseY in your code to access the position of the mouse.
DLL_EXPORT float getpmouseX();
DLL_EXPORT float getpmouseY();
#define pmouseX getpmouseX()  // Use "pmouseX" to access the previous mouse position
#define pmouseY getpmouseY()  // Use "pmouseY" to access the previous mouse position

// Get the position of the mouse. These will be globally available by using
// pmouseX and pmouseY in your code to access the position of the mouse.
DLL_EXPORT float getworldmouseX();
DLL_EXPORT float getworldmouseY();
#define worldMouseX getworldmouseX()   // Use "worldMouseX" in your code to access the mouse in world coordinates
#define worldMouseY getworldmouseY()   // Use "worldMouseY" in your code to access the mouse in world coordinates

// Use frameCount to get the number of frames that have elapsed since the 
// program started.
DLL_EXPORT int getframeCount();
#define frameCount getframeCount()   // Use "frameCount" to the the number of frames elapsed

// Get the display size of the window excluding the frame if it exists. This is
// the size of the canvas you will be drawing on.
DLL_EXPORT int getCanvasWidth();
DLL_EXPORT int getCanvasHeight();
#define canvasWidth getCanvasWidth()     // Use "canvasWidth" to access the size of the canvas in pixels
#define canvasHeight getCanvasHeight()   // Use "canvasHeight" to access the size of the canvas in pixels
