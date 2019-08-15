﻿// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "EditorActiveCameraStreamObject.h"
#include "MobuLiveLinkUtilities.h"

#include "CameraStreamObject.h"
#include "ModelStreamObject.h"

#include "Roles/LiveLinkCameraRole.h"
#include "Roles/LiveLinkCameraTypes.h"

FEditorActiveCameraStreamObject::FEditorActiveCameraStreamObject(const TSharedPtr<ILiveLinkProvider> StreamProvider)
	: Provider(StreamProvider)
	, SubjectName("EditorActiveCamera")
	, bIsActive(true)
	, bSendAnimatable(false)
{
	Refresh();
}

FEditorActiveCameraStreamObject::~FEditorActiveCameraStreamObject()
{
	Provider->RemoveSubject(SubjectName);
	FBTrace("Destroying Editor Camera\n");
}

const bool FEditorActiveCameraStreamObject::ShouldShowInUI() const
{
	return false;
}

const FString FEditorActiveCameraStreamObject::GetStreamOptions() const
{
	// Stream options are not valid on Editor camera
	return FString();
};

FName FEditorActiveCameraStreamObject::GetSubjectName() const
{
	return SubjectName;
};

void FEditorActiveCameraStreamObject::UpdateSubjectName(FName NewSubjectName)
{
	// Subject name is not changeable on the Editor camera
};


int FEditorActiveCameraStreamObject::GetStreamingMode() const
{
	return 0;
};

void FEditorActiveCameraStreamObject::UpdateStreamingMode(int NewStreamingMode)
{
	// Streaming mode is not changeable on the Editor camera
};


bool FEditorActiveCameraStreamObject::GetActiveStatus() const
{
	return bIsActive;
};

void FEditorActiveCameraStreamObject::UpdateActiveStatus(bool bIsNowActive)
{
	bIsActive = bIsNowActive;
};

bool FEditorActiveCameraStreamObject::GetSendAnimatableStatus() const
{
	return bSendAnimatable;
};

void FEditorActiveCameraStreamObject::UpdateSendAnimatableStatus(bool bNewSendAnimatable)
{
	if (bSendAnimatable != bNewSendAnimatable)
	{
		bSendAnimatable = bNewSendAnimatable;
		Refresh();
	}
};

const FBModel* FEditorActiveCameraStreamObject::GetModelPointer() const
{
	return nullptr;
};

const FString FEditorActiveCameraStreamObject::GetRootName() const
{
	// Root Name is not valid on Editor camera
	return FString();
}


bool FEditorActiveCameraStreamObject::IsValid() const
{
	// Editor camera is always valid
	return true;
}

void FEditorActiveCameraStreamObject::Refresh()
{
	FBSystem System;
	const FBCamera* CameraModel = System.Scene->Renderer->CurrentCamera;

	FLiveLinkStaticDataStruct CameraData(FLiveLinkCameraStaticData::StaticStruct());
	FModelStreamObject::UpdateSubjectTransformStaticData(CameraModel, bSendAnimatable, *CameraData.Cast<FLiveLinkCameraStaticData>());
	FCameraStreamObject::UpdateSubjectCameraStaticData(CameraModel, *CameraData.Cast<FLiveLinkCameraStaticData>());
	Provider->UpdateSubjectStaticData(SubjectName, ULiveLinkCameraRole::StaticClass(), MoveTemp(CameraData));
}

void FEditorActiveCameraStreamObject::UpdateSubjectFrame()
{
	if (!bIsActive)
	{
		return;
	}

	FBSystem System;
	const FBCamera* CameraModel = System.Scene->Renderer->CurrentCamera;

	if (CameraModel)
	{
		FLiveLinkFrameDataStruct CameraData(FLiveLinkCameraFrameData::StaticStruct());
		FModelStreamObject::UpdateSubjectTransformFrameData(CameraModel, bSendAnimatable, *CameraData.Cast<FLiveLinkTransformFrameData>());
		FCameraStreamObject::UpdateSubjectCameraFrameData(CameraModel, *CameraData.Cast<FLiveLinkCameraFrameData>());
		Provider->UpdateSubjectFrameData(SubjectName, MoveTemp(CameraData));
	}
}