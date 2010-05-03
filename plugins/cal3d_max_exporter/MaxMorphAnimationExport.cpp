//----------------------------------------------------------------------------//
// MaxMorphAnimationExport.cpp                                                     //
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
#include "MaxMorphAnimationExport.h"
#include "Exporter.h"
#include "MaxInterface.h"

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMaxMorphAnimationExport::CMaxMorphAnimationExport()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxMorphAnimationExport::~CMaxMorphAnimationExport()
{
}

//----------------------------------------------------------------------------//
// Following methods have to be implemented to make it a valid plugin         //
//----------------------------------------------------------------------------//

const TCHAR *CMaxMorphAnimationExport::AuthorName()
{
	return _T("IMVU Inc; Bruno 'Beosil' Heidelberger");
}

const TCHAR *CMaxMorphAnimationExport::CopyrightMessage()
{
	return _T("Copyright (c) 2010 IMVU Inc; Copyright (C) 2001 Bruno 'Beosil' Heidelberger");
}

int CMaxMorphAnimationExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
  ::OutputDebugString("CMaxMorphAnimationExport::DoExport()\n");

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// create an export interface for 3d studio max
	CMaxInterface maxInterface;
	if(!maxInterface.Create(ei, i))
	{
    ::OutputDebugString("maxInterface.Create() failed\n");
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// create an exporter instance
	if(!theExporter.Create(&maxInterface))
	{
    ::OutputDebugString("theExporter.Create() failed\n");
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// export the morphAnimation
  ::OutputDebugString("Attempting export...\n");
	if(!theExporter.ExportMorphAnimation(name))
	{
    ::OutputDebugString("theExporter.ExportMorphAnimation() failed\n");
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

  ::OutputDebugString("Morph export success!\n");
	return 1;
}

const TCHAR *CMaxMorphAnimationExport::Ext(int i)
{
	switch(i)
	{
	case 0:
		return _T("xpf");
	default:
		return _T("");
	}
}

int CMaxMorphAnimationExport::ExtCount()
{
	return 1;
}

const TCHAR *CMaxMorphAnimationExport::LongDesc()
{
	return _T("Cal3D MorphAnimation File");
}

const TCHAR *CMaxMorphAnimationExport::OtherMessage1()
{
	return _T("");
}

const TCHAR *CMaxMorphAnimationExport::OtherMessage2()
{
	return _T("");
}

const TCHAR *CMaxMorphAnimationExport::ShortDesc()
{
	return _T("Cal3D MorphAnimation File");
}

void CMaxMorphAnimationExport::ShowAbout(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDialog dlg(IDD_ABOUT);
	dlg.DoModal();
}

unsigned int CMaxMorphAnimationExport::Version()
{
	return 50;
}

bool CMaxMorphAnimationExport::ExportMorphAnimationFromMaxscriptCall(const TCHAR *name, AnimExportParams* _animexportparams)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// create an export interface for 3d studio max
	CMaxInterface maxInterface;
	if(!maxInterface.Create(NULL, GetCOREInterface()))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// create an exporter instance
	if(!theExporter.Create(&maxInterface))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// export the morphAnimation
	if(! theExporter.ExportMorphAnimationFromMaxscriptCall(name, (void*)_animexportparams))
	{
		AfxMessageBox(theExporter.GetLastError().c_str(), MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	return 1;
}

