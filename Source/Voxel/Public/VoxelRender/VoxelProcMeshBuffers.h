// Copyright 2020 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "VoxelGlobals.h"
#include "StaticMeshResources.h"
#include "VoxelRawStaticIndexBuffer.h"

class FVoxelProcMeshBuffersRenderData;

DECLARE_STATS_GROUP(TEXT("Voxel Proc Mesh Memory"), STATGROUP_VoxelProcMeshMemory, STATCAT_Advanced);
DECLARE_MEMORY_STAT_EXTERN(TEXT("Voxel Proc Mesh Memory"), STAT_VoxelProcMeshMemory, STATGROUP_VoxelMemory, VOXEL_API);
DECLARE_MEMORY_STAT_EXTERN(TEXT("Indices"), STAT_VoxelProcMeshMemory_Indices, STATGROUP_VoxelProcMeshMemory, VOXEL_API);
DECLARE_MEMORY_STAT_EXTERN(TEXT("Positions"), STAT_VoxelProcMeshMemory_Positions, STATGROUP_VoxelProcMeshMemory, VOXEL_API);
DECLARE_MEMORY_STAT_EXTERN(TEXT("Colors"), STAT_VoxelProcMeshMemory_Colors, STATGROUP_VoxelProcMeshMemory, VOXEL_API);
DECLARE_MEMORY_STAT_EXTERN(TEXT("Adjacency"), STAT_VoxelProcMeshMemory_Adjacency, STATGROUP_VoxelProcMeshMemory, VOXEL_API);
DECLARE_MEMORY_STAT_EXTERN(TEXT("UVs & Tangents"), STAT_VoxelProcMeshMemory_UVs_Tangents, STATGROUP_VoxelProcMeshMemory, VOXEL_API);

struct VOXEL_API FVoxelProcMeshBuffers
{
	// We'll be initializing/releasing a single buffer multiple times, so need to keep the data on the CPU!
	static constexpr bool bNeedsCPUAccess = true;

	// GUIDs of the meshes merged into these buffers, used to avoid rebuilding collisions & navmesh
	TArray<FGuid> Guids;
	
	/** Vertex buffer for this section */
	FStaticMeshVertexBuffers VertexBuffers;
	/** Index buffer for this section */
	FVoxelRawStaticIndexBuffer IndexBuffer{ bNeedsCPUAccess };
	/** Index buffer containing adjacency information required by tessellation. */
	FVoxelRawStaticIndexBuffer AdjacencyIndexBuffer{ bNeedsCPUAccess };
	/** Local bounds of this section */
	FBox LocalBounds = FBox(ForceInit);

	inline int32 GetNumVertices() const
	{
		return VertexBuffers.PositionVertexBuffer.GetNumVertices();
	}
	inline int32 GetNumIndices() const
	{
		return IndexBuffer.GetNumIndices();
	}
	
	FVoxelProcMeshBuffers() = default;
	~FVoxelProcMeshBuffers();

	uint32 GetAllocatedSize() const;
	void UpdateStats();

private:
	int32 LastAllocatedSize = 0;
	int32 LastAllocatedSize_Indices = 0;
	int32 LastAllocatedSize_Positions = 0;
	int32 LastAllocatedSize_Colors = 0;
	int32 LastAllocatedSize_Adjacency = 0;
	int32 LastAllocatedSize_UVs_Tangents = 0;
	mutable TVoxelWeakPtr<FVoxelProcMeshBuffersRenderData> RenderData;

	friend class FVoxelProcMeshBuffersRenderData;
};