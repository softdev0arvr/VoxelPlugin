// Copyright 2019 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "VoxelMaterial.h"
#include "VoxelValue.h"
#include "IntBox.h"
#include "VoxelDiff.h"
#include "VoxelUtilities.h"
#include "VoxelData/VoxelSave.h"
#include "VoxelData/VoxelDataOctree.h"
#include "VoxelMath.h"

struct FVoxelChunk;
class FVoxelData;
class FVoxelWorldGeneratorInstance;
class FVoxelPlaceableItem;

DECLARE_MEMORY_STAT(TEXT("Voxel 2D Cache Memory"), STAT_Voxel2DCacheMemory, STATGROUP_VoxelMemory);
DECLARE_MEMORY_STAT(TEXT("Voxel GeneratedChunksSet Memory"), STAT_VoxelGeneratedChunksSetMemory, STATGROUP_VoxelMemory);
DECLARE_CYCLE_STAT(TEXT("Lock"), STAT_TryLock, STATGROUP_Voxel);
DECLARE_CYCLE_STAT(TEXT("TryLock"), STAT_Lock, STATGROUP_Voxel);
DECLARE_CYCLE_STAT(TEXT("Unlock"), STAT_Unlock, STATGROUP_Voxel);
DECLARE_CYCLE_STAT(TEXT("FVoxelData::~FVoxelData"), STAT_VoxelData_Destructor, STATGROUP_Voxel);

DECLARE_MULTICAST_DELEGATE_TwoParams(FVoxelPreGenerationDelegate, FVoxelData&, const FIntBox&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FVoxelPostGenerationDelegate, FVoxelData&, const FIntBox&, const FVoxelChunk&);

/**
 * Class that handle voxel data
 */
class VOXEL_API FVoxelData : public TSharedFromThis<FVoxelData, ESPMode::ThreadSafe>
{
public:
	/**
	 * Constructor
	 * @param	Depth				Depth of this world; Size = VOXEL_CELL_SIZE * 2^Depth
	 * @param	WorldGenerator		Generator for this world
	 * @param	bEnableMultiplayer	Is this for a multiplayer world
	 * @param	bEnableUndoRedo	    Enable Undo/Redo?
	 */
	FVoxelData(int Depth, const FIntBox& WorldBounds, TSharedRef<const FVoxelWorldGeneratorInstance, ESPMode::ThreadSafe> WorldGenerator, bool bEnableMultiplayer, bool bEnableUndoRedo, bool bCacheLOD0Chunks)
		: Depth(Depth)
		, WorldBounds(WorldBounds)
		, bEnableMultiplayer(bEnableMultiplayer)
		, bEnableUndoRedo(bEnableUndoRedo)
		, bCacheLOD0Chunks(bCacheLOD0Chunks)
		, WorldGenerator(WorldGenerator)
		, Octree(MakeUnique<FVoxelDataOctree>(this, &WorldGenerator.Get(), Depth, bEnableMultiplayer, bEnableUndoRedo))
	{
		check(Depth > 0);
		check(Octree->GetBounds().Contains(WorldBounds));

		INC_MEMORY_STAT_BY(STAT_VoxelGeneratedChunksSetMemory, GeneratedChunks.GetAllocatedSize());
	}
	FVoxelData(int Depth, TSharedRef<const FVoxelWorldGeneratorInstance, ESPMode::ThreadSafe> WorldGenerator, bool bEnableMultiplayer, bool bEnableUndoRedo, bool bCacheLOD0Chunks)
		: FVoxelData(Depth, FVoxelUtilities::GetBoundsFromDepth<VOXEL_CELL_SIZE>(Depth), WorldGenerator, bEnableMultiplayer, bEnableUndoRedo, bCacheLOD0Chunks)
	{

	}
	~FVoxelData()
	{
		SCOPE_CYCLE_COUNTER(STAT_VoxelData_Destructor);
		DEC_MEMORY_STAT_BY(STAT_VoxelGeneratedChunksSetMemory, GeneratedChunks.GetAllocatedSize());
	}

	FVoxelData(const FVoxelData&) = delete;
	FVoxelData(const FVoxelData&&) = delete;
    FVoxelData& operator=(const FVoxelData&) = delete;

	const int Depth;
	const FIntBox WorldBounds;
	const bool bEnableMultiplayer;
	const bool bEnableUndoRedo;
	const bool bCacheLOD0Chunks;

	TSharedRef<const FVoxelWorldGeneratorInstance, ESPMode::ThreadSafe> const WorldGenerator;

private:
	TUniquePtr<FVoxelDataOctree> const Octree;
	FVoxelSharedMutex MainLock; // For octree compact, where the entire octree needs to be locked

public:
	inline int32 Size() const { return VOXEL_CELL_SIZE << Depth; } // Size = VOXEL_CELL_SIZE * 2^Depth
	inline FVoxelDataOctree* GetOctree() const { return Octree.Get(); }
	inline bool IsInWorld(int X, int Y, int Z) const { return WorldBounds.IsInside(X, Y, Z); }
	inline int IsInWorld(const FIntVector& P) const { return IsInWorld(P.X, P.Y, P.Z); }

public:
	/**
	 * Lock the bounds
	 * @param	Bounds		Bounds to lock
	 * @param	OutOctrees	Locked octrees
	 * @param	Name		The name of the task locking these bounds (for debug)
	 */
	template<EVoxelLockType LockType>
	void Lock(const FIntBox& Bounds, FVoxelLockedOctrees& OutOctrees, FString Name)
	{
		ensure(TryLock<LockType>(Bounds, 1e9, OutOctrees, Name));
	}
	/**
	 * Try locking the bounds
	 * @param	Bounds				Bounds to lock
	 * @param	TimeoutInSeconds	Timeout. Might not be respected
	 * @param	OutOctrees			Locked octrees
	 * @param	InOutName			The name of the task locking these bounds (for debug).
	                                If not successful will contain the name of the task currently locking these bounds
	 */
	template<EVoxelLockType LockType>
	bool TryLock(const FIntBox& Bounds, float TimeoutInSeconds, FVoxelLockedOctrees& OutOctrees, FString& InOutName);
	
	/**
	 * Unlock previously locked octrees
	 * @param	LockedOctrees	Locked octrees
	 */
	template<EVoxelLockType LockType>
	void Unlock(FVoxelLockedOctrees& LockedOctrees);
	 	
public:	
	// Get the values and materials in this zone. Values and Materials can be null. Requires read lock
	void GetValuesAndMaterials(FVoxelValue Values[], FVoxelMaterial Materials[], const FVoxelWorldGeneratorQueryZone& QueryZone, int QueryLOD) const;

	// Is this region of the world empty at this LOD? Requires read lock
	inline bool IsEmpty(const FIntBox& Bounds, int QueryLOD) const { return Octree->IsEmpty(Bounds, QueryLOD) != EVoxelEmptyState::Unknown; }
	
	/**
	 * Set values or materials in a zone. T is either FVoxelValue or FVoxelMaterial. Requires write lock
	 * @param	Bounds	Bounds to modify
	 * @param	Lambda	Lambda called on every position inside Bounds. Signature: void Lambda(int X, int Y, int Z, T& Ref); You need to modify Ref
	 */
	 template<typename T, typename F>
	inline void SetValueOrMaterialLambda(const FIntBox& Bounds, F Lambda) { Octree->SetValueOrMaterialLambda<T>(Bounds.Overlap(WorldBounds), Lambda); }
	
	/**
	 * Call lambda on every value & material in bounds. Requires read lock
	 * @param	Bounds	Bounds to call lambda in
	 * @param	Lambda	Lambda called on every position inside Bounds. Signature: void Lambda(int X, int Y, int Z, const FVoxelValue* Value, const FVoxelMaterial* Material); 
						If bOnlyIfDirty is false, the world generator will be queried if the values/materials in bounds aren't loaded.
						Value and Material are always valid when bOnlyIfDirty is false, but not when bOnlyIfDirty is true
	 */
	 template<bool bOnlyIfDirty, typename F>
	inline void CallLambdaOnValuesInBounds(const FIntBox& Bounds, F Lambda) const { Octree->CallLambdaOnValuesInBounds<bOnlyIfDirty>(Bounds, Lambda); }

public:
	/**
	 * Getters/Setters
	 */
	// Get value and material at position
	inline void GetValueAndMaterial(int X, int Y, int Z, FVoxelValue* OutValue, FVoxelMaterial* OutMaterial, int QueryLOD) const
	{
		if (UNLIKELY(!IsInWorld(X, Y, Z)))
		{
			WorldGenerator->GetValueAndMaterial(X, Y, Z, OutValue, OutMaterial, QueryLOD, FVoxelPlaceableItemHolder());
		}
		else
		{
			Octree->GetLeaf(X, Y, Z)->GetValueAndMaterial(X, Y, Z, OutValue, OutMaterial, QueryLOD);
		}
	}
	// Set value or material at position depending on template argument (FVoxelValue or FVoxelMaterial)
	template<typename T>
	inline void SetValueOrMaterial(int X, int Y, int Z, const T& Value)
	{
		if (LIKELY(IsInWorld(X, Y, Z)))
		{
			Octree->GetLeaf(X, Y, Z)->SetValueOrMaterial<T>(X, Y, Z, Value);
		}
	}

	// Get the value & material at position. Requires read lock
	inline void GetValueAndMaterial(int X, int Y, int Z, FVoxelValue& OutValue, FVoxelMaterial& OutMaterial, int QueryLOD) const { GetValueAndMaterial(X, Y, Z, &OutValue, &OutMaterial, QueryLOD); }
	inline void GetValueAndMaterial(const FIntVector& P, FVoxelValue& OutValue, FVoxelMaterial& OutMaterial, int QueryLOD) const { GetValueAndMaterial(P.X, P.Y, P.Z, &OutValue, &OutMaterial, QueryLOD); }
	// Get the value at position. Requires read lock
	inline FVoxelValue GetValue(int X, int Y, int Z, int QueryLOD) const { FVoxelValue Value; GetValueAndMaterial(X, Y, Z, &Value, nullptr, QueryLOD); return Value; }
	inline FVoxelValue GetValue(const FIntVector& P, int QueryLOD) const { return GetValue(P.X, P.Y, P.Z, QueryLOD); }
	// Get the material at position. Requires read lock
	inline FVoxelMaterial GetMaterial(int X, int Y, int Z, int QueryLOD) const { FVoxelMaterial Material; GetValueAndMaterial(X, Y, Z, nullptr, &Material, QueryLOD); return Material; }
	inline FVoxelMaterial GetMaterial(const FIntVector& P, int QueryLOD) const { return GetMaterial(P.X, P.Y, P.Z, QueryLOD); }
	
	// Set value at position. Requires write lock
	inline void SetValue(int X, int Y, int Z, FVoxelValue Value) { SetValueOrMaterial<FVoxelValue>(X, Y, Z, Value); }
	inline void SetValue(const FIntVector& P, FVoxelValue Value) { SetValueOrMaterial<FVoxelValue>(P.X, P.Y, P.Z, Value); }
	// Set material at position. Requires write lock
	inline void SetMaterial(int X, int Y, int Z, FVoxelMaterial Material) { SetValueOrMaterial<FVoxelMaterial>(X, Y, Z, Material); }
	inline void SetMaterial(const FIntVector& P, FVoxelMaterial Material) { SetValueOrMaterial<FVoxelMaterial>(P.X, P.Y, P.Z, Material); }
	
public:
	/**
	 * Cache
	 */
	// Cache the most accessed chunks since last call to this function. No lock required
	void CacheMostUsedChunks(
		uint32 Threshold,
		uint32 MaxCacheSize, 
		uint32& NumChunksSubdivided, 
		uint32& NumChunksCached, 
		uint32& NumRemovedFromCache,
		uint32& TotalNumCachedChunks);
	// Compact the octree by removing unused empty nodes. No lock required
	void Compact(uint32& NumDeleted);

	// Are all the chunks intersecting this bounds cached LOD 0 chunks? Requires read lock
	inline bool AreBoundsCached(const FIntBox& Bounds) const { return Octree->AreBoundsCached(Bounds); }
	/**
	 * Cache the bounds. Requires write lock
	 * @param	Bounds				Bounds
	 * @param	bIsManualCache		Is this a manual cache? If not it can be removed by CacheMostUsedChunks 
	 * @param	bCacheValues		Cache the values?
	 * @param	bCacheMaterials		Cache the materials?
	 */
	 inline void CacheBounds(const FIntBox& Bounds, bool bIsManualCache, bool bCacheValues, bool bCacheMaterials) { Octree->CacheBounds(Bounds, bIsManualCache, bCacheValues, bCacheMaterials); }

private:
	uint32 CacheTime = 0;
	FCriticalSection CacheTimeSection;

public:
	/**
	 * Load/Save
	 */

	// Get a save of this world. No lock required
	void GetSave(FVoxelUncompressedWorldSave& OutSave);

	/**
	 * Load this world from save. No lock required
	 * @param	Save						Save to load from
	 * @param	OutBoundsToUpdate			The modified bounds
	 */
	void LoadFromSave(const FVoxelUncompressedWorldSave& Save, TArray<FIntBox>& OutBoundsToUpdate);


public:
	/**
	 * Undo/Redo
	 */

	// Undo one frame and add it to the redo stack. Current frame must be empty. No lock required
	void Undo(TArray<FIntBox>& OutBoundsToUpdate);
	// Redo one frame and add it to the undo stack. Current frame must be empty. No lock required
	void Redo(TArray<FIntBox>& OutBoundsToUpdate);
	// Clear all the frames. No lock required
	void ClearFrames();
	// Add the current frame to the undo stack. Clear the redo stack. No lock required
	void SaveFrame();
	// Check that the current frame is empty (safe to call Undo/Redo). No lock required
	bool IsCurrentFrameEmpty();
	// Get the history position. No lock required
	inline int GetHistoryPosition() const { return HistoryPosition; }
	// Get the max history position, ie HistoryPosition + redo frames. No lock required
	inline int GetMaxHistoryPosition() const { return MaxHistoryPosition; }

	// Mark the world as dirty
	inline void MarkAsDirty() { bIsDirty = true; }
	// Mark the world as not dirty
	inline void ClearDirtyFlag() { bIsDirty = false; }
	// Is the world dirty?
	inline bool IsDirty() { return bIsDirty; }

private:
	int HistoryPosition = 0;
	int MaxHistoryPosition = 0;
	bool bIsDirty = false;

public:
	// Add a FVoxelPlaceableItem to the world. Args will be forwarded to its constructor. Requires write lock
	template<typename T, typename... TArgs>
	uint64 AddItem(TArgs&&... Args)
	{
		TUniquePtr<T> Item = MakeUnique<T>(Forward<TArgs>(Args)...);
		uint64 ItemId = Item->UniqueId;

		Octree->AddItem<T>(Item.Get());
		Items.Add(ItemId, MoveTemp(Item));

		return ItemId;
	}

private:
	TMap<uint64, TUniquePtr<FVoxelPlaceableItem>> Items;

public:
	/**
	 * 2D Cache
	 */

	// Get the cached buffer at the 2D point Position
	inline float* GetCache2DValues(const FIntPoint& Position)
	{
		Cache2DLock.lock_shared();
		TUniquePtr<FCache2DValues>* Values = Cache2DValues.Find(Position);
		Cache2DLock.unlock_shared();
		if (Values)
		{
			return Values->Get()->Values;
		}
		else
		{
			TUniquePtr<FCache2DValues> NewValues = MakeUnique<FCache2DValues>();
			WorldGenerator->Cache2D(NewValues->Values, FVoxelWorldGeneratorQueryZone2D(Position, Position + FIntPoint(VOXEL_CELL_SIZE, VOXEL_CELL_SIZE)));
			float* ReturnValues = NewValues->Values;

			Cache2DLock.lock();
			if (TUniquePtr<FCache2DValues>* ExistingValues = Cache2DValues.Find(Position))
			{
				UE_LOG(LogVoxel, Warning, TEXT("Position (%d, %d) was cached twice, dropping one"), Position.X, Position.Y);
				ReturnValues = ExistingValues->Get()->Values;
			}
			else
			{
				Cache2DValues.Add(Position, MoveTemp(NewValues));
				Cache2DValues.Compact();
			}
			Cache2DLock.unlock();

			return ReturnValues;
		}
	}

private:
	std::shared_timed_mutex Cache2DLock;
	struct FCache2DValues
	{
		float Values[VOXEL_CELL_SIZE * VOXEL_CELL_SIZE];

		FCache2DValues()
		{
			INC_MEMORY_STAT_BY(STAT_Voxel2DCacheMemory, sizeof(FCache2DValues));
		}
		~FCache2DValues()
		{
			DEC_MEMORY_STAT_BY(STAT_Voxel2DCacheMemory, sizeof(FCache2DValues));
		}
	};
	TMap<FIntPoint, TUniquePtr<FCache2DValues>> Cache2DValues;

public:
	inline bool NeedToGenerateChunk(const FIntVector& Position)
	{
		FScopeLock Lock(&GeneratedChunksSection);
		
		const FIntVector Index = Position / CHUNK_SIZE;
		if (GeneratedChunks.Contains(Index))
		{
			return false;
		}
		else
		{
			DEC_MEMORY_STAT_BY(STAT_VoxelGeneratedChunksSetMemory, GeneratedChunks.GetAllocatedSize());
			GeneratedChunks.Add(Index);
			GeneratedChunks.Compact();
			INC_MEMORY_STAT_BY(STAT_VoxelGeneratedChunksSetMemory, GeneratedChunks.GetAllocatedSize());

			return true;
		}
	}
	
	// Always use those before using any of the delegates below
	
	inline void LockDelegatesForRead() { DelegatesLock.lock_shared(); }
	inline void UnlockDelegatesForRead() { DelegatesLock.unlock_shared(); }

	inline void LockDelegatesForWrite() { DelegatesLock.lock(); }
	inline void UnlockDelegatesForWrite() { DelegatesLock.unlock(); }

	// Data will be locked in setter mode
	FVoxelPreGenerationDelegate PreGenerationDelegate;
	// Data won't be locked
	FVoxelPostGenerationDelegate PostGenerationDelegate;

private:
	TSet<FIntVector> GeneratedChunks;
	FCriticalSection GeneratedChunksSection;
	std::shared_timed_mutex DelegatesLock;
};

template<EVoxelLockType LockType>
class FVoxelScopeLock
{
public:
	inline FVoxelScopeLock(FVoxelData* Data, const FIntBox& Bounds, const FString& Name)
		: Data(Data)
	{
		SCOPE_CYCLE_COUNTER(STAT_Lock);
		Data->Lock<LockType>(Bounds, LockedOctrees, Name);
	}
	inline FVoxelScopeLock(FVoxelData& Data, const FIntBox& Bounds, const FString& Name)
		: FVoxelScopeLock(&Data, Bounds, Name)
	{
	}
	inline FVoxelScopeLock(const TSharedPtr<FVoxelData, ESPMode::ThreadSafe>& Data, const FIntBox& Bounds, const FString& Name)
		: FVoxelScopeLock(Data.Get(), Bounds, Name)
	{
	}
	inline FVoxelScopeLock(const TSharedRef<FVoxelData, ESPMode::ThreadSafe>& Data, const FIntBox& Bounds, const FString& Name)
		: FVoxelScopeLock(&Data.Get(), Bounds, Name)
	{
	}

	inline ~FVoxelScopeLock()
	{
		SCOPE_CYCLE_COUNTER(STAT_Unlock);
		Data->Unlock<LockType>(LockedOctrees);
	}

private:
	FVoxelData* const Data;
	FVoxelLockedOctrees LockedOctrees;
};

template<EVoxelLockType LockType>
class FVoxelScopeTryLock
{
public:
	inline FVoxelScopeTryLock(FVoxelData* Data, const FIntBox& Bounds, float TimeoutInSeconds, const FString& Name)
		: Data(Data)
	{
		SCOPE_CYCLE_COUNTER(STAT_TryLock);
		LockerName = Name;
		bSuccess = Data->TryLock<LockType>(Bounds, TimeoutInSeconds, LockedOctrees, LockerName);
	}
	inline FVoxelScopeTryLock(FVoxelData& Data, const FIntBox& Bounds, float TimeoutInSeconds, const FString& Name)
		: FVoxelScopeTryLock(&Data, Bounds, TimeoutInSeconds, Name)
	{
	}
	inline FVoxelScopeTryLock(const TSharedPtr<FVoxelData, ESPMode::ThreadSafe>& Data, float TimeoutInSeconds, const FIntBox& Bounds, const FString& Name)
		: FVoxelScopeTryLock(Data.Get(), Bounds, TimeoutInSeconds, Name)
	{
	}
	inline FVoxelScopeTryLock(const TSharedRef<FVoxelData, ESPMode::ThreadSafe>& Data, float TimeoutInSeconds, const FIntBox& Bounds, const FString& Name)
		: FVoxelScopeTryLock(&Data.Get(), Bounds, TimeoutInSeconds, Name)
	{
	}

	inline ~FVoxelScopeTryLock()
	{
		if (bSuccess)
		{
			SCOPE_CYCLE_COUNTER(STAT_Unlock);
			Data->Unlock<LockType>(LockedOctrees);
		}
	}

	inline bool Success() const
	{
		return bSuccess;
	}
	inline const FString& GetLockerName() const
	{
		return LockerName;
	}

private:
	FVoxelData* const Data;
	
	bool bSuccess;
	FString LockerName;
	FVoxelLockedOctrees LockedOctrees;
};

using FVoxelReadScopeLock      = FVoxelScopeLock<EVoxelLockType::Read>;
using FVoxelReadWriteScopeLock = FVoxelScopeLock<EVoxelLockType::ReadWrite>;

using FVoxelReadScopeTryLock      = FVoxelScopeTryLock<EVoxelLockType::Read>;
using FVoxelReadWriteScopeTryLock = FVoxelScopeTryLock<EVoxelLockType::ReadWrite>;

class DEPRECATED(0, "Please use FVoxelReadScopeLock instead."     ) FVoxelScopeGetLock : public FVoxelReadScopeLock      { using FVoxelScopeLock::FVoxelScopeLock; };
class DEPRECATED(0, "Please use FVoxelReadWriteScopeLock instead.") FVoxelScopeSetLock : public FVoxelReadWriteScopeLock { using FVoxelScopeLock::FVoxelScopeLock; };