//****************************************************************************//
// corebone.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#pragma once

#include <list>
#include <boost/shared_ptr.hpp>
#include "cal3d/global.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"

class CalCoreSkeleton;

enum CalLightType {
    LIGHT_TYPE_NONE,
    LIGHT_TYPE_OMNI,
    LIGHT_TYPE_DIRECTIONAL,
    LIGHT_TYPE_TARGET,
    LIGHT_TYPE_AMBIENT
};

namespace cal3d {
    struct Transform {
        CalVector translation;
        CalQuaternion rotation;
    };
}

class CAL3D_API CalCoreBone {
public:
    int parentId;
    std::string name;

    CalVector lightColor;
    CalLightType lightType;

    cal3d::Transform relativeTransform;
    cal3d::Transform absoluteTransform;
    cal3d::Transform boneSpaceTransform;

    std::vector<int> childIds;

    CalCoreBone(const std::string& name, int parentId = -1);
    void calculateState(CalCoreSkeleton* skeleton);

    void scale(float factor, CalCoreSkeleton* skeleton);

    bool hasLightingData() const {
        return lightType != LIGHT_TYPE_NONE;
    }
};
