//****************************************************************************//
// bone.h                                                                     //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#pragma once

#include "cal3d/bonetransform.h"
#include "cal3d/global.h"
#include "cal3d/transform.h"

class CalCoreBone;
class CalSkeleton;
class CalModel;

class CAL3D_API CalBone {
public:
    cal3d::Transform relativeTransform;
    cal3d::Transform absoluteTransform;

    CalBone(const CalCoreBone& coreBone);

    void blendState(
        float unrampedWeight,
        const CalVector& translation,
        const CalQuaternion& rotation,
        float scale = 1.0f,
        bool replace = false,
        float rampValue = 1.0f);
    BoneTransform calculateState(const CalBone* bones);
    void clearState();
    const CalVector& getOriginalTranslation() const {
        return coreRelativeTransform.translation;
    }

    void setMeshScaleAbsolute(const CalVector& sv) {
        m_meshScaleAbsolute = sv;
    }
    void lockState();

private:
    // from core bone. stored locally for better cache locality
    int parentId;
    cal3d::Transform coreRelativeTransform;
    cal3d::Transform coreBoneSpaceTransform;

    // animated bone state
    float m_accumulatedWeight;
    float m_accumulatedWeightAbsolute;
    float m_accumulatedReplacementAttenuation;
    float m_firstBlendScale;
    CalVector m_meshScaleAbsolute; // w.r.t. absolute coord system in 3dsMax (Z up), not local coord of bone.
};
