// Copyright 2019 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"
#include "PhysicsEngine/BodyInstance.h"

#include "IntBox.h"
#include "VoxelUtilities.h"
#include "VoxelConfigEnums.h"
#include "VoxelWorldGeneratorPicker.h"
#include "VoxelWorldInterface.h"

#include "VoxelWorld.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWorldLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWorldDestroyed);

class IVoxelPool;
class FVoxelData;
class IVoxelLODManager;
class IVoxelRenderer;
class FVoxelCacheManager;
class FVoxelDebugManager;
class FVoxelMultiplayerManager;
struct FVoxelWorldGeneratorInit;
class UVoxelWorldSaveObject;
class UVoxelGrassSpawner;
class UVoxelActorSpawner;
class AVoxelWorldEditorInterface;
class UVoxelMaterialCollection;
struct FVoxelGrassSpawner;
struct FVoxelLODDynamicSettings;
class UVoxelProceduralMeshComponent;

/**
 * Voxel World actor class
 */
UCLASS()
class VOXEL_API AVoxelWorld : public AVoxelWorldInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnWorldLoaded OnWorldLoaded;

	// Called right before destroying the world. Use this if you want to save data
	UPROPERTY(BlueprintAssignable)
	FOnWorldDestroyed OnWorldDestroyed;

public:
	// Automatically loaded on creation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel")
	UVoxelWorldSaveObject* SaveObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel", meta = (FilePathFilter = "voxelsave"))
	FFilePath SaveFilePath;

	// If true, will save the world to SaveFilePath each time it's saved to the save object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel")
	bool bAutomaticallySaveToFile = false;

	// If true, will add the current time & date to the filepath when saving
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel")
	bool bAppendDateToSavePath = false;

	//////////////////////////////////////////////////////////////////////////////

	// Size of a voxel in cm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|General", meta = (RecreateRender, ClampMin = 0.0001))
	float VoxelSize = 100;

	// Mass in kg of a voxel (multiplied by its density)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|General")
	float VoxelMassMultiplierInKg = 1;

	// Generator of this world
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|General", meta = (Recreate))
	FVoxelWorldGeneratorPicker WorldGenerator;
	
	// Keep all the changes in memory to enable undo/redo. Can be expensive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|General", meta = (Recreate))
	bool bEnableUndoRedo = false;

	// If true, the voxel world will try to stay near its original coordinates when rebasing, and will offset the voxel coordinates instead
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|General")
	bool bEnableWorldRebasing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|General", meta = (Recreate))
	TMap<FString, int> Seeds;

	//////////////////////////////////////////////////////////////////////////////

	// WorldSizeInVoxel = CHUNK_SIZE * 2^Depth. Has little impact on performance
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voxel|World Size", meta = (Recreate, ClampMin = 1, ClampMax = 24, UIMin = 1, UIMax = 24))
	int OctreeDepth = 10;

	// Size of an edge of the world
	UPROPERTY(EditAnywhere, Category = "Voxel|World Size", meta = (Recreate, ClampMin = 1))
	int WorldSizeInVoxel = FVoxelUtilities::GetSizeFromDepth<CHUNK_SIZE>(10);

	UPROPERTY(meta = (Recreate))
	bool bUseCustomWorldBounds = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|World Size", meta = (Recreate, EditCondition = "bUseCustomWorldBounds"))
	FIntBox CustomWorldBounds;
	
	//////////////////////////////////////////////////////////////////////////////

	// Chunks can't have a LOD strictly higher than this. Useful is background has a too low resolution. WARNING: Don't set this too low, 5 under Octree Depth should be safe
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|LOD Settings", meta = (UpdateLODs, DisplayName = "LOD Limit", ClampMin = 0, ClampMax = 25, UIMin = 0, UIMax = 25))
	int LODLimit = MAX_WORLD_DEPTH - 1;

	// In world space. Chunks under this distance from voxel invokers will have at least the given LOD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|LOD Settings", meta = (UpdateLODs, DisplayName = "LOD to Min Distance"))
	TMap<FString, float> LODToMinDistance = { {"0", 1000.f } };
	
	// In world space. If invokers move by less than this distance LODs won't be updated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|LOD Settings", meta = (UpdateLODs))
	float InvokerDistanceThreshold = 100;
	
	// Number of LOD update per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|LOD Settings", meta = (RecreateRender, ClampMin = 0.000001), DisplayName = "LOD Update Rate")
	float LODUpdateRate = 15;
	
	//////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Materials", meta = (RecreateRender, UpdateAll, DisplayName = "UV Config"))
	EVoxelUVConfig UVConfig = EVoxelUVConfig::GlobalUVs;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Materials", meta = (RecreateRender, UpdateAll))
	EVoxelNormalConfig NormalConfig = EVoxelNormalConfig::GradientNormal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Materials", meta = (RecreateRender, Recreate))
	EVoxelMaterialConfig MaterialConfig = EVoxelMaterialConfig::RGB;

	// Only used if Material Config = RGB
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Materials", meta = (RecreateRender, Recreate))
	UMaterialInterface* VoxelMaterial = nullptr;

	// Only used if Material Config = RGB
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Materials", meta = (RecreateRender, Recreate, EditCondition ="bEnableTessellation"))
	UMaterialInterface* TessellatedVoxelMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Materials", meta = (RecreateRender, Recreate))
	UVoxelMaterialCollection* MaterialCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Materials", meta = (RecreateRender, Recreate))
	bool bEnableTessellation = false;
	
	// Tessellation distance in world space
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Materials", meta = (UpdateLODs, ClampMin = 0, EditCondition ="bEnableTessellation"))
	float TessellationDistance = 1000;

	// Increases the chunks bounding boxes, useful when using tessellation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Materials", meta = (RecreateRender, UpdateAll, EditCondition ="bEnableTessellation"))
	float TessellationBoundsExtension = 0;
	
	//////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Rendering", meta = (RecreateRender))
	EVoxelRenderType RenderType = EVoxelRenderType::MarchingCubes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Rendering", meta = (RecreateRender, ClampMin = 0))
	float ChunksDitheringDuration = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Rendering", meta = (RecreateRender))
	TSubclassOf<UVoxelProceduralMeshComponent> ProcMeshClass;

	// Chunks with a LOD strictly higher than this won't be rendered
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Rendering", meta = (UpdateLODs, ClampMin = 0, ClampMax = 25, UIMin = 0, UIMax = 25))
	int ChunksCullingLOD = MAX_WORLD_DEPTH - 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Rendering", meta = (RecreateRender))
	bool bRenderWorld = true;

	// Will update all the chunks modified by a single edit at once, by waiting for their completion. This will look better but will add latency
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Rendering", meta = (RecreateRender))
	bool bWaitForOtherChunksToAvoidHoles = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Rendering", meta = (RecreateRender, ClampMin = 0))
	float WaitForOtherChunksToAvoidHolesTimeout = 10;

	//////////////////////////////////////////////////////////////////////////////


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Collisions & Navmesh", meta = (RecreateRender))
	bool bEnableCollisions = true;

	// If false, use only invokers collisions settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Collisions & Navmesh", meta = (RecreateRender, EditCondition = bEnableCollisions))
	bool bComputeVisibleChunksCollisions = true;

	// Max LOD to compute collisions on. Inclusive. If not 0 collisions won't be precise
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Collisions & Navmesh", meta = (RecreateRender, ClampMin = 0, ClampMax = 25, UIMin = 0, UIMax = 25, EditCondition = bEnableCollisions))
	int VisibleChunksCollisionsMaxLOD = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Collisions & Navmesh", meta = (RecreateRender, EditCondition = bEnableCollisions))
	FBodyInstance CollisionPresets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Collisions & Navmesh", meta = (RecreateRender))
	bool bEnableNavmesh = false;
	
	//////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Cache", meta = (Recreate, ClampMin = 0))
	float DataOctreeCompactDelayInSeconds = 30;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Cache", meta = (Recreate))
	bool bEnableAutomaticCache = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Cache", meta = (Recreate, ClampMin = 0, EditCondition = bEnableAutomaticCache))
	float CacheUpdateDelayInSeconds = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Cache", meta = (Recreate, EditCondition = bEnableAutomaticCache, ClampMin = 0))
	int CacheAccessThreshold = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Cache", meta = (Recreate, EditCondition = bEnableAutomaticCache))
	bool bCacheLOD0Chunks = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Cache", meta = (Recreate, EditCondition = bEnableAutomaticCache, ClampMin = 0))
	int MaxCacheSize = 10000;

	UPROPERTY(VisibleAnywhere, Category = "Voxel|Cache", meta = (Recreate, EditCondition = bEnableAutomaticCache))
	int MaxCacheSizeInMB = 0;
	
	//////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Threads")
	bool bCreateGlobalPool = true;

	// Number of threads allocated for the mesh processing. Setting it too high may impact performance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Threads", meta = (Recreate, ClampMin = 1, EditCondition = "!bUseGlobalPool"))
	int MeshThreadCount = 2;
	
	//////////////////////////////////////////////////////////////////////////////

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Misc")
	bool bCreateWorldAutomatically = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Misc", meta = (DisplayName = "Use camera if no invokers found"))
	bool bUseCameraIfNoInvokersFound = false;
	
	// If true, the mesh indices will be sorted to improve GPU cache performance. Adds a cost to the async mesh building. If you don't see any perf difference, leave it off
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxel|Misc", meta = (RecreateRender))
	bool bOptimizeIndices = false;

public:
	AVoxelWorld();
	~AVoxelWorld();

public:
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	void CreateWorld();
	
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	void DestroyWorld();

public:
	inline IVoxelPool& GetPool() const { return *Pool; }
	inline FVoxelData& GetData() const { return *Data; }
	inline IVoxelLODManager& GetLODManager() const { return *LODManager; }
	inline IVoxelRenderer& GetRenderer() const { return *Renderer; }
	inline FVoxelCacheManager& GetCacheManager() const { return *CacheManager; }
	inline FVoxelDebugManager& GetDebugManager() const { return *DebugManager; }

	inline const FVoxelWorldGeneratorPicker GetWorldGeneratorPicker() const { return WorldGenerator; }
	FVoxelWorldGeneratorInit GetInitStruct() const;
	inline TSharedPtr<FVoxelData, ESPMode::ThreadSafe> GetDataSharedPtr() const { return Data; }
	
public:
	UFUNCTION(BlueprintCallable, Category = "Voxel|General")
	void SetWorldGeneratorObject(UVoxelWorldGenerator* NewGenerator);
	UFUNCTION(BlueprintCallable, Category = "Voxel|General")
	void SetWorldGeneratorClass(TSubclassOf<UVoxelWorldGenerator> NewGeneratorClass);

	// Adds to existing seeds
	UFUNCTION(BlueprintCallable, Category = "Voxel|General")
	void AddSeeds(const TMap<FString, int>& NewSeeds);
	UFUNCTION(BlueprintCallable, Category = "Voxel|General")
	void AddSeed(FString Name, int Value);
	UFUNCTION(BlueprintCallable, Category = "Voxel|General")
	void ClearSeeds();

	UFUNCTION(BlueprintCallable, Category = "Voxel|World Size")
	void SetOctreeDepth(int NewDepth);

public:
	// Is this world created?
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	inline bool IsCreated() const { return bIsCreated; }

public:
	/**
	 * Convert position from world space to voxel space
	 * @param	Position	Position in world space
	 * @return	Position in voxel space
	 */
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	FIntVector GlobalToLocal(const FVector& Position) const final;
	// Precision errors with large world offset
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	FVector GlobalToLocalFloat(const FVector& Position) const final;

	/**
	 * Convert position from voxel space to world space
	 * @param	Position	Position in voxel space
	 * @return	Position in world space
	 */
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	FVector LocalToGlobal(const FIntVector& Position) const final;
	// Precision errors with large world offset
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	FVector LocalToGlobalFloat(const FVector& Position) const final;

	/**
	 * Get the 8 neighbors in voxel space of GlobalPosition
	 * @param	GlobalPosition	The position in world space
	 * @return	The 8 neighbors in voxel space 
	 */
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	TArray<FIntVector> GetNeighboringPositions(const FVector& GlobalPosition) const;
	
	/**
	 * Add an offset to the world coordinate system (eg for rebasing)
	 * @param	OffsetInVoxels	Offset
	 * @param	bMoveActor		If false, the actor will keep its current position
	 */
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	 void AddOffset(const FIntVector& OffsetInVoxels, bool bMoveActor = true);

public:
	//~ Begin AActor Interface
	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	void BeginDestroy() override;
	void Tick(float DeltaTime) override;
	void Serialize(FArchive& Ar) override;
	void ApplyWorldOffset(const FVector& InOffset, bool bWorldShift) override;
	void PostLoad() override;
#if WITH_EDITOR
	bool ShouldTickIfViewportsOnly() const override;
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	//~ End AActor Interface

	void UpdateCollisionProfile();

private:
	UPROPERTY(Transient)
	AActor* VoxelWorldEditor;

	bool bIsCreated = false;
	bool bIsPreviewing = false;
	double TimeOfCreation = 0;

private:	
	TSharedPtr<FVoxelDebugManager, ESPMode::ThreadSafe> DebugManager;
	TSharedPtr<FVoxelData, ESPMode::ThreadSafe> Data;
	TSharedPtr<IVoxelPool> Pool;
	TSharedPtr<IVoxelRenderer> Renderer;
	TSharedPtr<IVoxelLODManager> LODManager;
	TSharedPtr<FVoxelCacheManager> CacheManager;
	TSharedPtr<FVoxelMultiplayerManager> MultiplayerManager;


	TSharedRef<FIntVector> WorldOffset = MakeShared<FIntVector>(FIntVector::ZeroValue);
	TSharedPtr<FVoxelLODDynamicSettings> LODDynamicSettings;

private:
	void SyncRefs();

	FIntBox GetWorldBounds() const;
	
	void OnWorldLoadedCallback();

	TSharedRef<FVoxelDebugManager, ESPMode::ThreadSafe> CreateDebugManager() const;
	TSharedRef<FVoxelData, ESPMode::ThreadSafe> CreateData() const;
	TSharedRef<IVoxelPool> CreatePool() const;
	TSharedRef<IVoxelRenderer> CreateRenderer() const;
	TSharedRef<IVoxelLODManager> CreateLODManager() const;
	TSharedRef<FVoxelCacheManager> CreateCacheManager() const;

	void CreateWorldInternal();
	void DestroyWorldInternal();
	void DestroyVoxelComponents();

public:
	void LoadFromSaveObject();
	void ApplyPlaceableItems();

#if WITH_EDITOR
	FSimpleMulticastDelegate OnRecreate;
	FSimpleMulticastDelegate OnRecreateRender;
	FSimpleMulticastDelegate OnUpdateAll;
	FSimpleMulticastDelegate OnUpdateLODs;

	void CreateForPreview(UClass* VoxelWorldEditorClass);
	void SaveData();
	void LoadFromSaveObjectEditor();
	bool SaveToFile(const FString& Path, FText& Error);
	bool LoadFromFile(const FString& Path, FText& Error);
	FString GetDefaultFilePath() const;
	
	void OnPreSaveWorld(uint32 SaveFlags, UWorld* World);
	void OnPreBeginPIE(bool bIsSimulating);
	void OnEndPIE(bool bIsSimulating);
	void OnPrepareToCleanseEditorObject(UObject* Object);
	void OnPreExit();
#endif
};

#if CPP // UHT is dumb
#if WITH_EDITOR
class VOXEL_API IVoxelWorldEditor
{
public:
	virtual ~IVoxelWorldEditor() = default;

	virtual UVoxelWorldSaveObject* CreateSaveObject() = 0;
	virtual void BindEditorDelegates(AVoxelWorld* World) = 0;

public:
	// Sets the voxel world editor implementation.*
	static void SetVoxelWorldEditor(TSharedPtr<IVoxelWorldEditor> InVoxelWorldEditor);
	inline static IVoxelWorldEditor* GetVoxelWorldEditor() { return VoxelWorldEditor.Get(); }

private:
	// Ptr to interface to voxel editor operations.
	static TSharedPtr<IVoxelWorldEditor> VoxelWorldEditor;
};
#endif
#endif