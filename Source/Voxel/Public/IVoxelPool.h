// Copyright 2019 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "VoxelGlobals.h"

class FVoxelQueuedThreadPool;
class FQueuedThreadPool;
class IVoxelQueuedWork;
class IQueuedWork;

class VOXEL_API IVoxelPool
{
public:
	virtual ~IVoxelPool() {}

	virtual void QueueMeshingTask(IVoxelQueuedWork* Work) = 0;
	virtual void QueueCollisionTask(IVoxelQueuedWork* Work) = 0;
	virtual void QueueCacheTask(IVoxelQueuedWork* Work) = 0;
	virtual void QueueOctreeBuildTask(IQueuedWork* Work) = 0;
	virtual void QueueAsyncEditTask(IQueuedWork* Work) = 0;

public:
	inline static TSharedPtr<IVoxelPool> GetGlobalPool()
	{
		return Global;
	}
	inline static void DestroyGlobalVoxelPool()
	{
		Global.Reset();
		UE_LOG(LogVoxel, Log, TEXT("Global pool destroyed"));
	}
	inline static bool IsGlobalVoxelPoolCreated()
	{
		return Global.IsValid();
	}

protected:
	inline static void SetGlobalVoxelPool(const TSharedPtr<IVoxelPool>& InGlobal)
	{
		checkf(!Global.IsValid(), TEXT("Global voxel pool already created!"));
		Global = InGlobal;
		UE_LOG(LogVoxel, Log, TEXT("Global pool created"));
	}

private:
	static TSharedPtr<IVoxelPool> Global;
};