//****************************************************************************//
// renderer.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#pragma once

#include "cal3d/global.h"

class CalModel;
class CalSubmesh;

namespace CalRenderer
{
  CAL3D_API int getMapCount(const CalSubmesh* submesh);
  CAL3D_API Cal::UserData* getMaterialUserData(const CalSubmesh* submesh);
  CAL3D_API void getSpecularColor(const CalSubmesh* submesh, unsigned char *pColorBuffer);
  CAL3D_API bool textureCoordinatesForMapValid(const CalSubmesh* submesh, int mapId );
};
