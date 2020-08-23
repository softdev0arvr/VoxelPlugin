// Copyright 2020 Phyronnaz

#include "VoxelTools/VoxelAssetTools.h"
#include "VoxelTools/VoxelToolHelpers.h"
#include "VoxelData/VoxelData.h"
#include "VoxelData/VoxelData.inl"
#include "VoxelAssets/VoxelDataAsset.h"
#include "VoxelAssets/VoxelDataAssetData.inl"
#include "VoxelWorld.h"


void UVoxelAssetTools::ImportAssetAsReference(
	FVoxelAssetItemReference& Reference,
	AVoxelWorld* World,
	UVoxelTransformableWorldGenerator* Asset,
	const TMap<FName, int32>& Seeds,
	FTransform Transform,
	FVoxelIntBox Bounds,
	int32 Priority,
	bool bConvertToVoxelSpace,
	bool bUpdateRender)
{
	VOXEL_PRO_ONLY_VOID();
}

void UVoxelAssetTools::ImportAssetAsReferenceAsync(
	UObject* WorldContextObject,
    FLatentActionInfo LatentInfo,
	FVoxelAssetItemReference& Reference,
	AVoxelWorld* World,
	UVoxelTransformableWorldGenerator* Asset,
	const TMap<FName, int32>& Seeds,
	FTransform Transform,
	FVoxelIntBox Bounds,
	int32 Priority,
	bool bConvertToVoxelSpace,
	bool bUpdateRender,
	bool bHideLatentWarnings)
{
	VOXEL_PRO_ONLY_VOID();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


void UVoxelAssetTools::ImportModifierAsset(
	AVoxelWorld* World,
	UVoxelTransformableWorldGenerator* Asset,
	const TMap<FName, int32>& Seeds,
	FTransform Transform,
	FVoxelIntBox Bounds,
	bool bModifyValues,
	bool bModifyMaterials,
	bool bLockEntireWorld,
	bool bConvertToVoxelSpace)
{
	VOXEL_PRO_ONLY_VOID();
}

void UVoxelAssetTools::ImportModifierAssetAsync(
	UObject* WorldContextObject,
	FLatentActionInfo LatentInfo,
	AVoxelWorld* World,
	UVoxelTransformableWorldGenerator* Asset,
	const TMap<FName, int32>& Seeds,
	FTransform Transform,
	FVoxelIntBox Bounds,
	bool bModifyValues,
	bool bModifyMaterials,
	bool bLockEntireWorld,
	bool bConvertToVoxelSpace,
	bool bHideLatentWarnings)
{
	VOXEL_PRO_ONLY_VOID();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


void UVoxelAssetTools::ImportAsset(
	AVoxelWorld* World,
	UVoxelTransformableWorldGenerator* Asset,
	const TMap<FName, int32>& Seeds,
	FTransform Transform,
	FVoxelIntBox Bounds,
	bool bSubtractive,
	EVoxelAssetMergeMode MergeMode,
	bool bConvertToVoxelSpace)
{
	VOXEL_PRO_ONLY_VOID();
}

void UVoxelAssetTools::ImportAssetAsync(
	UObject* WorldContextObject,
	FLatentActionInfo LatentInfo,
	AVoxelWorld* World,
	UVoxelTransformableWorldGenerator* Asset,
	const TMap<FName, int32>& Seeds,
	FTransform Transform,
	FVoxelIntBox Bounds,
	bool bSubtractive,
	EVoxelAssetMergeMode MergeMode,
	bool bConvertToVoxelSpace,
	bool bHideLatentWarnings)
{
	VOXEL_PRO_ONLY_VOID();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void UVoxelAssetTools::ImportDataAssetFast(
	AVoxelWorld* World, 
	UVoxelDataAsset* Asset, 
	FVector InPosition, 
	EVoxelAssetMergeMode MergeMode, 
	bool bConvertToVoxelSpace)
{
	VOXEL_PRO_ONLY_VOID();
}

void UVoxelAssetTools::ImportDataAssetFastAsync(
	UObject* WorldContextObject,
	FLatentActionInfo LatentInfo,
	AVoxelWorld* World, 
	UVoxelDataAsset* Asset, 
	FVector InPosition, 
	EVoxelAssetMergeMode MergeMode, 
	bool bConvertToVoxelSpace,
	bool bHideLatentWarnings)
{
	VOXEL_PRO_ONLY_VOID();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


void UVoxelAssetTools::InvertDataAsset(UVoxelDataAsset* Asset, UVoxelDataAsset*& InvertedAsset)
{
	VOXEL_PRO_ONLY_VOID();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


void UVoxelAssetTools::SetDataAssetMaterial(UVoxelDataAsset* Asset, UVoxelDataAsset*& NewAsset, FVoxelMaterial Material)
{
	VOXEL_PRO_ONLY_VOID();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


UVoxelDataAsset* UVoxelAssetTools::CreateDataAssetFromWorldSection(
	AVoxelWorld* World,
	FVoxelIntBox Bounds,
	bool bCopyMaterials)
{
	VOXEL_PRO_ONLY();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void UVoxelAssetTools::AddDisableEditsBox(
	FVoxelDisableEditsBoxItemReference& Reference,
	AVoxelWorld* World, 
	FVoxelIntBox Bounds)
{
	VOXEL_PRO_ONLY_VOID();
}

void UVoxelAssetTools::AddDisableEditsBoxAsync(
	UObject* WorldContextObject, 
	FLatentActionInfo LatentInfo, 
	FVoxelDisableEditsBoxItemReference& Reference,
	AVoxelWorld* World,
	FVoxelIntBox Bounds,
	bool bHideLatentWarnings)
{
	VOXEL_PRO_ONLY_VOID();
}