//------------------------------------------------------------------------------
// file:	CP_Shape.h
// author:	Daniel Hamilton
// brief:	Manipulate and draw basic vector shapes
//
// DO NOT INCLUDE THIS HEADER IN YOUR PROJECTS DIRECTLY
// SEE: C_Processing.h
//
// Copyright © 2018 DigiPen, All rights reserved.
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "CP_Export.h"

//------------------------------------------------------------------------------
// Public Enums:
//------------------------------------------------------------------------------

//////////////////
enum ShapeMode
{
    CENTER = 0,  // Center
    CORNER,  // Top Left
    CORNERS, // Top Left, Bottom Right
    RADIUS,  // Ellipse by radius rather than diameter

    // tracking
    SHAPEMODE_MAX,
};
typedef enum ShapeMode ShapeMode;
//////////////////

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

DLL_EXPORT void rect(float x, float y, float w, float h);
DLL_EXPORT void ellipse(float x, float y, float w, float h);
DLL_EXPORT void circle(float x, float y, float r);
DLL_EXPORT void line(float x1, float y1, float x2, float y2);
DLL_EXPORT void point(float x, float y);
DLL_EXPORT void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
/*
    A quad is a quadrilateral, a four sided polygon. It is similar to a rectangle, 
    but the angles between its edges are not constrained to ninety degrees. The 
    first pair of parameters (x1,y1) sets the first vertex and the subsequent pairs 
    should proceed clockwise or counter-clockwise around the defined shape.
*/
DLL_EXPORT void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

// Rotated shapes
// Circle/point dont have rotated forms as they do not make sense
DLL_EXPORT void rectRotated(float x, float y, float w, float h, float degrees);
DLL_EXPORT void ellipseRotated(float x, float y, float w, float h, float degrees);
DLL_EXPORT void lineRotated(float x1, float y1, float x2, float y2, float degrees);
DLL_EXPORT void triangleRotated(float x1, float y1, float x2, float y2, float x3, float y3, float degrees);
DLL_EXPORT void quadRotated(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float degrees);

// Uniform rounded rectangle
DLL_EXPORT void rectRounded(float x, float y, float w, float h, float r);

// Create a new shape manually by defining a path
DLL_EXPORT void beginShape();
DLL_EXPORT void vertex(float x, float y);
DLL_EXPORT void endShape();

// Set the mode via which rectangles and ellipses draw
DLL_EXPORT void rectMode(ShapeMode mode);
DLL_EXPORT void ellipseMode(ShapeMode mode);
DLL_EXPORT void circleMode(ShapeMode mode);