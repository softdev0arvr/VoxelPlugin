// Copyright 2019 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "IntBox.h"
#include "VoxelConfigEnums.h"
#include "PhysicsEngine/BodyInstance.h"

class IVoxelPool;
class FVoxelData;
class FVoxelDebugManager;
class UMaterialInterface;
class UVoxelMaterialCollection;
struct FVoxelGrassSpawner;
struct FVoxelBlendedMaterial;

DECLARE_MULTICAST_DELEGATE(FVoxelRendererOnWorldLoaded);
DECLARE_DELEGATE_OneParam(FVoxelOnUpdateFinished, FIntBox);

struct FVoxelRenderChunkSettings
{
	bool bVisible : 1;
	bool bEnableCollisions : 1;
	bool bEnableNavmesh : 1;
	bool bEnableGrass : 1;
	bool bEnableTessellation : 1;

	inline bool IsRendered() const { return bVisible || bEnableCollisions || bEnableNavmesh; }

	inline bool operator!=(const FVoxelRenderChunkSettings& Other) const
	{
		return bVisible != Other.bVisible ||
			bEnableCollisions != Other.bEnableCollisions ||
			bEnableNavmesh != Other.bEnableNavmesh ||
			bEnableGrass != Other.bEnableGrass ||
			bEnableTessellation != Other.bEnableTessellation;
	}
	inline bool operator==(const FVoxelRenderChunkSettings& Other) const
	{
		return bVisible == Other.bVisible &&
			bEnableCollisions == Other.bEnableCollisions &&
			bEnableNavmesh == Other.bEnableNavmesh &&
			bEnableGrass == Other.bEnableGrass &&
			bEnableTessellation == Other.bEnableTessellation;
	}

	inline static FVoxelRenderChunkSettings Visible()
	{
		return { true, false, false, false, false };
	}
};

struct FVoxelRendererSettings
{
	float VoxelSize;
	int OctreeDepth;
	FIntBox WorldBounds;
	UClass* ProcMeshClass;

	TSharedRef<TArray<FIntVector>> InvokersPositions = MakeShared<TArray<FIntVector>>();
	
	TSharedPtr<FIntVector> WorldOffset;
	
	TWeakObjectPtr<UWorld> World;
	TWeakObjectPtr<AActor> ComponentsOwner;
	TSharedPtr<FVoxelData, ESPMode::ThreadSafe> Data;
	TSharedPtr<IVoxelPool> Pool;
	TSharedPtr<FVoxelDebugManager, ESPMode::ThreadSafe> DebugManager;

	EVoxelUVConfig UVConfig;
	EVoxelNormalConfig NormalConfig;
	EVoxelMaterialConfig MaterialConfig;
	TWeakObjectPtr<UMaterialInterface> VoxelMaterialWithoutTessellation;
	TWeakObjectPtr<UMaterialInterface> VoxelMaterialWithTessellation;
	TWeakObjectPtr<UVoxelMaterialCollection> MaterialCollection;

	float TessellationBoundsExtension;
	float WaitForOtherChunksToAvoidHolesTimeout;

	FBodyInstance CollisionPresets;

	EVoxelRenderType RenderType;
	float ChunksDitheringDuration;
	bool bOptimizeIndices;


public:
	FVector GetChunkRelativePosition(const FIntVector& Position) const
	{
		return FVector(Position + *WorldOffset) * VoxelSize;
	}

	inline UMaterialInterface* GetVoxelMaterial(bool bEnableTessellation, int InLOD, const FVoxelBlendedMaterial& Index) const
	{
		return bEnableTessellation ? GetVoxelMaterialWithTessellation(Index) : GetVoxelMaterialWithoutTessellation(Index);
	}
	inline UMaterialInterface* GetVoxelMaterial(bool bEnableTessellation, int InLOD) const
	{
		return bEnableTessellation ? GetVoxelMaterialWithTessellation() : GetVoxelMaterialWithoutTessellation();
	}

private:
	UMaterialInterface* GetVoxelMaterialWithoutTessellation(const FVoxelBlendedMaterial& Index) const;
	UMaterialInterface* GetVoxelMaterialWithTessellation(const FVoxelBlendedMaterial& Index) const;
	UMaterialInterface* GetVoxelMaterialWithoutTessellation() const;
	UMaterialInterface* GetVoxelMaterialWithTessellation() const;
};

struct FVoxelChunkToAdd
{
	uint64 Id;
	FIntBox Bounds;
	int LOD;
	FVoxelRenderChunkSettings Settings;
	TArray<uint64> PreviousChunks; // Can be chunks to remove or visible chunks that become hidden
};

struct FVoxelChunkToUpdate
{
	uint64 Id;
	FIntBox Bounds;
	FVoxelRenderChunkSettings OldSettings;
	FVoxelRenderChunkSettings NewSettings;
	TArray<uint64> PreviousChunks; // If bVisible is now true
};

struct FVoxelChunkToRemove
{
	uint64 Id;
};

struct FVoxelTransitionsToUpdate
{
	uint64 Id;
	uint8 TransitionsMask;
};

class VOXEL_API IVoxelRenderer
{
public:
	const FVoxelRendererSettings Settings;
	FVoxelRendererOnWorldLoaded OnWorldLoaded;

	IVoxelRenderer(const FVoxelRendererSettings& Settings)
		: Settings(Settings)
	{

	}
	virtual ~IVoxelRenderer() = default;

	virtual void UpdateChunks(const TArray<uint64>& ChunksToUpdate, bool bWaitForAllChunksToFinishUpdating, const FVoxelOnUpdateFinished& FinishDelegate) = 0;
	virtual void CancelDithering(const FIntBox& Bounds, const TArray<uint64>& Chunks) = 0;

	virtual int GetTaskCount() const = 0;

	virtual void RecomputeMeshPositions() = 0;
	
	virtual void UpdateLODs(const TArray<FVoxelChunkToAdd>& ChunksToAdd, const TArray<FVoxelChunkToUpdate>& ChunksToUpdate, const TArray<FVoxelChunkToRemove>& ChunksToRemove, const TArray<FVoxelTransitionsToUpdate>& TransitionsToUpdate) = 0;
};