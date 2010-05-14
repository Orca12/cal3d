//****************************************************************************//
// coretrack.h                                                                //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#pragma once

#include "cal3d/global.h"
#include "cal3d/vector.h"
#include "cal3d/quaternion.h"
#include "cal3d/corekeyframe.h"

class CalCoreBone;
class CalCoreSkeleton;


class CAL3D_API CalCoreTrack : public Cal::Object
{
private:
  /// The index of the associated CoreBone in the CoreSkeleton.
  int m_coreBoneId;

  // If translationRequired is false, then the translations are the same as the
  // skeleton's translations.
  bool m_translationRequired;
  bool m_highRangeRequired;
  bool m_translationIsDynamic;
  static int m_translationRequiredCount;
  static int m_translationNotRequiredCount;

  /// List of keyframes, always sorted by time.
  typedef std::vector<CalCoreKeyframe> CalCoreKeyFrameVector;
  CalCoreKeyFrameVector m_keyframes;

public:
  CalCoreTrack();
  ~CalCoreTrack();

  void create();
  void destroy();
  size_t size() const;

  bool getState(float time, CalVector& translation, CalQuaternion& rotation);

  int getCoreBoneId();
  bool setCoreBoneId(int coreBoneId);
  
  int getCoreKeyframeCount();
  CalCoreKeyframe const &getCoreKeyframe(int idx);

  static int translationRequiredCount() { return m_translationRequiredCount; }
  static int translationNotRequiredCount() { return m_translationNotRequiredCount; }
  bool addCoreKeyframe(CalCoreKeyframe coreKeyframe);
  bool getTranslationRequired() { return m_translationRequired; }
  void setTranslationRequired( bool p ) { m_translationRequired = p; }
  bool getTranslationIsDynamic() { return m_translationIsDynamic; }
  void setTranslationIsDynamic( bool p ) { m_translationIsDynamic = p; }
  bool getHighRangeRequired() { return m_highRangeRequired; }
  void setHighRangeRequired( bool p ) { m_highRangeRequired = p; }
  void fillInvalidTranslations( CalVector const & trans );

  void scale(float factor);
  void compress( double translationTolerance, double rotationToleranceDegrees, CalCoreSkeleton * skelOrNull );
  bool roundTranslation( CalCoreKeyframe const & prev, CalCoreKeyframe & p, double translationTolerance );
  void translationCompressibility( 
    bool * transRequiredResult, bool * transDynamicResult, bool * highRangeRequiredResult,
    float threshold, float highRangeThreshold, CalCoreSkeleton * skel );
  void collapseSequences( double translationTolerance, double rotationToleranceDegrees );

private:

  bool keyframeEliminatable( CalCoreKeyframe & prev, CalCoreKeyframe & p, CalCoreKeyframe & next,
	   double translationTolerance, double rotationToleranceDegrees);

private:
  CalCoreKeyFrameVector::iterator getUpperBound(float time);
};
