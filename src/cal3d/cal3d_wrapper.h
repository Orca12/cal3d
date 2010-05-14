//****************************************************************************//
// cal3d_wrapper.h                                                            //
// Copyright (C) 2002 Bruno 'Beosil' Heidelberger                             //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#pragma once

#ifdef __cplusplus
#include "cal3d/global.h"
#endif

//****************************************************************************//
// Defines for Win32 and MingW32                                              //
//****************************************************************************//

#ifdef _WIN32

#ifdef __MINGW32__

#define CAL3D_WRAPPER_API

#else

#pragma warning(disable : 4251)
#pragma warning(disable : 4099)

#ifdef CAL3D_WRAPPER_EXPORTS
#define CAL3D_WRAPPER_API __declspec(dllexport)
#else
#define CAL3D_WRAPPER_API __declspec(dllimport)
#endif

#endif

#else

//****************************************************************************//
// Defines for Linux, Cygwin, FreeBSD Sun and Mips...                         //
//****************************************************************************//

#define CAL3D_WRAPPER_API

#endif 

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

#ifndef CAL3D_WRAPPER_EXPORTS

struct CalAnimation;
struct CalAnimationAction;
struct CalAnimationCycle;
struct CalBone;
struct CalCoreAnimation;
struct CalCoreBone;
struct CalCoreKeyframe;
struct CalCoreMaterial;
struct CalCoreMesh;
struct CalCoreModel;
struct CalCoreSkeleton;
struct CalCoreSubmesh;
struct CalCoreTrack;
struct CalLoader;
struct CalMatrix;
struct CalMesh;
struct CalMixer;
struct CalModel;
struct CalPhysique;
struct CalPlatform;
struct CalQuaternion;
struct CalRenderer;
struct CalSaver;
struct CalSkeleton;
struct CalSpringSystem;
struct CalSubmesh;
struct CalVector;

#ifndef __cplusplus
typedef int CalIndex; 
#endif

#endif



//****************************************************************************//
// Substitute for the C++ 'bool' type                                         //
//****************************************************************************//

enum Boolean
{
  False = 0,
  True = 1
};

//****************************************************************************//
// "C" wrapper functions declaration                                          //
//****************************************************************************//

#ifdef __cplusplus
extern "C"
{
#endif

//****************************************************************************//
// CalAnimation wrapper functions declaration                                 //
//****************************************************************************//

  CAL3D_WRAPPER_API enum CalAnimationType
  {
    ANIMATION_TYPE_NONE = 0,
    ANIMATION_TYPE_CYCLE,
    ANIMATION_TYPE_POSE,
    ANIMATION_TYPE_ACTION
  };

  CAL3D_WRAPPER_API enum CalAnimationState
  {
    ANIMATION_STATE_NONE = 0,
    ANIMATION_STATE_SYNC,
    ANIMATION_STATE_ASYNC,
    ANIMATION_STATE_IN,
    ANIMATION_STATE_STEADY,
    ANIMATION_STATE_OUT
  };

  CAL3D_WRAPPER_API enum Boolean CalAnimation_Create(struct CalAnimation *self, struct  CalCoreAnimation *pCoreAnimation);
  CAL3D_WRAPPER_API void CalAnimation_Delete(struct CalAnimation *self);
  CAL3D_WRAPPER_API void CalAnimation_Destroy(struct CalAnimation *self);
  CAL3D_WRAPPER_API struct CalCoreAnimation *CalAnimation_GetCoreAnimation(struct CalAnimation *self);
  CAL3D_WRAPPER_API enum CalAnimationState CalAnimation_GetState(struct CalAnimation *self);
  CAL3D_WRAPPER_API float CalAnimation_GetTime(struct CalAnimation *self);
  CAL3D_WRAPPER_API enum CalAnimationType CalAnimation_GetType(struct CalAnimation *self);
  CAL3D_WRAPPER_API float CalAnimation_GetWeight(struct CalAnimation *self);

//****************************************************************************//
// CalAnimationAction wrapper functions declaration                           //
//****************************************************************************//

  CAL3D_WRAPPER_API enum Boolean CalAnimationAction_Create(struct CalAnimationAction *self, struct  CalCoreAnimation *pCoreAnimation);
  CAL3D_WRAPPER_API void CalAnimationAction_Delete(struct CalAnimationAction *self);
  CAL3D_WRAPPER_API void CalAnimationAction_Destroy(struct CalAnimationAction *self);
  CAL3D_WRAPPER_API enum Boolean CalAnimationAction_Execute(struct CalAnimationAction *self, float delayIn, float delayOut);
  CAL3D_WRAPPER_API struct CalAnimationAction *CalAnimationAction_New();
  CAL3D_WRAPPER_API enum Boolean CalAnimationAction_Update(struct CalAnimationAction *self, float deltaTime);

//****************************************************************************//
// CalAnimationCycle wrapper functions declaration                            //
//****************************************************************************//

  CAL3D_WRAPPER_API enum Boolean CalAnimationCycle_Blend(struct CalAnimationCycle *self, float weight, float delay);
  CAL3D_WRAPPER_API enum Boolean CalAnimationCycle_Create(struct CalAnimationCycle *self, struct  CalCoreAnimation *pCoreAnimation);
  CAL3D_WRAPPER_API void CalAnimationCycle_Delete(struct CalAnimationCycle *self);
  CAL3D_WRAPPER_API void CalAnimationCycle_Destroy(struct CalAnimationCycle *self);
  CAL3D_WRAPPER_API struct CalAnimationCycle *CalAnimationCycle_New();
  CAL3D_WRAPPER_API void CalAnimationCycle_SetAsync(struct CalAnimationCycle *self, float time, float duration);
  CAL3D_WRAPPER_API enum Boolean CalAnimationCycle_Update(struct CalAnimationCycle *self, float deltaTime);

//****************************************************************************//
// CalCoreAnimation wrapper functions declaration                             //
//****************************************************************************//

  CAL3D_WRAPPER_API enum Boolean CalCoreAnimation_AddCoreTrack(struct CalCoreAnimation *self, struct CalCoreTrack *pCoreTrack);
  CAL3D_WRAPPER_API enum Boolean CalCoreAnimation_Create(struct CalCoreAnimation *self);
  CAL3D_WRAPPER_API void CalCoreAnimation_Delete(struct CalCoreAnimation *self);
  CAL3D_WRAPPER_API void CalCoreAnimation_Destroy(struct CalCoreAnimation *self);
  CAL3D_WRAPPER_API struct CalCoreTrack *CalCoreAnimation_GetCoreTrack(struct CalCoreAnimation *self, int coreBoneId);
  CAL3D_WRAPPER_API float CalCoreAnimation_GetDuration(struct CalCoreAnimation *self);
//  CAL3D_WRAPPER_API std::list<CalCoreTrack *>& CalCoreAnimation_GetListCoreTrack(struct CalCoreAnimation *self);
  CAL3D_WRAPPER_API struct CalCoreAnimation *CalCoreAnimation_New();
  CAL3D_WRAPPER_API void CalCoreAnimation_SetDuration(struct CalCoreAnimation *self, float duration);

//****************************************************************************//
// CalCoreBone wrapper functions declaration                                  //
//****************************************************************************//

  CAL3D_WRAPPER_API enum Boolean CalCoreBone_AddChildId(struct CalCoreBone *self, int childId);
  CAL3D_WRAPPER_API void CalCoreBone_CalculateState(struct CalCoreBone *self);
  CAL3D_WRAPPER_API enum Boolean CalCoreBone_Create(struct CalCoreBone *self, char *strName);
  CAL3D_WRAPPER_API void CalCoreBone_Delete(struct CalCoreBone *self);
  CAL3D_WRAPPER_API void CalCoreBone_Destroy(struct CalCoreBone *self);
//  CAL3D_WRAPPER_API std::list<int>& CalCoreBone_GetListChildId(struct CalCoreBone *self);
  CAL3D_WRAPPER_API char *CalCoreBone_GetName(struct CalCoreBone *self);
  CAL3D_WRAPPER_API int CalCoreBone_GetParentId(struct CalCoreBone *self);
  CAL3D_WRAPPER_API struct CalQuaternion *CalCoreBone_GetRotation(struct CalCoreBone *self);
  CAL3D_WRAPPER_API struct CalQuaternion *CalCoreBone_GetRotationAbsolute(struct CalCoreBone *self);
  CAL3D_WRAPPER_API struct CalQuaternion *CalCoreBone_GetRotationBoneSpace(struct CalCoreBone *self);
  CAL3D_WRAPPER_API struct CalVector *CalCoreBone_GetTranslation(struct CalCoreBone *self);
  CAL3D_WRAPPER_API struct CalVector *CalCoreBone_GetTranslationAbsolute(struct CalCoreBone *self);
  CAL3D_WRAPPER_API struct CalVector *CalCoreBone_GetTranslationBoneSpace(struct CalCoreBone *self);
  CAL3D_WRAPPER_API struct CalCoreBone *CalCoreBone_New();
  CAL3D_WRAPPER_API void CalCoreBone_SetCoreSkeleton(struct CalCoreBone *self, struct CalCoreSkeleton *pCoreSkeleton);
  CAL3D_WRAPPER_API void CalCoreBone_SetParentId(struct CalCoreBone *self, int parentId);
  CAL3D_WRAPPER_API void CalCoreBone_SetRotation(struct CalCoreBone *self, struct CalQuaternion *pRotation);
  CAL3D_WRAPPER_API void CalCoreBone_SetRotationBoneSpace(struct CalCoreBone *self, struct CalQuaternion *pRotation);
  CAL3D_WRAPPER_API void CalCoreBone_SetTranslation(struct CalCoreBone *self, struct CalVector *pTranslation);
  CAL3D_WRAPPER_API void CalCoreBone_SetTranslationBoneSpace(struct CalCoreBone *self, struct CalVector *pTranslation);

//****************************************************************************//
// CalCoreKeyframe wrapper functions declaration                              //
//****************************************************************************//

  CAL3D_WRAPPER_API enum Boolean CalCoreKeyframe_Create(struct CalCoreKeyframe *self);
  CAL3D_WRAPPER_API void CalCoreKeyframe_Delete(struct CalCoreKeyframe *self);
  CAL3D_WRAPPER_API void CalCoreKeyframe_Destroy(struct CalCoreKeyframe *self);
  CAL3D_WRAPPER_API struct CalQuaternion *CalCoreKeyframe_GetRotation(struct CalCoreKeyframe *self);
  CAL3D_WRAPPER_API float CalCoreKeyframe_GetTime(struct CalCoreKeyframe *self);
  CAL3D_WRAPPER_API struct CalVector *CalCoreKeyframe_GetTranslation(struct CalCoreKeyframe *self);
  CAL3D_WRAPPER_API struct CalCoreKeyframe *CalCoreKeyframe_New();
  CAL3D_WRAPPER_API void CalCoreKeyframe_SetRotation(struct CalCoreKeyframe *self, struct CalQuaternion *pRotation);
  CAL3D_WRAPPER_API void CalCoreKeyframe_SetTime(struct CalCoreKeyframe *self, float time);
  CAL3D_WRAPPER_API void CalCoreKeyframe_SetTranslation(struct CalCoreKeyframe *self, struct CalVector *pTranslation);

//****************************************************************************//
// CalCoreMaterial wrapper functions declaration                              //
//****************************************************************************//

  CAL3D_WRAPPER_API enum Boolean CalCoreMaterial_Create(struct CalCoreMaterial *self);
  CAL3D_WRAPPER_API void CalCoreMaterial_Delete(struct CalCoreMaterial *self);
  CAL3D_WRAPPER_API void CalCoreMaterial_Destroy(struct CalCoreMaterial *self);
//  CAL3D_WRAPPER_API CalCoreMaterial::Color *CalCoreMaterial_GetAmbientColor(struct CalCoreMaterial *self);
//  CAL3D_WRAPPER_API CalCoreMaterial::Color *CalCoreMaterial_GetDiffuseColor(struct CalCoreMaterial *self);
  CAL3D_WRAPPER_API int CalCoreMaterial_GetMapCount(struct CalCoreMaterial *self);
  CAL3D_WRAPPER_API char *CalCoreMaterial_GetMapFilename(struct CalCoreMaterial *self, int mapId);
  CAL3D_WRAPPER_API char *CalCoreMaterial_GetMapType(struct CalCoreMaterial *self, int mapId);
  CAL3D_WRAPPER_API float CalCoreMaterial_GetShininess(struct CalCoreMaterial *self);
//  CAL3D_WRAPPER_API CalCoreMaterial::Color *CalCoreMaterial_GetSpecularColor(struct CalCoreMaterial *self);
//  CAL3D_WRAPPER_API std::vector<Map>& CalCoreMaterial_GetVectorMap(struct CalCoreMaterial *self);
  CAL3D_WRAPPER_API struct CalCoreMaterial *CalCoreMaterial_New();
  CAL3D_WRAPPER_API enum Boolean CalCoreMaterial_Reserve(struct CalCoreMaterial *self, int mapCount);
//  CAL3D_WRAPPER_API void CalCoreMaterial_SetAmbientColor(struct CalCoreMaterial *self, struct CalCoreMaterial::Color *pAmbientColor);
//  CAL3D_WRAPPER_API void CalCoreMaterial_SetDiffuseColor(struct CalCoreMaterial *self, struct CalCoreMaterial::Color *pDiffuseColor);
//  CAL3D_WRAPPER_API enum Boolean CalCoreMaterial_SetMap(struct CalCoreMaterial *self, int mapId, struct CalCoreMaterial::Map *pMap);
  CAL3D_WRAPPER_API void CalCoreMaterial_SetShininess(struct CalCoreMaterial *self, float shininess);
//  CAL3D_WRAPPER_API void CalCoreMaterial_SetSpecularColor(struct CalCoreMaterial *self, struct CalCoreMaterial::Color *pSpecularColor);

//****************************************************************************//
// CalCoreMesh wrapper functions declaration                                  //
//****************************************************************************//

  CAL3D_WRAPPER_API int CalCoreMesh_AddCoreSubmesh(struct CalCoreMesh *self, struct CalCoreSubmesh *pCoreSubmesh);
  CAL3D_WRAPPER_API enum Boolean CalCoreMesh_Create(struct CalCoreMesh *self);
  CAL3D_WRAPPER_API void CalCoreMesh_Delete(struct CalCoreMesh *self);
  CAL3D_WRAPPER_API struct CalCoreSubmesh *CalCoreMesh_GetCoreSubmesh(struct CalCoreMesh *self, int id);
  CAL3D_WRAPPER_API int CalCoreMesh_GetCoreSubmeshCount(struct CalCoreMesh *self);
//  CAL3D_WRAPPER_API std::vector<CalCoreSubmesh *>& CalCoreMesh_GetVectorCoreSubmesh(struct CalCoreMesh *self);
  CAL3D_WRAPPER_API struct CalCoreMesh *CalCoreMesh_New();

//****************************************************************************//
// CalCoreModel wrapper functions declaration                                 //
//****************************************************************************//

  CAL3D_WRAPPER_API int CalCoreModel_AddCoreAnimation(struct CalCoreModel *self, struct  CalCoreAnimation *pCoreAnimation);
  CAL3D_WRAPPER_API int CalCoreModel_AddCoreMaterial(struct CalCoreModel *self, struct CalCoreMaterial *pCoreMaterial);
  CAL3D_WRAPPER_API enum Boolean CalCoreModel_Create(struct CalCoreModel *self, char *strName);
  CAL3D_WRAPPER_API enum Boolean CalCoreModel_CreateCoreMaterialThread(struct CalCoreModel *self, int coreMaterialThreadId);
  CAL3D_WRAPPER_API void CalCoreModel_Delete(struct CalCoreModel *self);
  CAL3D_WRAPPER_API void CalCoreModel_Destroy(struct CalCoreModel *self);
  CAL3D_WRAPPER_API struct CalCoreAnimation *CalCoreModel_GetCoreAnimation(struct CalCoreModel *self, int coreAnimationId);
  CAL3D_WRAPPER_API int CalCoreModel_GetCoreAnimationMaxId(struct CalCoreModel *self);
  CAL3D_WRAPPER_API struct CalCoreAnimatedMorph *CalCoreModel_GetCoreAnimatedMorph(struct CalCoreModel *self, int coreAnimatedMorphId);
  CAL3D_WRAPPER_API int CalCoreModel_GetCoreAnimatedMorphCount(struct CalCoreModel *self);
  CAL3D_WRAPPER_API struct CalCoreMaterial *CalCoreModel_GetCoreMaterial(struct CalCoreModel *self, int coreMaterialId);
  CAL3D_WRAPPER_API int CalCoreModel_GetCoreMaterialCount(struct CalCoreModel *self);
  CAL3D_WRAPPER_API int CalCoreModel_GetCoreMaterialId(struct CalCoreModel *self, int coreMaterialThreadId, int coreMaterialSetId);
  CAL3D_WRAPPER_API struct CalCoreMesh *CalCoreModel_GetCoreMesh(struct CalCoreModel *self, int coreMeshId);
  CAL3D_WRAPPER_API int CalCoreModel_GetCoreMeshCount(struct CalCoreModel *self);
  CAL3D_WRAPPER_API struct CalCoreSkeleton *CalCoreModel_GetCoreSkeleton(struct CalCoreModel *self);
  CAL3D_WRAPPER_API int CalCoreModel_LoadCoreAnimation(struct CalCoreModel *self, char *strFilename);
  CAL3D_WRAPPER_API int CalCoreModel_LoadCoreAnimatedMorph(struct CalCoreModel *self, char *strFilename);
  CAL3D_WRAPPER_API int CalCoreModel_LoadCoreMaterial(struct CalCoreModel *self, char *strFilename);
  CAL3D_WRAPPER_API int CalCoreModel_LoadCoreMesh(struct CalCoreModel *self, char *strFilename);
  CAL3D_WRAPPER_API enum Boolean CalCoreModel_LoadCoreSkeleton(struct CalCoreModel *self, char *strFilename);
  CAL3D_WRAPPER_API struct CalCoreModel *CalCoreModel_New();
  CAL3D_WRAPPER_API enum Boolean CalCoreModel_SaveCoreAnimation(struct CalCoreModel *self, char *strFilename, int coreAnimtionId);
  CAL3D_WRAPPER_API enum Boolean CalCoreModel_SaveCoreMaterial(struct CalCoreModel *self, char *strFilename, int coreMaterialId);
  CAL3D_WRAPPER_API enum Boolean CalCoreModel_SaveCoreMesh(struct CalCoreModel *self, char *strFilename, int coreMeshId);
  CAL3D_WRAPPER_API enum Boolean CalCoreModel_SaveCoreSkeleton(struct CalCoreModel *self, char *strFilename);
  CAL3D_WRAPPER_API enum Boolean CalCoreModel_SetCoreMaterialId(struct CalCoreModel *self, int coreMaterialThreadId, int coreMaterialSetId, int coreMaterialId);
  CAL3D_WRAPPER_API void CalCoreModel_SetCoreSkeleton(struct CalCoreModel *self, struct CalCoreSkeleton *pCoreSkeleton);

//****************************************************************************//
// CalCoreSkeleton wrapper functions declaration                              //
//****************************************************************************//

  CAL3D_WRAPPER_API int CalCoreSkeleton_AddCoreBone(struct CalCoreSkeleton *self, struct CalCoreBone *pCoreBone);
  CAL3D_WRAPPER_API void CalCoreSkeleton_CalculateState(struct CalCoreSkeleton *self);
  CAL3D_WRAPPER_API enum Boolean CalCoreSkeleton_Create(struct CalCoreSkeleton *self);
  CAL3D_WRAPPER_API void CalCoreSkeleton_Delete(struct CalCoreSkeleton *self);
  CAL3D_WRAPPER_API void CalCoreSkeleton_Destroy(struct CalCoreSkeleton *self);
  CAL3D_WRAPPER_API struct CalCoreBone *CalCoreSkeleton_GetCoreBone(struct CalCoreSkeleton *self, int coreBoneId);
  CAL3D_WRAPPER_API int CalCoreSkeleton_GetCoreBoneId(struct CalCoreSkeleton *self, char *strName);
//  CAL3D_WRAPPER_API std::list<int>& CalCoreSkeleton_GetListRootCoreBoneId(struct CalCoreSkeleton *self);
//  CAL3D_WRAPPER_API std::vector<CalCoreBone *>& CalCoreSkeleton_GetVectorCoreBone(struct CalCoreSkeleton *self);
  CAL3D_WRAPPER_API struct CalCoreSkeleton *CalCoreSkeleton_New();

//****************************************************************************//
// CalCoreSubmesh wrapper functions declaration                               //
//****************************************************************************//

  CAL3D_WRAPPER_API enum Boolean CalCoreSubmesh_Create(struct CalCoreSubmesh *self);
  CAL3D_WRAPPER_API void CalCoreSubmesh_Delete(struct CalCoreSubmesh *self);
  CAL3D_WRAPPER_API int CalCoreSubmesh_GetCoreMaterialThreadId(struct CalCoreSubmesh *self);
  CAL3D_WRAPPER_API int CalCoreSubmesh_GetFaceCount(struct CalCoreSubmesh *self);
  CAL3D_WRAPPER_API int CalCoreSubmesh_GetLodCount(struct CalCoreSubmesh *self);
  CAL3D_WRAPPER_API int CalCoreSubmesh_GetSpringCount(struct CalCoreSubmesh *self);
  CAL3D_WRAPPER_API int CalCoreSubmesh_GetVertexCount(struct CalCoreSubmesh *self);
  CAL3D_WRAPPER_API struct CalCoreSubmesh *CalCoreSubmesh_New();
  CAL3D_WRAPPER_API enum Boolean CalCoreSubmesh_Reserve(struct CalCoreSubmesh *self, int vertexCount, int textureCoordinateCount, int faceCount, int springCount);
  CAL3D_WRAPPER_API void CalCoreSubmesh_SetCoreMaterialThreadId(struct CalCoreSubmesh *self, int coreMaterialThreadId);
  CAL3D_WRAPPER_API void CalCoreSubmesh_SetLodCount(struct CalCoreSubmesh *self, int lodCount);
CAL3D_WRAPPER_API enum Boolean  CalCoreSubmesh_IsTangentsEnabled(struct CalCoreSubmesh *self, int mapId);
CAL3D_WRAPPER_API enum Boolean  CalCoreSubmesh_EnableTangents(struct CalCoreSubmesh *self, int mapId, bool enabled);

//****************************************************************************//
// CalCoreTrack wrapper functions declaration                                 //
//****************************************************************************//

  CAL3D_WRAPPER_API enum Boolean CalCoreTrack_AddCoreKeyframe(struct CalCoreTrack *self, struct CalCoreKeyframe &pCoreKeyframe);
  CAL3D_WRAPPER_API enum Boolean CalCoreTrack_Create(struct CalCoreTrack *self);
  CAL3D_WRAPPER_API void CalCoreTrack_Delete(struct CalCoreTrack *self);
  CAL3D_WRAPPER_API void CalCoreTrack_Destroy(struct CalCoreTrack *self);
  CAL3D_WRAPPER_API int CalCoreTrack_GetCoreBoneId(struct CalCoreTrack *self);
//  CAL3D_WRAPPER_API std::map<float, CalCoreKeyframe *>& CalCoreTrack_GetMapCoreKeyframe(struct CalCoreTrack *self);
  CAL3D_WRAPPER_API enum Boolean CalCoreTrack_GetState(struct CalCoreTrack *self, float time, struct CalVector *pTranslation, struct CalQuaternion *pRotation);
  CAL3D_WRAPPER_API struct CalCoreTrack *CalCoreTrack_New();
  CAL3D_WRAPPER_API enum Boolean CalCoreTrack_SetCoreBoneId(struct CalCoreTrack *self, int coreBoneId);

//****************************************************************************//
// CalError wrapper functions declaration                                     //
//****************************************************************************//

  CAL3D_WRAPPER_API enum CalErrorCode
  {
    ERROR_CODE_OK = 0,
    ERROR_CODE_INTERNAL,
    ERROR_CODE_INVALID_HANDLE,
    ERROR_CODE_MEMORY_ALLOCATION_FAILED,
    ERROR_CODE_FILE_NOT_FOUND,
    ERROR_CODE_INVALID_FILE_FORMAT,
    ERROR_CODE_FILE_PARSER_FAILED,
    ERROR_CODE_INDEX_BUILD_FAILED,
    ERROR_CODE_NO_PARSER_DOCUMENT,
    ERROR_CODE_INVALID_ANIMATION_DURATION,
    ERROR_CODE_BONE_NOT_FOUND,
    ERROR_CODE_INVALID_ATTRIBUTE_VALUE,
    ERROR_CODE_INVALID_KEYFRAME_COUNT,
    ERROR_CODE_INVALID_ANIMATION_TYPE,
    ERROR_CODE_FILE_CREATION_FAILED,
    ERROR_CODE_FILE_WRITING_FAILED,
    ERROR_CODE_INCOMPATIBLE_FILE_VERSION,
    ERROR_CODE_NO_MESH_IN_MODEL,
    ERROR_CODE_MAX_ERROR_CODE
  };

  CAL3D_WRAPPER_API enum CalErrorCode CalError_GetLastErrorCode();
  CAL3D_WRAPPER_API char *CalError_GetLastErrorDescription();
  CAL3D_WRAPPER_API char *CalError_GetLastErrorFile();
  CAL3D_WRAPPER_API int CalError_GetLastErrorLine();
  CAL3D_WRAPPER_API char *CalError_GetLastErrorText();
  CAL3D_WRAPPER_API void CalError_PrintLastError();
//  CAL3D_WRAPPER_API void CalError_SetLastError(enum CalErrorCode code, char *strFile, int line, char *strText);

//****************************************************************************//
// CalLoader wrapper functions declaration                                    //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalLoader_Delete(struct CalLoader *self);
  CAL3D_WRAPPER_API struct CalCoreAnimation *CalLoader_LoadCoreAnimationFromBuffer(struct CalLoader *self, char *buf, unsigned int len);
  CAL3D_WRAPPER_API struct CalCoreAnimatedMorph *CalLoader_LoadCoreAnimatedMorphFromBuffer(struct CalLoader *self, char *buf, unsigned int len);
  CAL3D_WRAPPER_API struct CalCoreMaterial *CalLoader_LoadCoreMaterialFromBuffer(struct CalLoader *self, char *buf, unsigned int len);
  CAL3D_WRAPPER_API struct CalCoreMesh *CalLoader_LoadCoreMeshFromBuffer(struct CalLoader *self, char *buf, unsigned int len);
  CAL3D_WRAPPER_API struct CalCoreSkeleton *CalLoader_LoadCoreSkeletonFromBuffer(struct CalLoader *self, char *buf, unsigned int len);
  CAL3D_WRAPPER_API struct CalLoader *CalLoader_New();

//****************************************************************************//
// CalMesh wrapper functions declaration                                      //
//****************************************************************************//

  CAL3D_WRAPPER_API enum Boolean CalMesh_Create(struct CalMesh *self, struct CalCoreMesh *pCoreMesh);
  CAL3D_WRAPPER_API void CalMesh_Delete(struct CalMesh *self);
  CAL3D_WRAPPER_API void CalMesh_Destroy(struct CalMesh *self);
  CAL3D_WRAPPER_API struct CalCoreMesh *CalMesh_GetCoreMesh(struct CalMesh *self);
  CAL3D_WRAPPER_API struct CalSubmesh *CalMesh_GetSubmesh(struct CalMesh *self, int id);
  CAL3D_WRAPPER_API int CalMesh_GetSubmeshCount(struct CalMesh *self);
//  CAL3D_WRAPPER_API std::vector<CalSubmesh *>& CalMesh_GetVectorSubmesh(struct CalMesh *self);
  CAL3D_WRAPPER_API struct CalMesh *CalMesh_New();
  CAL3D_WRAPPER_API void CalMesh_SetLodLevel(struct CalMesh *self, float lodLevel);
  CAL3D_WRAPPER_API void CalMesh_SetMaterialSet(struct CalMesh *self, int setId);
  CAL3D_WRAPPER_API void CalMesh_SetModel(struct CalMesh *self, struct CalModel *pModel);

//****************************************************************************//
// CalMixer wrapper functions declaration                                     //
//****************************************************************************//

  CAL3D_WRAPPER_API enum Boolean CalMixer_BlendCycle(struct CalMixer *self, int id, float weight, float delay);
  CAL3D_WRAPPER_API enum Boolean CalMixer_ClearCycle(struct CalMixer *self, int id, float delay);
  CAL3D_WRAPPER_API void CalMixer_Create(struct CalMixer *self, struct CalModel *pModel);
  CAL3D_WRAPPER_API void CalMixer_Delete(struct CalMixer *self);
  CAL3D_WRAPPER_API void CalMixer_Destroy(struct CalMixer *self);
  CAL3D_WRAPPER_API enum Boolean CalMixer_ExecuteAction(struct CalMixer *self, int id, float delayIn, float delayOut);
  CAL3D_WRAPPER_API struct CalMixer *CalMixer_New();
  CAL3D_WRAPPER_API void CalMixer_UpdateAnimation(struct CalMixer *self, float deltaTime);
  CAL3D_WRAPPER_API void CalMixer_UpdateSkeleton(struct CalMixer *self);

//****************************************************************************//
// CalModel wrapper functions declaration                                     //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalModel_Create(struct CalModel *self, struct CalCoreModel *pCoreModel);
  CAL3D_WRAPPER_API void CalModel_Delete(struct CalModel *self);
  CAL3D_WRAPPER_API void CalModel_Destroy(struct CalModel *self);
  CAL3D_WRAPPER_API struct CalCoreModel *CalModel_GetCoreModel(struct CalModel *self);
  CAL3D_WRAPPER_API struct CalMesh *CalModel_GetMesh(struct CalModel *self, int coreMeshId);
  CAL3D_WRAPPER_API struct CalMixer *CalModel_GetMixer(struct CalModel *self);
  CAL3D_WRAPPER_API struct CalPhysique *CalModel_GetPhysique(struct CalModel *self);
  CAL3D_WRAPPER_API struct CalRenderer *CalModel_GetRenderer(struct CalModel *self);
  CAL3D_WRAPPER_API struct CalSkeleton *CalModel_GetSkeleton(struct CalModel *self);
  CAL3D_WRAPPER_API struct CalSpringSystem *CalModel_GetSpringSystem(struct CalModel *self);
//  CAL3D_WRAPPER_API std::vector<CalMesh *>& CalModel_GetVectorMesh(struct CalModel *self);
  CAL3D_WRAPPER_API struct CalModel *CalModel_New();
  CAL3D_WRAPPER_API void CalModel_SetLodLevel(struct CalModel *self, float lodLevel);
  CAL3D_WRAPPER_API void CalModel_SetMaterialSet(struct CalModel *self, int setId);
  CAL3D_WRAPPER_API void CalModel_Update(struct CalModel *self, float deltaTime);

//****************************************************************************//
// CalQuaternion wrapper functions declaration                                //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalQuaternion_Blend(struct CalQuaternion *self, float d, struct CalQuaternion *pQ);
  CAL3D_WRAPPER_API void CalQuaternion_Clear(struct CalQuaternion *self);
  CAL3D_WRAPPER_API void CalQuaternion_Conjugate(struct CalQuaternion *self);
  CAL3D_WRAPPER_API void CalQuaternion_Delete(struct CalQuaternion *self);
  CAL3D_WRAPPER_API void CalQuaternion_Equal(struct CalQuaternion *self, struct CalQuaternion *pQ);
  CAL3D_WRAPPER_API float *CalQuaternion_Get(struct CalQuaternion *self);
  CAL3D_WRAPPER_API void CalQuaternion_Multiply(struct CalQuaternion *self, struct CalQuaternion *pQ);
  CAL3D_WRAPPER_API void CalQuaternion_MultiplyVector(struct CalQuaternion *self, struct CalVector *pV);
  CAL3D_WRAPPER_API struct CalQuaternion *CalQuaternion_New();
  CAL3D_WRAPPER_API void CalQuaternion_Op_Multiply(struct CalQuaternion *pResult, struct CalQuaternion *pQ, struct CalQuaternion *pR);
  CAL3D_WRAPPER_API void CalQuaternion_Set(struct CalQuaternion *self, float qx, float qy, float qz, float qw);

//****************************************************************************//
// CalSaver wrapper functions declaration                                     //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalSaver_Delete(struct CalSaver *self);
  CAL3D_WRAPPER_API struct CalSaver *CalSaver_New();
  CAL3D_WRAPPER_API enum Boolean CalSaver_SaveCoreAnimation(struct CalSaver *self, char *strFilename, struct  CalCoreAnimation *pCoreAnimation);
  CAL3D_WRAPPER_API enum Boolean CalSaver_SaveCoreAnimatedMorph(struct CalSaver *self, char *strFilename, struct  CalCoreAnimatedMorph *pCoreAnimatedMorph);
  CAL3D_WRAPPER_API enum Boolean CalSaver_SaveCoreMaterial(struct CalSaver *self, char *strFilename, struct CalCoreMaterial *pCoreMaterial);
  CAL3D_WRAPPER_API enum Boolean CalSaver_SaveCoreMesh(struct CalSaver *self, char *strFilename, struct CalCoreMesh *pCoreMesh);
  CAL3D_WRAPPER_API enum Boolean CalSaver_SaveCoreSkeleton(struct CalSaver *self, char *strFilename, struct CalCoreSkeleton *pCoreSkeleton);

//****************************************************************************//
// CalSkeleton wrapper functions declaration                                  //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalSkeleton_CalculateState(struct CalSkeleton *self);
  CAL3D_WRAPPER_API void CalSkeleton_ClearState(struct CalSkeleton *self);
  CAL3D_WRAPPER_API void CalSkeleton_Create(struct CalSkeleton *self, struct CalCoreSkeleton *pCoreSkeleton);
  CAL3D_WRAPPER_API void CalSkeleton_Delete(struct CalSkeleton *self);
  CAL3D_WRAPPER_API void CalSkeleton_Destroy(struct CalSkeleton *self);
  CAL3D_WRAPPER_API struct CalBone *CalSkeleton_GetBone(struct CalSkeleton *self, int boneId);
  CAL3D_WRAPPER_API struct CalCoreSkeleton *CalSkeleton_GetCoreSkeleton(struct CalSkeleton *self);
//  CAL3D_WRAPPER_API std::vector<CalBone *>& CalSkeleton_GetVectorBone(struct CalSkeleton *self);
  CAL3D_WRAPPER_API void CalSkeleton_LockState(struct CalSkeleton *self);
  CAL3D_WRAPPER_API struct CalSkeleton *CalSkeleton_New();

  // DEBUG-CODE
  CAL3D_WRAPPER_API int CalSkeleton_GetBonePoints(struct CalSkeleton *self, float *pPoints);
  CAL3D_WRAPPER_API int CalSkeleton_GetBonePointsStatic(struct CalSkeleton *self, float *pPoints);
  CAL3D_WRAPPER_API int CalSkeleton_GetBoneLines(struct CalSkeleton *self, float *pLines);
  CAL3D_WRAPPER_API int CalSkeleton_GetBoneLinesStatic(struct CalSkeleton *self, float *pLines);

//****************************************************************************//
// CalSpringSystem wrapper functions declaration                              //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalSpringSystem_CalculateForces(struct CalSpringSystem *self, struct CalSubmesh *pSubmesh, float deltaTime);
  CAL3D_WRAPPER_API void CalSpringSystem_CalculateVertices(struct CalSpringSystem *self, struct CalSubmesh *pSubmesh, float deltaTime);
  CAL3D_WRAPPER_API void CalSpringSystem_Create(struct CalSpringSystem *self, struct CalModel *pModel);
  CAL3D_WRAPPER_API void CalSpringSystem_Delete(struct CalSpringSystem *self);
  CAL3D_WRAPPER_API void CalSpringSystem_Destroy(struct CalSpringSystem *self);
  CAL3D_WRAPPER_API struct CalSpringSystem *CalSpringSystem_New();
  CAL3D_WRAPPER_API void CalSpringSystem_Update(struct CalSpringSystem *self, float deltaTime);

//****************************************************************************//
// CalSubmesh wrapper functions declaration                                   //
//****************************************************************************//

  CAL3D_WRAPPER_API enum Boolean CalSubmesh_Create(struct CalSubmesh *self, struct CalCoreSubmesh *pCoreSubmesh);
  CAL3D_WRAPPER_API void CalSubmesh_Delete(struct CalSubmesh *self);
  CAL3D_WRAPPER_API struct CalCoreSubmesh *CalSubmesh_GetCoreSubmesh(struct CalSubmesh *self);
  CAL3D_WRAPPER_API int CalSubmesh_GetCoreMaterialId(struct CalSubmesh *self);
  CAL3D_WRAPPER_API int CalSubmesh_GetFaceCount(struct CalSubmesh *self);
  CAL3D_WRAPPER_API int CalSubmesh_GetFaces(struct CalSubmesh *self, CalIndex *pFaceBuffer);

//  CAL3D_WRAPPER_API std::vector<CalVector>& CalSubmesh_GetVectorNormal(struct CalSubmesh *self);
//  CAL3D_WRAPPER_API std::vector<CalSubmesh::PhysicalProperty>& CalSubmesh_GetVectorPhysicalProperty(struct CalSubmesh *self);
//  CAL3D_WRAPPER_API std::vector<CalVector>& CalSubmesh_GetVectorVertex(struct CalSubmesh *self);
  CAL3D_WRAPPER_API int CalSubmesh_GetVertexCount(struct CalSubmesh *self);
  CAL3D_WRAPPER_API enum Boolean CalSubmesh_HasInternalData(struct CalSubmesh *self);
  CAL3D_WRAPPER_API struct CalSubmesh *CalSubmesh_New();
  CAL3D_WRAPPER_API void CalSubmesh_SetCoreMaterialId(struct CalSubmesh *self, int coreMaterialId);
  CAL3D_WRAPPER_API void CalSubmesh_SetLodLevel(struct CalSubmesh *self, float lodLevel);
  CAL3D_WRAPPER_API int CalSubmesh_GetMorphTargetWeightCount(struct CalSubmesh *self);
  CAL3D_WRAPPER_API float CalSubmesh_GetBaseWeight(struct CalSubmesh *self);
  CAL3D_WRAPPER_API void CalSubmesh_SetMorphTargetWeight(struct CalSubmesh *self, int blendid, float weight);

//****************************************************************************//
// CalVector wrapper functions declaration                                    //
//****************************************************************************//

  CAL3D_WRAPPER_API void CalVector_Add(struct CalVector *self, struct CalVector *pV);
  CAL3D_WRAPPER_API void CalVector_Blend(struct CalVector *self, float d, struct CalVector *pV);
  CAL3D_WRAPPER_API void CalVector_Clear(struct CalVector *self);
  CAL3D_WRAPPER_API void CalVector_Delete(struct CalVector *self);
  CAL3D_WRAPPER_API void CalVector_Equal(struct CalVector *self, struct CalVector *pV);
  CAL3D_WRAPPER_API void CalVector_InverseScale(struct CalVector *self, float d);
  CAL3D_WRAPPER_API float *CalVector_Get(struct CalVector *self);
  CAL3D_WRAPPER_API float CalVector_Length(struct CalVector *self);
  CAL3D_WRAPPER_API struct CalVector *CalVector_New();
  CAL3D_WRAPPER_API float CalVector_Normalize(struct CalVector *self);
  CAL3D_WRAPPER_API void CalVector_Op_Add(struct CalVector *pResult, struct CalVector *pV, struct CalVector *pU);
  CAL3D_WRAPPER_API void CalVector_Op_Subtract(struct CalVector *pResult, struct CalVector *pV, struct CalVector *pU);
  CAL3D_WRAPPER_API void CalVector_CalVector_Op_Scale(struct CalVector *pResult, struct CalVector *pV, float d);
  CAL3D_WRAPPER_API void CalVector_CalVector_Op_InverseScale(struct CalVector *pResult, struct CalVector *pV, float d);
  CAL3D_WRAPPER_API float CalVector_Op_Scalar(struct CalVector *pV, struct CalVector *pU);
  CAL3D_WRAPPER_API void CalVector_Op_Cross(struct CalVector *pResult, struct CalVector *pV, struct CalVector *pU);
  CAL3D_WRAPPER_API void CalVector_Scale(struct CalVector *self, float d);
  CAL3D_WRAPPER_API void CalVector_Set(struct CalVector *self, float vx, float vy, float vz);
  CAL3D_WRAPPER_API void CalVector_Subtract(struct CalVector *self, struct CalVector *pV);
  CAL3D_WRAPPER_API void CalVector_Transform(struct CalVector *self, struct CalQuaternion *pQ);

#ifdef __cplusplus
}
#endif
