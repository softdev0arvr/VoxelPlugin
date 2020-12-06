// Copyright 2020 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "VoxelIntBox.h"
#include "VoxelMinimal.h"

struct FVoxelChunkMesh;
class FVoxelData;
class IVoxelRenderer;
class FVoxelDataLockInfo;
class FVoxelRuntimeSettings;
class FVoxelRuntimeDynamicSettings;

#if ENABLE_MESHER_STATS
struct FVoxelScopedMesherTime
{
	const uint64 StartCycle = FPlatformTime::Cycles64();
	uint64& Cycles;

	FORCEINLINE explicit FVoxelScopedMesherTime(uint64& Cycles)
		: Cycles(Cycles)
	{
	}
	FORCEINLINE ~FVoxelScopedMesherTime()
	{
		Cycles += FPlatformTime::Cycles64() - StartCycle;
	}
};

#define MESHER_TIME_SCOPE(Time) FVoxelScopedMesherTime LocalScope(Times.Time);
#define MESHER_TIME_INLINE(Time, X) [&]() { FVoxelScopedMesherTime LocalScope(Times.Time); return X; }()

#define MESHER_TIME_SCOPE_VALUES(Count) FVoxelScopedMesherTime LocalScope(Times._Values); Times._ValuesAccesses += Count;
#define MESHER_TIME_INLINE_VALUES(Count, X) [&]() { FVoxelScopedMesherTime LocalScope(Times._Values); Times._ValuesAccesses += Count; return X; }()

#define MESHER_TIME_SCOPE_MATERIALS(Count) FVoxelScopedMesherTime LocalScope(Times._Materials); Times._MaterialsAccesses += Count;
#define MESHER_TIME_INLINE_MATERIALS(Count, X) [&]() { FVoxelScopedMesherTime LocalScope(Times._Materials); Times._MaterialsAccesses += Count; return X; }()
#else
#define MESHER_TIME_SCOPE(Time)
#define MESHER_TIME_INLINE(Time, X) X

#define MESHER_TIME_SCOPE_VALUES(Count)
#define MESHER_TIME_INLINE_VALUES(Count, X) X

#define MESHER_TIME_SCOPE_MATERIALS(Count)
#define MESHER_TIME_INLINE_MATERIALS(Count, X) X
#endif

// All times are in cycles
struct FVoxelMesherTimes
{
	uint64 _Values = 0;
	uint64 _Materials = 0;
	uint64 _ValuesAccesses = 0;
	uint64 _MaterialsAccesses = 0;

	uint64 Normals = 0;
	uint64 UVs = 0;
	uint64 CreateChunk = 0;

	uint64 FindFaces = 0;
	uint64 AddFaces = 0;
	uint64 GreedyMeshing = 0;
	uint64 CollisionCubes = 0;
	
	uint64 FinishCreatingChunk = 0;
	uint64 DistanceField = 0;
};

class FVoxelMesherBase
{
public:
	const int32 LOD;
	const int32 Step;
	const int32 Size;
	const FIntVector ChunkPosition;
	const FVoxelRuntimeSettings& Settings;
	const FVoxelRuntimeDynamicSettings& DynamicSettings;
	const FVoxelData& Data;
	const IVoxelRenderer& Renderer;
	const bool bIsTransitions;

	FVoxelMesherBase(
		int32 LOD,
		const FIntVector& ChunkPosition,
		const IVoxelRenderer& Renderer,
		const FVoxelData& Data,
		bool bIsTransitions);
	virtual ~FVoxelMesherBase();

	virtual TVoxelSharedPtr<FVoxelChunkMesh> CreateFullChunk() = 0;
	virtual void CreateGeometry(TArray<uint32>& Indices, TArray<FVector>& Vertices) = 0;
	
	TVoxelSharedPtr<FVoxelChunkMesh> CreateEmptyChunk() const;
	void FinishCreatingChunk(FVoxelChunkMesh& Chunk) const;

protected:
	virtual FVoxelIntBox GetBoundsToCheckIsEmptyOn() const = 0;
	virtual FVoxelIntBox GetBoundsToLock() const = 0;

	void UnlockData();
	
private:
	TUniquePtr<FVoxelDataLockInfo> LockInfo;

	void LockData();
	bool IsEmpty() const;

	friend class FVoxelMesher;
	friend class FVoxelTransitionsMesher;
};

class FVoxelMesher : public FVoxelMesherBase
{
public:
	FVoxelMesher(
		int32 LOD,
		const FIntVector& ChunkPosition,
		const IVoxelRenderer& Renderer,
		const FVoxelData& Data);

	virtual TVoxelSharedPtr<FVoxelChunkMesh> CreateFullChunk() override final;
	virtual void CreateGeometry(TArray<uint32>& Indices, TArray<FVector>& Vertices) override final;

protected:
	// Need to call UnlockData
	virtual TVoxelSharedPtr<FVoxelChunkMesh> CreateFullChunkImpl(FVoxelMesherTimes& Times) = 0;
	// Need to call UnlockData
	virtual void CreateGeometryImpl(FVoxelMesherTimes& Times, TArray<uint32>& Indices, TArray<FVector>& Vertices) = 0;
};

class FVoxelTransitionsMesher : public FVoxelMesherBase
{
public:
	const uint8 TransitionsMask;
	const int32 HalfLOD;
	const int32 HalfStep;

	FVoxelTransitionsMesher(
		int32 LOD,
		const FIntVector& ChunkPosition,
		const IVoxelRenderer& Renderer,
		const FVoxelData& Data,
		uint8 TransitionsMask);

	virtual TVoxelSharedPtr<FVoxelChunkMesh> CreateFullChunk() override final;
	virtual void CreateGeometry(TArray<uint32>& Indices, TArray<FVector>& Vertices) override final;
	
protected:
	// Need to call UnlockData
	virtual TVoxelSharedPtr<FVoxelChunkMesh> CreateFullChunkImpl(FVoxelMesherTimes& Times) = 0;
};