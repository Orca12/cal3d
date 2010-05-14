//----------------------------------------------------------------------------//
// Exporter.cpp                                                               //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "StdAfx.h"
#include "cal3d/corematerial.h"
#include "Exporter.h"
#include "BaseInterface.h"
#include "SkeletonCandidate.h"
#include "BoneCandidate.h"
#include "BaseNode.h"
#include "BaseMesh.h"
#include "SkeletonExportSheet.h"
#include "AnimationExportSheet.h"
#include "MorphAnimationExportSheet.h"
#include "MeshExportSheet.h"
#include "MaterialExportSheet.h"
#include "MeshCandidate.h"
#include "MorphAnimationCandidate.h"
#include "SubmeshCandidate.h"
#include "VertexCandidate.h"
#include "MaterialLibraryCandidate.h"
#include "MaterialCandidate.h"

//----------------------------------------------------------------------------//
// The one and only exporter instance                                         //
//----------------------------------------------------------------------------//

CExporter theExporter;

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CExporter::CExporter()
{
    m_pInterface = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CExporter::~CExporter()
{
}

//----------------------------------------------------------------------------//
// Create an exporter instance for a given interface                          //
//----------------------------------------------------------------------------//

bool CExporter::Create(CBaseInterface *pInterface)
{
    ::OutputDebugString("CExporter::Create()\n");
    // check if a valid interface is set
    if(pInterface == 0)
    {
        SetLastError("Invalid handle.", __FILE__, __LINE__);
        return false;
    }

    m_pInterface = pInterface;

    return true;
}

//----------------------------------------------------------------------------//
// Export the animation to a given file                                       //
//----------------------------------------------------------------------------//

bool CExporter::ExportAnimation(const std::string& strFilename)
{
    // check if a valid interface is set
    if(m_pInterface == 0)
    {
        SetLastError("Invalid handle.", __FILE__, __LINE__);
        return false;
    }

    // build a skeleton candidate
    CSkeletonCandidate skeletonCandidate;

    // show export wizard sheet
    CAnimationExportSheet sheet(m_pInterface, _T("Cal3D Animation Export"), m_pInterface->GetMainWnd());
    sheet.SetSkeletonCandidate(&skeletonCandidate);
    sheet.SetAnimationTime(m_pInterface->GetStartFrame(), m_pInterface->GetEndFrame(), m_pInterface->GetCurrentFrame(), m_pInterface->GetFps());
    sheet.SetWizardMode();
    if(sheet.DoModal() != ID_WIZFINISH) return true;

    // get the number of selected bone candidates
    int selectedCount;
    selectedCount = skeletonCandidate.GetSelectedCount();
    if(selectedCount == 0)
    {
        SetLastError("No bones selected to export.", __FILE__, __LINE__);
        return false;
    }

    // create the core animation instance
    CalCoreAnimation coreAnimation;

    // set the duration of the animation
    float duration;
    duration = (float)(sheet.GetEndFrame() - sheet.GetStartFrame()) / (float)m_pInterface->GetFps();
    coreAnimation.setDuration(duration);

    // get bone candidate vector
    std::vector<CBoneCandidate *>& vectorBoneCandidate = skeletonCandidate.GetVectorBoneCandidate();

    size_t boneCandidateId;
    for(boneCandidateId = 0; boneCandidateId < vectorBoneCandidate.size(); boneCandidateId++)
    {
        // get the bone candidate
        CBoneCandidate *pBoneCandidate;
        pBoneCandidate = vectorBoneCandidate[boneCandidateId];

        // only create tracks for the selected bone candidates
        if(pBoneCandidate->IsSelected())
        {
            // allocate new core track instance
            CalCoreTrack *pCoreTrack;
            pCoreTrack = new CalCoreTrack();
            if(pCoreTrack == 0)
            {
                SetLastError("Memory allocation failed.", __FILE__, __LINE__);
                return false;
            }

            // create the core track instance
            pCoreTrack->create();

            // set the core bone id
            pCoreTrack->setCoreBoneId(boneCandidateId);

            // add the core track to the core animation instance
            if(!coreAnimation.addCoreTrack(pCoreTrack))
            {
                SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
                delete pCoreTrack;
                return false;
            }
        }
    }

    // start the progress info
    CStackProgress progress(m_pInterface, "Exporting to animation file...");

    // calculate the end frame
    int endFrame;
    endFrame = (int)(duration * (float)sheet.GetFps() + 0.5f);

    // calculate the displaced frame
    int displacedFrame;
    displacedFrame = (int)(((float)sheet.GetDisplacement() / (float)m_pInterface->GetFps()) * (float)sheet.GetFps() + 0.5f) % endFrame;

    // calculate the possible wrap frame
    int wrapFrame;
    wrapFrame = (displacedFrame > 0) ? 1 : 0;
    float wrapTime;
    wrapTime = 0.0f;

    int frame;
    int outputFrame;
    for(frame = 0,  outputFrame = 0; frame <= (endFrame + wrapFrame); frame++)
    {
        // update the progress info
        m_pInterface->SetProgressInfo(int(100.0f * (float)frame / (float)(endFrame + wrapFrame + 1)));

        // calculate the time in seconds
        float time;
        time = (float)sheet.GetStartFrame() / (float)m_pInterface->GetFps() + (float)displacedFrame / (float)sheet.GetFps();

        /* DEBUG
CString str;
str.Format("frame=%d, endframe=%d, disframe=%d, ouputFrame=%d (%f), time=%f\n", frame, endFrame, displacedFrame, outputFrame, (float)outputFrame / (float)sheet.GetFps() + wrapTime, time);
OutputDebugString(str);
         */

        for(boneCandidateId = 0; boneCandidateId < vectorBoneCandidate.size(); boneCandidateId++)
        {
            // get the bone candidate
            CBoneCandidate *pBoneCandidate;
            pBoneCandidate = vectorBoneCandidate[boneCandidateId];

            // only export keyframes for the selected bone candidates
            if(pBoneCandidate->IsSelected())
            {
                CalCoreKeyframe coreKeyframe;

                // set the frame time
                coreKeyframe.setTime((float)outputFrame / (float)sheet.GetFps() + wrapTime);

                // get the translation and the rotation of the bone candidate
                CalVector translation;
                CalQuaternion rotation;
                skeletonCandidate.GetTranslationAndRotation(boneCandidateId, time, translation, rotation);

                // set the translation and rotation
                coreKeyframe.setTranslation(translation);
                coreKeyframe.setRotation(rotation);

                // get the core track for this bone candidate
                CalCoreTrack *pCoreTrack;
                pCoreTrack = coreAnimation.getCoreTrack(pBoneCandidate->GetId());
                if(pCoreTrack == 0)
                {
                    SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
                    return false;
                }

                // add this core keyframe to the core track
                pCoreTrack->addCoreKeyframe(coreKeyframe);
            }
        }

        // calculate the next displaced frame and its frame time
        if(wrapFrame > 0)
        {
            if(displacedFrame == endFrame)
            {
                wrapTime = 0.0001f;
                displacedFrame = 0;
            }
            else
            {
                wrapTime = 0.0f;
                outputFrame++;
                displacedFrame++;
            }
        }
        else
        {
            outputFrame++;
            displacedFrame++;
        }
    }

    for(boneCandidateId = 0; boneCandidateId < vectorBoneCandidate.size(); boneCandidateId++)
    {
        CBoneCandidate *pBoneCandidate;
        pBoneCandidate = vectorBoneCandidate[boneCandidateId];
        CalCoreTrack *pCoreTrack;
        pCoreTrack = coreAnimation.getCoreTrack(pBoneCandidate->GetId());
        static bool useCompression = true;
        static double translationTolerance = 0.05;
        static double rotationToleranceDegrees = 0.1;
        // there is no pCoreTrack for bones that are deselected
        if( pCoreTrack && useCompression ) {
            CalCoreSkeleton * skelOrNull = skeletonCandidate.GetCoreSkeleton();
            pCoreTrack->compress(translationTolerance, rotationToleranceDegrees, skelOrNull );
        }
    }

    // save core animation to the file
    if(!CalSaver::saveCoreAnimation(strFilename, &coreAnimation))
    {
        SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
        return false;
    }

    return true;
}

bool CExporter::ExportMorphAnimation(const std::string& strFilename)
{
    // check if a valid interface is set
    if(m_pInterface == 0)
    {
        SetLastError("Invalid handle.", __FILE__, __LINE__);
        return false;
    }

    // build a morph animation candidate
    CMorphAnimationCandidate morphAnimationCandidate;
    if( !morphAnimationCandidate.Create() ) {
        SetLastError("Creation of CMorphAnimationCandidate instance failed.", __FILE__, __LINE__);
        return false;
    }

    // show export wizard sheet
    CMorphAnimationExportSheet sheet(_T("Cal3D Animation Export"), m_pInterface->GetMainWnd());
    sheet.SetMorphAnimationTime(m_pInterface->GetStartFrame(), m_pInterface->GetEndFrame(), m_pInterface->GetCurrentFrame(), m_pInterface->GetFps());
    sheet.SetWizardMode();
    if(sheet.DoModal() != ID_WIZFINISH) return true;

    // create the core animation instance
    CalCoreAnimatedMorph coreAnimation;

    // set the duration of the animation
    float duration;
    duration = (float)(sheet.GetEndFrame() - sheet.GetStartFrame()) / (float)m_pInterface->GetFps();
    coreAnimation.setDuration(duration);


    // find the selected mesh
    // find the morpher modifier
    // foreach channel
    //   create a morph anim track
    //   set the name
    // foreach frame
    //   foreach channel
    //     create keyframe
    //     set value from modifier

    // start the progress info
    CStackProgress progress(m_pInterface, "Exporting to animation file...");

    CBaseMesh * pMesh = morphAnimationCandidate.meshAtTime(-1);
    if( !pMesh ) {
        ::OutputDebugString("No mesh found.\n");
        return false;
    }
    int numMC = pMesh->numMorphChannels();
    for( int i = 0; i < numMC; i++ ) {
        CalCoreMorphTrack * pTrack = new CalCoreMorphTrack();
        if( !pTrack->create() ) {
            SetLastError("Creation of CalCoreMorphTrack instance failed.", __FILE__, __LINE__);
            return false;
        }
        CBaseMesh::MorphKeyFrame keyFrame = pMesh->frameForChannel(i, 0);
        pTrack->setMorphName(keyFrame.name);
        coreAnimation.addCoreTrack(pTrack);
    }

    // calculate the end frame
    int endFrame;
    endFrame = (int)(duration * (float)sheet.GetFps() + 0.5f);

    // calculate the displaced frame
    int displacedFrame;
    displacedFrame = (int)(((float)sheet.GetDisplacement() / (float)m_pInterface->GetFps()) * (float)sheet.GetFps() + 0.5f) % endFrame;

    // calculate the possible wrap frame
    int wrapFrame;
    wrapFrame = (displacedFrame > 0) ? 1 : 0;
    float wrapTime;
    wrapTime = 0.0f;

    int frame;
    int outputFrame;
    {
        std::stringstream ss;
        ss << "Writing " << (endFrame + wrapFrame) << " frames of animation." << std::endl;
        ::OutputDebugString(ss.str().c_str());
    }
    for(frame = 0,  outputFrame = 0; frame <= (endFrame + wrapFrame); frame++)
    {
        // update the progress info
        m_pInterface->SetProgressInfo(int(100.0f * (float)frame / (float)(endFrame + wrapFrame + 1)));

        // calculate the time in seconds
        float time;
        time = (float)sheet.GetStartFrame() / (float)m_pInterface->GetFps() + (float)displacedFrame / (float)sheet.GetFps();

        /* DEBUG
             CString str;
             str.Format("frame=%d, endframe=%d, disframe=%d, ouputFrame=%d (%f), time=%f\n", frame, endFrame, displacedFrame, outputFrame, (float)outputFrame / (float)sheet.GetFps() + wrapTime, time);
             OutputDebugString(str);
         */


        for( int i = 0; i < numMC; i++ ) {
            CBaseMesh::MorphKeyFrame keyFrame = pMesh->frameForChannel(i, time);
            CalCoreMorphTrack * pTrack = coreAnimation.getCoreTrack(keyFrame.name);
            CalCoreMorphKeyframe * pFrame = new CalCoreMorphKeyframe();
            if(!pFrame->create()) {
                SetLastError("Creation of CalCoreMorphKeyframe instance failed.", __FILE__, __LINE__);
                return false;
            }
            pFrame->setTime(keyFrame.time);
            pFrame->setWeight(keyFrame.weight / keyFrame.totalWeight);
            if(!pTrack->addCoreMorphKeyframe(pFrame)) {
                SetLastError(" addCoreMorphKeyframe failed.", __FILE__, __LINE__);
                pFrame->destroy();
                return false;
            }
        }

        // calculate the next displaced frame and its frame time
        if(wrapFrame > 0)
        {
            if(displacedFrame == endFrame)
            {
                wrapTime = 0.0001f;
                displacedFrame = 0;
            }
            else
            {
                wrapTime = 0.0f;
                outputFrame++;
                displacedFrame++;
            }
        }
        else
        {
            outputFrame++;
            displacedFrame++;
        }
    }

    // save core animation to the file
    ::OutputDebugString("Writing to output file...\n");
    if(!CalSaver::saveCoreAnimatedMorph(strFilename, &coreAnimation))
    {
        SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
        return false;
    }

    // destroy the core animation
    ::OutputDebugString("Writing complete.\n");

    return true;
}

//----------------------------------------------------------------------------//
// Export the material to a given file                                        //
//----------------------------------------------------------------------------//

bool CExporter::ExportMaterial(const std::string& strFilename)
{
    // check if a valid interface is set
    if(m_pInterface == 0)
    {
        SetLastError("Invalid handle.", __FILE__, __LINE__);
        return false;
    }

    // check if a valid interface is set
    if(m_pInterface == 0)
    {
        SetLastError("Invalid handle.", __FILE__, __LINE__);
        return false;
    }

    // build a material library candidate
    CMaterialLibraryCandidate materialLibraryCandidate;
    if(!materialLibraryCandidate.CreateFromInterface()) return false;

    // show export wizard sheet
    CMaterialExportSheet sheet("Cal3D Material Export", m_pInterface->GetMainWnd());
    sheet.SetMaterialLibraryCandidate(&materialLibraryCandidate);
    sheet.SetWizardMode();
    if(sheet.DoModal() != ID_WIZFINISH) return true;

    // get selected material candidate
    CMaterialCandidate *pMaterialCandidate;
    pMaterialCandidate = materialLibraryCandidate.GetSelectedMaterialCandidate();
    if(pMaterialCandidate == 0)
    {
        SetLastError("No material selected.", __FILE__, __LINE__);
        return false;
    }

    // create the core material instance
    CalCoreMaterial coreMaterial;
    // set the ambient color
    CalCoreMaterial::Color coreColor;
    float color[4];
    pMaterialCandidate->GetAmbientColor(&color[0]);
    coreColor.red = (unsigned char)(255.0f * color[0]);
    coreColor.green = (unsigned char)(255.0f * color[1]);
    coreColor.blue = (unsigned char)(255.0f * color[2]);
    coreColor.alpha = (unsigned char)(255.0f * color[3]);
    coreMaterial.setAmbientColor(coreColor);


    // set the diffuse color
    pMaterialCandidate->GetDiffuseColor(&color[0]);
    coreColor.red = (unsigned char)(255.0f * color[0]);
    coreColor.green = (unsigned char)(255.0f * color[1]);
    coreColor.blue = (unsigned char)(255.0f * color[2]);
    coreColor.alpha = (unsigned char)(255.0f * color[3]);
    coreMaterial.setDiffuseColor(coreColor);

    // set the specular color
    pMaterialCandidate->GetSpecularColor(&color[0]);
    coreColor.red = (unsigned char)(255.0f * color[0]);
    coreColor.green = (unsigned char)(255.0f * color[1]);
    coreColor.blue = (unsigned char)(255.0f * color[2]);
    coreColor.alpha = (unsigned char)(255.0f * color[3]);
    coreMaterial.setSpecularColor(coreColor);

    // set the shininess factor
    coreMaterial.setShininess(pMaterialCandidate->GetShininess());

    // get the map vector of the material candidate
    std::vector<CMaterialCandidate::Map>& vectorMap = pMaterialCandidate->GetVectorMap();

    // reserve memory for all the material data
    coreMaterial.reserve(vectorMap.size());

    // load all maps
    for(size_t mapId = 0; mapId < vectorMap.size(); mapId++)
    {
        CalCoreMaterial::Map map;

        // set map data
        map.strFilename = vectorMap[mapId].strFilename;
        map.mapType = vectorMap[mapId].mapType;

        // set map in the core material instance
        coreMaterial.setMap(mapId, map);
    }

    // save core mesh to the file
    if(!CalSaver::saveCoreMaterial(strFilename, &coreMaterial))
    {
        SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------//
// Export the mesh to a given file                                            //
//----------------------------------------------------------------------------//

bool CExporter::ExportMesh(const std::string& strFilename)
{
    // check if a valid interface is set
    if(m_pInterface == 0)
    {
        SetLastError("Invalid handle.", __FILE__, __LINE__);
        return false;
    }

    // build a mesh candidate
    CMeshCandidate meshCandidate;

    // build a skeleton candidate
    CSkeletonCandidate skeletonCandidate;

    // show export wizard sheet
    CMeshExportSheet sheet("Cal3D Mesh Export", m_pInterface->GetMainWnd());
    sheet.SetSkeletonCandidate(&skeletonCandidate);
    sheet.SetMeshCandidate(&meshCandidate);
    sheet.SetWizardMode();
    if(sheet.DoModal() != ID_WIZFINISH) return true;

    // create the core mesh instance
    CalCoreMesh coreMesh;

    CalVector zero;
    bool r = meshCandidateToCoreMesh(meshCandidate, coreMesh, zero);
    if( !r ) {
        return false;
    }

    int numMorphs = meshCandidate.numMorphs();
    for( int morphI = 0; morphI < numMorphs; morphI++ ) {
        CMeshCandidate morphCandidate;

        CBaseNode * morphNode = meshCandidate.nthMorphNode(morphI);
        if(!morphCandidate.Create(morphNode, &skeletonCandidate, sheet.GetMaxBoneCount(),
                sheet.GetWeightThreshold()))
        {
            SetLastError("Creation of core mesh instance failed.", __FILE__, __LINE__);
            return false;
        }

        if( !morphCandidate.DisableLOD() ) {
            SetLastError("CalculateLOD failed.", __FILE__, __LINE__);
            return false;
        }

        CalCoreMesh morphCoreMesh;

        CalVector trans;
        CalQuaternion rot;
        m_pInterface->GetTranslationAndRotation(morphNode, meshCandidate.getNode(),
                0, trans, rot);

        if( !meshCandidateToCoreMesh(morphCandidate, morphCoreMesh, trans) ) {
            SetLastError("Creation of core mesh instance failed.", __FILE__, __LINE__);
            return false;
        }
        if( coreMesh.addAsMorphTarget(&morphCoreMesh, morphNode->GetName()) == -1 ) {
            SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
            return false;
        }

    }

    // save core mesh to the file
    if(!CalSaver::saveCoreMesh(strFilename, &coreMesh))
    {
        SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
        return false;
    }

    return true;
}

bool CExporter::meshCandidateToCoreMesh(CMeshCandidate const & meshCandidate, CalCoreMesh & coreMesh,
        CalVector const & positionOffset)
{
    // get the submesh candidate vector
    std::vector<CSubmeshCandidate *> const & vectorSubmeshCandidate = meshCandidate.GetVectorSubmeshCandidate();

    // start the progress info
    CStackProgress progress(m_pInterface, "Exporting to mesh file...");

    size_t submeshCandidateId;
    for(submeshCandidateId = 0; submeshCandidateId < vectorSubmeshCandidate.size(); submeshCandidateId++)
    {
        // update the progress info
        m_pInterface->SetProgressInfo(int(100.0f * (float)submeshCandidateId / (float)vectorSubmeshCandidate.size()));

        // get the submesh candidate
        CSubmeshCandidate *pSubmeshCandidate;
        pSubmeshCandidate = vectorSubmeshCandidate[submeshCandidateId];

        // get the face vector
        std::vector<CSubmeshCandidate::Face>& vectorFace = pSubmeshCandidate->GetVectorFace();

        // check if the submesh actually contains faces
        if(vectorFace.size() > 0)
        {
            // get the vertex candidate vector
            std::vector<CVertexCandidate *>& vectorVertexCandidate = pSubmeshCandidate->GetVectorVertexCandidate();

            // allocate new core submesh instance
            CalCoreSubmesh *pCoreSubmesh = new CalCoreSubmesh(
                    vectorVertexCandidate.size(),
                    pSubmeshCandidate->GetMapCount(),
                    vectorFace.size());

            // set the core material id
            pCoreSubmesh->setCoreMaterialThreadId(pSubmeshCandidate->GetMaterialThreadId());

            size_t vertexCandidateId;
            for(vertexCandidateId = 0; vertexCandidateId < vectorVertexCandidate.size(); vertexCandidateId++)
            {
                // get the vertex candidate
                CVertexCandidate *pVertexCandidate;
                pVertexCandidate = vectorVertexCandidate[vertexCandidateId];

                CalCoreSubmesh::Vertex vertex;
                CalColor32 vertexColor;
                CalCoreSubmesh::LodData lodData;

                // set the vertex position
                CalVector position;
                pVertexCandidate->GetPosition(position);
                position -= positionOffset;
                vertex.position.setAsPoint(position);

                // set the vertex color
                CalVector vc;
                pVertexCandidate->GetVertColor(vc);
                vertexColor = CalMakeColor(vc);

                // set the vertex normal
                CalVector normal;
                pVertexCandidate->GetNormal(normal);
                vertex.normal.setAsVector(normal);

                // set the collapse id
                lodData.collapseId = pVertexCandidate->GetCollapseId();

                // set the face collapse count
                lodData.faceCollapseCount = pVertexCandidate->GetFaceCollapseCount();

                // get the texture coordinate vector
                std::vector<CVertexCandidate::TextureCoordinate>& vectorTextureCoordinate = pVertexCandidate->GetVectorTextureCoordinate();

                // set all texture coordinates
                size_t textureCoordinateId;
                for(textureCoordinateId = 0; textureCoordinateId < vectorTextureCoordinate.size(); textureCoordinateId++)
                {
                    CalCoreSubmesh::TextureCoordinate textureCoordinate;
                    textureCoordinate.u = vectorTextureCoordinate[textureCoordinateId].u;
                    textureCoordinate.v = vectorTextureCoordinate[textureCoordinateId].v;

                    // set texture coordinate
                    pCoreSubmesh->setTextureCoordinate(pVertexCandidate->GetLodId(), textureCoordinateId, textureCoordinate);
                }

                // get the influence vector
                std::vector<CalCoreSubmesh::Influence>& vectorInfluence = pVertexCandidate->GetVectorInfluence();

                // set vertex in the core submesh instance
                // Chad says that this might work. -- andy 4 June 2009
                //assert(false); // Not sure exactly how this call should work.
                //pCoreSubmesh->setVertex(pVertexCandidate->GetLodId(), vertex, vertexColor, lodData, vectorInfluence);
                pCoreSubmesh->addVertex(vertex, vertexColor, lodData, vectorInfluence);
            }

            size_t faceId;
            for(faceId = 0; faceId < vectorFace.size(); faceId++)
            {
                CalCoreSubmesh::Face face;

                // set the vertex ids
                face.vertexId[0] = vectorFace[faceId].vertexLodId[0];
                face.vertexId[1] = vectorFace[faceId].vertexLodId[1];
                face.vertexId[2] = vectorFace[faceId].vertexLodId[2];

                // set face in the core submesh instance
                pCoreSubmesh->setFace(vectorFace[faceId].lodId, face);
            }

            // set the LOD step count
            pCoreSubmesh->setLodCount(pSubmeshCandidate->GetLodCount());

            // add the core submesh to the core mesh instance
            coreMesh.addCoreSubmesh(pCoreSubmesh);
        }
    }

    return true;
}

//----------------------------------------------------------------------------//
// Export the skeleton to a given file                                        //
//----------------------------------------------------------------------------//

bool CExporter::ExportSkeleton(const std::string& strFilename)
{
    // check if a valid interface is set
    if(m_pInterface == 0)
    {
        SetLastError("Invalid handle.", __FILE__, __LINE__);
        return false;
    }

    // build a skeleton candidate
    CSkeletonCandidate skeletonCandidate;
    if(!skeletonCandidate.CreateFromInterface()) return false;

    // show export wizard sheet
    CSkeletonExportSheet sheet(m_pInterface, "Cal3D Skeleton Export", m_pInterface->GetMainWnd());
    sheet.SetSkeletonCandidate(&skeletonCandidate);
    sheet.SetWizardMode();
    if(sheet.DoModal() != ID_WIZFINISH) return true;

    // build the selected ids of the bone candidates
    int selectedCount;
    selectedCount = skeletonCandidate.BuildSelectedId();
    if(selectedCount == 0)
    {
        SetLastError("No bones selected to export.", __FILE__, __LINE__);
        return false;
    }

    // create the core skeleton instance
    CalCoreSkeleton coreSkeleton;

    // get bone candidate vector
    std::vector<CBoneCandidate *>& vectorBoneCandidate = skeletonCandidate.GetVectorBoneCandidate();

    CalVector sceneAmbientColor;
    m_pInterface->GetAmbientLight( sceneAmbientColor );
    coreSkeleton.setSceneAmbientColor( sceneAmbientColor );

    // start the progress info
    CStackProgress progress(m_pInterface, "Exporting to skeleton file...");

    size_t boneCandidateId;
    int selectedId;
    for(boneCandidateId = 0, selectedId = 0; boneCandidateId < vectorBoneCandidate.size(); boneCandidateId++)
    {
        // get the bone candidate
        CBoneCandidate *pBoneCandidate;
        pBoneCandidate = vectorBoneCandidate[boneCandidateId];

        // only export selected bone candidates
        if(pBoneCandidate->IsSelected())
        {
            // update the progress info
            m_pInterface->SetProgressInfo(int(100.0f * (selectedId + 1) / selectedCount));
            selectedId++;

            // allocate new core bone instance
            CalCoreBone *pCoreBone = new CalCoreBone(pBoneCandidate->GetNode()->GetName());

            // get the parent id of the bone candidate
            int parentId;
            parentId = skeletonCandidate.GetParentSelectedId(boneCandidateId);

            // set the parentId
            pCoreBone->setParentId(parentId);

            // get the translation and the rotation of the bone candidate
            CalVector translation;
            CalQuaternion rotation;
            skeletonCandidate.GetTranslationAndRotation(boneCandidateId, -1.0f, translation, rotation);

            // set the translation and rotation
            pCoreBone->setTranslation(translation);
            pCoreBone->setRotation(rotation);

            // get the bone space translation and the rotation of the bone candidate
            CalVector translationBoneSpace;
            CalQuaternion rotationBoneSpace;
            skeletonCandidate.GetTranslationAndRotationBoneSpace(boneCandidateId, -1.0f, translationBoneSpace, rotationBoneSpace);

            // set the bone space translation and rotation
            pCoreBone->setTranslationBoneSpace(translationBoneSpace);
            pCoreBone->setRotationBoneSpace(rotationBoneSpace);

            CBaseNode * pBoneNode = pBoneCandidate->GetNode();
            pCoreBone->setLightType( pBoneNode->GetLightType() );
            CalVector color;
            pBoneNode->GetLightColor( color );
            pCoreBone->setLightColor( color );

            // add the core bone to the core skeleton instance
            int boneId;
            boneId = coreSkeleton.addCoreBone(pCoreBone);

            // adjust child list of parent bone
            if(parentId != -1)
            {
                // get parent core bone
                CalCoreBone *pParentCoreBone;
                pParentCoreBone = coreSkeleton.getCoreBone(parentId);
                if(pParentCoreBone == 0)
                {
                    SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
                    delete pCoreBone;
                    return false;
                }

                // add this core bone to the child list of the parent bone
                pParentCoreBone->addChildId(boneId);
            }
        }
    }

    // save core skeleton to the file
    if(!CalSaver::saveCoreSkeleton(strFilename, &coreSkeleton))
    {
        SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
        return false;
    }

    HKEY hk;
    LONG lret=RegCreateKey(HKEY_CURRENT_USER, "Software\\Cal3D\\Exporter", &hk);
    if(lret==ERROR_SUCCESS && NULL!=hk)
    {
        lret=RegSetValueEx(hk,"skeleton",NULL,REG_SZ,(unsigned char *)strFilename.c_str() ,strFilename.length());
        RegCloseKey(hk);
    }


    return true;
}

//----------------------------------------------------------------------------//
// Get the last error message                                                 //
//----------------------------------------------------------------------------//

CBaseInterface *CExporter::GetInterface()
{
    return m_pInterface;
}

//----------------------------------------------------------------------------//
// Get the last error message                                                 //
//----------------------------------------------------------------------------//

const std::string& CExporter::GetLastError()
{
    return m_strLastError;
}

//----------------------------------------------------------------------------//
// Set the last error message                                                 //
//----------------------------------------------------------------------------//

void CExporter::SetLastError(const std::string& strText, const std::string& strFilename, int line)
{
    std::stringstream strstrError;
    strstrError << strText << "\n(" << strFilename << " " << line << ")" << std::ends;

    m_strLastError = strstrError.str();
    std::string s(m_strLastError);
    ::OutputDebugString(s.c_str());
}

//----------------------------------------------------------------------------//
// Set the last error message from the cal3d library                          //
//----------------------------------------------------------------------------//

void CExporter::SetLastErrorFromCal(const std::string& strFilename, int line)
{
    std::stringstream strstrError;
    strstrError << "cal3d : " << CalError::getLastErrorDescription();

    if(!CalError::getLastErrorTextInternal().empty())
    {
        strstrError << " '" << CalError::getLastErrorText() << "'";
    }

    strstrError << " in " << CalError::getLastErrorFile()
    << "(" << CalError::getLastErrorLine() << ")";

    strstrError << "\n(" << strFilename << " " << line << ")" << std::ends;

    m_strLastError = strstrError.str();
    std::string s(m_strLastError);
    ::OutputDebugString(s.c_str());
}

//----------------------------------------------------------------------------//
