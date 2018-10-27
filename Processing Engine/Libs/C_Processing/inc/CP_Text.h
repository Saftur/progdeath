//------------------------------------------------------------------------------
// file:	CP_Text.h
// author:	Daniel Hamilton
// brief:	Text functions for loading fonts and drawing text
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
#include "C_Processing.h"

//------------------------------------------------------------------------------
// Defines:
//------------------------------------------------------------------------------

#define FONT_LOAD_ERROR -1

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct PFont_struct* PFont;

//------------------------------------------------------------------------------
// Public Enums:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

DLL_EXPORT PFont getDefaultFont();
#define defaultFont getDefaultFont()

DLL_EXPORT PFont loadFont(const char * filepath);
DLL_EXPORT void  text(const char * text, float x, float y);
DLL_EXPORT void  textBox(const char * text, float x, float y, float row_width);
DLL_EXPORT void  textFont(PFont font, float size);
DLL_EXPORT void  textSize(float size);