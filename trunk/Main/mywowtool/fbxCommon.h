/****************************************************************************************

   Copyright (C) 2014 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/
#ifndef _FBXCOMMON_H
#define _FBXCOMMON_H

#include <fbxsdk.h>

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
void DestroySdkObjects(FbxManager* pManager, bool pExitStatus);
void CreateAndFillIOSettings(FbxManager* pManager);

bool SaveScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename, int pFileFormat=-1, bool pEmbedMedia=false);
bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);

#endif // #ifndef _COMMON_H


