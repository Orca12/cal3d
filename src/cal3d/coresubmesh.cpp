//****************************************************************************//
// coresubmesh.cpp                                                            //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/coresubmesh.h"
#include "cal3d/coresubmorphtarget.h"

 /*****************************************************************************/
/** Constructs the core submesh instance.
  *
  * This function is the default constructor of the core submesh instance.
  *****************************************************************************/

CalCoreSubmesh::CalCoreSubmesh(int vertexCount, int textureCoordinateCount, int faceCount)
  : m_coreMaterialThreadId(0)
  , m_lodCount(0)
  , m_isStatic(false)
  , m_currentVertexId(0)
{
  m_hasNonWhiteVertexColors = false;

  // reserve the space needed in all the vectors
  m_vertices.resize(vertexCount);
  m_vertexColors.resize(vertexCount);
  m_lodData.resize(vertexCount);
  m_influenceRanges.resize(vertexCount);

  m_vectorvectorTextureCoordinate.resize(textureCoordinateCount);
  for(int textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; ++textureCoordinateId)
  {
    m_vectorvectorTextureCoordinate[textureCoordinateId].resize(vertexCount);
  }

  m_vectorFace.resize(faceCount);
}

 /*****************************************************************************/
/** Destructs the core submesh instance.
  *
  * This function is the destructor of the core submesh instance.
  *****************************************************************************/

void CalCoreSubmesh::setSubMorphTargetGroupIndexArray( unsigned int len, unsigned int const * indexArray )
{
  m_vectorSubMorphTargetGroupIndex.reserve( len );
  m_vectorSubMorphTargetGroupIndex.resize( len );
  unsigned int i;
  for( i = 0; i < len; i++ ) {
    m_vectorSubMorphTargetGroupIndex[ i ] = indexArray[ i ];
  }
}

template<typename T>
size_t vectorSize(const SSEArray<T>& v) {
  return sizeof(T) * v.size();
}

template<typename T>
size_t vectorSize(const std::vector<T>& v) {
  return sizeof(T) * v.size();
}

size_t CalCoreSubmesh::sizeWithoutSubMorphTargets()
{
  unsigned int r = sizeof( CalCoreSubmesh );
  r += vectorSize(m_vertices);
  r += vectorSize(m_vertexColors);
  r += vectorSize(m_vectorFace);
  r += vectorSize(m_vectorSubMorphTargetGroupIndex);
  r += vectorSize(m_lodData);
  std::vector<std::vector<TextureCoordinate> >::iterator iter3;
  for( iter3 = m_vectorvectorTextureCoordinate.begin(); iter3 != m_vectorvectorTextureCoordinate.end(); ++iter3 ) {
    r += sizeof( TextureCoordinate ) * (*iter3).size();
  }
  return r;
}


size_t CalCoreSubmesh::size()
{
  unsigned int r = sizeWithoutSubMorphTargets();
  CoreSubMorphTargetVector::iterator iter1;
  for( iter1 = m_vectorCoreSubMorphTarget.begin(); iter1 != m_vectorCoreSubMorphTarget.end(); ++iter1 ) {
    r += (*iter1)->size();
  }
  return r;
}


 /*****************************************************************************/
/** Returns the ID of the core material thread.
  *
  * This function returns the ID of the core material thread of this core
  * submesh instance.
  *
  * @return The ID of the core material thread.
  *****************************************************************************/

int CalCoreSubmesh::getCoreMaterialThreadId()
{
  return m_coreMaterialThreadId;
}

 /*****************************************************************************/
/** Returns the number of faces.
  *
  * This function returns the number of faces in the core submesh instance.
  *
  * @return The number of faces.
  *****************************************************************************/

int CalCoreSubmesh::getFaceCount()
{
  return m_vectorFace.size();
}

 /*****************************************************************************/
/** Returns the number of LOD steps.
  *
  * This function returns the number of LOD steps in the core submesh instance.
  *
  * @return The number of LOD steps.
  *****************************************************************************/

int CalCoreSubmesh::getLodCount()
{
  return m_lodCount;
}

const std::vector<CalCoreSubmesh::Face>& CalCoreSubmesh::getVectorFace() const
{
  return m_vectorFace;
}

int CalCoreSubmesh::getVertexCount()
{
  return m_vertices.size();
}

 /*****************************************************************************/
/** Sets the ID of the core material thread.
  *
  * This function sets the ID of the core material thread of the core submesh
  * instance.
  *
  * @param coreMaterialThreadId The ID of the core material thread that should
  *                             be set.
  *****************************************************************************/

void CalCoreSubmesh::setCoreMaterialThreadId(int coreMaterialThreadId)
{
  m_coreMaterialThreadId = coreMaterialThreadId;
}

 /*****************************************************************************/
/** Sets a specified face.
  *
  * This function sets a specified face in the core submesh instance.
  *
  * @param faceId  The ID of the face.
  * @param face The face that should be set.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreSubmesh::setFace(int faceId, const Face& face)
{
  if((faceId < 0) || (faceId >= (int)m_vectorFace.size())) return false;

  m_vectorFace[faceId] = face;

  return true;
}

 /*****************************************************************************/
/** Sets the number of LOD steps.
  *
  * This function sets the number of LOD steps of the core submesh instance.
  *
  * @param lodCount The number of LOD steps that should be set.
  *****************************************************************************/

void CalCoreSubmesh::setLodCount(int lodCount)
{
  m_lodCount = lodCount;
}

 /*****************************************************************************/
/** Sets a specified texture coordinate.
  *
  * This function sets a specified texture coordinate in the core submesh
  * instance.
  *
  * @param vertexId  The ID of the vertex.
  * @param textureCoordinateId  The ID of the texture coordinate.
  * @param textureCoordinate The texture coordinate that should be set.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreSubmesh::setTextureCoordinate(int vertexId, int textureCoordinateId, const TextureCoordinate& textureCoordinate)
{
  if((textureCoordinateId < 0) || (textureCoordinateId >= (int)m_vectorvectorTextureCoordinate.size())) return false;
  if((vertexId < 0) || (vertexId >= (int)m_vectorvectorTextureCoordinate[textureCoordinateId].size())) return false;

  m_vectorvectorTextureCoordinate[textureCoordinateId][vertexId] = textureCoordinate;

  return true;
}

void CalCoreSubmesh::addVertex(const Vertex& vertex, CalColor32 vertexColor, const LodData& lodData, std::vector<Influence> inf) {
  assert(m_currentVertexId < m_vertices.size());

  const int vertexId = m_currentVertexId++;
  if (vertexId == 0) {
    m_isStatic = true;
    m_staticInfluenceSet = inf;
  } else if (m_isStatic) {
    m_isStatic = m_staticInfluenceSet == inf;
  }

  m_vertices[vertexId] = vertex;
  m_vertexColors[vertexId] = vertexColor;
  m_lodData[vertexId] = lodData;

  // Each vertex needs at least one influence.
  if (inf.empty()) {
    m_isStatic = false;
    Influence i;
    i.boneId = 0;
    i.weight = 0.0f;
    inf.push_back(i);
  }

  // Mark the last influence as the last one.  :)
  for (size_t i = 0; i + 1 < inf.size(); ++i) {
    inf[i].lastInfluenceForThisVertex = 0;
  }
  inf[inf.size() - 1].lastInfluenceForThisVertex = 1;

  m_influenceRanges[vertexId].influenceStart = influences.size();
  m_influenceRanges[vertexId].influenceEnd   = influences.size() + inf.size();

  influences.insert(influences.end(), inf.begin(), inf.end());
}

 /*****************************************************************************/
/** Adds a core sub morph target.
  *
  * This function adds a core sub morph target to the core sub mesh instance.
  *
  * @param pCoreSubMorphTarget A pointer to the core sub morph target that should be added.
  *
  * @return One of the following values:
  *         \li the assigned sub morph target \b ID of the added core sub morph target
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreSubmesh::addCoreSubMorphTarget(boost::shared_ptr<CalCoreSubMorphTarget> pCoreSubMorphTarget)
{
  // get next sub morph target id
  int subMorphTargetId;
  subMorphTargetId = m_vectorCoreSubMorphTarget.size();

  m_vectorCoreSubMorphTarget.push_back(pCoreSubMorphTarget);

  return subMorphTargetId;
}

int CalCoreSubmesh::getCoreSubMorphTargetCount()
{
  return m_vectorCoreSubMorphTarget.size();
}

CalCoreSubmesh::CoreSubMorphTargetVector& CalCoreSubmesh::getVectorCoreSubMorphTarget()
{
  return m_vectorCoreSubMorphTarget;
}

void CalCoreSubmesh::scale(float factor)
{
  for(int vertexId = 0; vertexId < m_vertices.size(); vertexId++) {
    m_vertices[vertexId].position*=factor;		
  }
}

bool CalCoreSubmesh::isStatic() const {
  return m_isStatic && m_vectorCoreSubMorphTarget.empty();
}

BoneTransform CalCoreSubmesh::getStaticTransform(const BoneTransform* bones) const {
  BoneTransform rm;

  std::set<Influence>::const_iterator current = m_staticInfluenceSet.influences.begin();
  while (current != m_staticInfluenceSet.influences.end()) {
    const BoneTransform& influence = bones[current->boneId];
    rm.rowx.x += current->weight * influence.rowx.x;
    rm.rowx.y += current->weight * influence.rowx.y;
    rm.rowx.z += current->weight * influence.rowx.z;
    rm.rowx.w += current->weight * influence.rowx.w;

    rm.rowy.x += current->weight * influence.rowy.x;
    rm.rowy.y += current->weight * influence.rowy.y;
    rm.rowy.z += current->weight * influence.rowy.z;
    rm.rowy.w += current->weight * influence.rowy.w;

    rm.rowz.x += current->weight * influence.rowz.x;
    rm.rowz.y += current->weight * influence.rowz.y;
    rm.rowz.z += current->weight * influence.rowz.z;
    rm.rowz.w += current->weight * influence.rowz.w;

    ++current;
  }

  return rm;
}
