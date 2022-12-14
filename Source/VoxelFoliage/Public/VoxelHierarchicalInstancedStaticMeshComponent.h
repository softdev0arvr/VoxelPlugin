// Copyright 2021 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "VoxelMinimal.h"
#include "VoxelEnums.h"
#include "VoxelIntBox.h"
#include "VoxelInstancedMeshSettings.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "VoxelHierarchicalInstancedStaticMeshComponent.generated.h"

struct FVoxelFoliageTransform;
struct FVoxelFoliageTransforms;
struct FVoxelHISMBuiltData;
class FVoxelPool;
class FVoxelRuntimeSettings;
class FVoxelConstDataAccelerator;
class FVoxelInstancedMeshManager;

DECLARE_UNIQUE_VOXEL_ID(FVoxelHISMBuildTaskId);
DECLARE_VOXEL_MEMORY_STAT(TEXT("Voxel HISM Memory"), STAT_VoxelHISMMemory, STATGROUP_VoxelMemory, VOXELFOLIAGE_API);

struct FVoxelInstancesSection
{
	int32 StartIndex = -1;
	int32 Num = -1;

	// Between 0 and Num - 1
	TArray<int32> RemovedIndices;
};

// Need to prefix names with Voxel to avoid collisions with normal HISM
UCLASS()
class VOXELFOLIAGE_API UVoxelHierarchicalInstancedStaticMeshComponent : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	// How long to wait for new instances before triggering a new cull tree/render update
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel")
	float Voxel_BuildDelay = 0.5f;

private:
	UPROPERTY()
	UMaterialInterface* Voxel_DebugMaterial;
		
public:
	UVoxelHierarchicalInstancedStaticMeshComponent(const FObjectInitializer& ObjectInitializer);

};

inline UVoxelHierarchicalInstancedStaticMeshComponent::UVoxelHierarchicalInstancedStaticMeshComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}
