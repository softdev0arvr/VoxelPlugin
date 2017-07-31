// Fill out your copyright notice in the Description page of Project Settings.

#include "VoxelChunk.h"
#include "VoxelWorld.h"
#include "VoxelData.h"
#include "EngineGlobals.h"
#include "Engine.h"
#include "Transvoxel.h"
#include "DrawDebugHelpers.h"
#include "VoxelCollisionChunk.h"
#include "RuntimeMeshComponent.h"
#include "VoxelTransitionChunk.h"
#include <vector>

// Sets default values
AVoxelChunk::AVoxelChunk() : bCollisionDirty(true)
{
	// Create primary mesh
	PrimaryMesh = CreateDefaultSubobject<URuntimeMeshComponent>(FName("PrimaryMesh"));
	RootComponent = PrimaryMesh;
}

// Called when the game starts or when spawned
void AVoxelChunk::BeginPlay()
{
	Super::BeginPlay();

	CollisionChunk = GetWorld()->SpawnActor<AVoxelCollisionChunk>(FVector::ZeroVector, FRotator::ZeroRotator);
	CollisionChunk->VoxelChunk = this;
	CollisionChunk->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	CollisionChunk->SetActorLabel("VoxelCollisionChunkActor");
	CollisionChunk->SetActorRelativeLocation(FVector::ZeroVector);
	CollisionChunk->SetActorRelativeRotation(FRotator::ZeroRotator);
	CollisionChunk->SetActorRelativeScale3D(FVector::OneVector);
}

void AVoxelChunk::Init(FIntVector position, int depth, AVoxelWorld* world)
{
	check(world);

	Position = position;
	Depth = depth;
	World = world;

	FString name = FString::FromInt(position.X) + ", " + FString::FromInt(position.Y) + ", " + FString::FromInt(position.Z);
	FVector relativeLocation = (FVector)position;

	this->AttachToActor(world, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	this->SetActorLabel(name);
	this->SetActorRelativeLocation(relativeLocation);
	this->SetActorRelativeRotation(FRotator::ZeroRotator);
	this->SetActorRelativeScale3D(FVector::OneVector);
	PrimaryMesh->SetMaterial(0, world->VoxelMaterial);
	PrimaryMesh->bCastShadowAsTwoSided = true;

	XMinChunk = GetWorld()->SpawnActor<AVoxelTransitionChunk>(FVector::ZeroVector, FRotator::ZeroRotator);
	XMinChunk->Init(World, this, Position, Depth, XMin);
	XMinChunk->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	XMinChunk->SetActorLabel("XMin");
	XMinChunk->SetActorRelativeLocation(FVector::ZeroVector);
	XMinChunk->SetActorRelativeRotation(FRotator::ZeroRotator);
	XMinChunk->SetActorRelativeScale3D(FVector::OneVector);

	XMaxChunk = GetWorld()->SpawnActor<AVoxelTransitionChunk>(FVector::ZeroVector, FRotator::ZeroRotator);
	XMaxChunk->Init(World, this, Position, Depth, XMax);
	XMaxChunk->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	XMaxChunk->SetActorLabel("XMax");
	XMaxChunk->SetActorRelativeLocation(FVector::ZeroVector);
	XMaxChunk->SetActorRelativeRotation(FRotator::ZeroRotator);
	XMaxChunk->SetActorRelativeScale3D(FVector::OneVector);

	YMinChunk = GetWorld()->SpawnActor<AVoxelTransitionChunk>(FVector::ZeroVector, FRotator::ZeroRotator);
	YMinChunk->Init(World, this, Position, Depth, YMin);
	YMinChunk->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	YMinChunk->SetActorLabel("YMin");
	YMinChunk->SetActorRelativeLocation(FVector::ZeroVector);
	YMinChunk->SetActorRelativeRotation(FRotator::ZeroRotator);
	YMinChunk->SetActorRelativeScale3D(FVector::OneVector);

	YMaxChunk = GetWorld()->SpawnActor<AVoxelTransitionChunk>(FVector::ZeroVector, FRotator::ZeroRotator);
	YMaxChunk->Init(World, this, Position, Depth, YMax);
	YMaxChunk->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	YMaxChunk->SetActorLabel("YMax");
	YMaxChunk->SetActorRelativeLocation(FVector::ZeroVector);
	YMaxChunk->SetActorRelativeRotation(FRotator::ZeroRotator);
	YMaxChunk->SetActorRelativeScale3D(FVector::OneVector);

	ZMinChunk = GetWorld()->SpawnActor<AVoxelTransitionChunk>(FVector::ZeroVector, FRotator::ZeroRotator);
	ZMinChunk->Init(World, this, Position, Depth, ZMin);
	ZMinChunk->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	ZMinChunk->SetActorLabel("ZMin");
	ZMinChunk->SetActorRelativeLocation(FVector::ZeroVector);
	ZMinChunk->SetActorRelativeRotation(FRotator::ZeroRotator);
	ZMinChunk->SetActorRelativeScale3D(FVector::OneVector);

	ZMaxChunk = GetWorld()->SpawnActor<AVoxelTransitionChunk>(FVector::ZeroVector, FRotator::ZeroRotator);
	ZMaxChunk->Init(World, this, Position, Depth, ZMax);
	ZMaxChunk->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	ZMaxChunk->SetActorLabel("ZMax");
	ZMaxChunk->SetActorRelativeLocation(FVector::ZeroVector);
	ZMaxChunk->SetActorRelativeRotation(FRotator::ZeroRotator);
	ZMaxChunk->SetActorRelativeScale3D(FVector::OneVector);
}

void AVoxelChunk::Update(URuntimeMeshComponent* mesh, bool bCreateCollision)
{
	if (mesh == nullptr)
	{
		mesh = PrimaryMesh;
	}

	/**
	* Initialize
	*/
	Vertices.clear();
	Triangles.clear();
	VerticesCount = 0;
	TrianglesCount = 0;

	int Step = 1 << Depth;
	XMinChunkHasHigherRes = HasChunkHigherRes(-Step, 0, 0);
	XMaxChunkHasHigherRes = HasChunkHigherRes(Step * 16, 0, 0);
	YMinChunkHasHigherRes = HasChunkHigherRes(0, -Step, 0);
	YMaxChunkHasHigherRes = HasChunkHigherRes(0, Step * 16, 0);
	ZMinChunkHasHigherRes = HasChunkHigherRes(0, 0, -Step);
	ZMaxChunkHasHigherRes = HasChunkHigherRes(0, 0, Step * 16);

	if (XMinChunkHasHigherRes)
	{
		XMinChunk->Update();
	}
	if (XMaxChunkHasHigherRes)
	{
		XMaxChunk->Update();
	}
	if (YMinChunkHasHigherRes)
	{
		YMinChunk->Update();
	}
	if (YMaxChunkHasHigherRes)
	{
		YMaxChunk->Update();
	}
	if (ZMinChunkHasHigherRes)
	{
		ZMinChunk->Update();
	}
	if (ZMaxChunkHasHigherRes)
	{
		ZMaxChunk->Update();
	}

	/**
	 * Polygonize
	 */
	for (int z = -1; z < 17; z++)
	{
		for (int y = -1; y < 17; y++)
		{
			for (int x = -1; x < 17; x++)
			{
				short validityMask = (x == -1 ? 0 : 1) + (y == -1 ? 0 : 2) + (z == -1 ? 0 : 4);
				TransvoxelTools::RegularPolygonize(this, x, y, z, validityMask, Triangles, TrianglesCount, Vertices, VerticesProperties, VerticesCount);
			}
		}
		NewCacheIs1 = !NewCacheIs1;
	}

	/**
	 * Compute normals + tangents & final arrays
	 */
	TArray<FVector> VerticesArray;
	TArray<int> BijectionArray;
	TArray<int> InverseBijectionArray;
	TArray<VertexProperties> VerticesPropertiesArray;

	VerticesArray.SetNumUninitialized(VerticesCount);
	BijectionArray.SetNumUninitialized(VerticesCount);
	InverseBijectionArray.SetNumUninitialized(VerticesCount);
	VerticesPropertiesArray.SetNumUninitialized(VerticesCount);

	// Fill arrays
	int cleanedIndex = 0;
	for (int i = VerticesCount - 1; i >= 0; i--)
	{
		FVector Vertex = Vertices.front();
		auto Properties = VerticesProperties.front();

		VerticesArray[i] = Vertex;
		VerticesPropertiesArray[i] = Properties;
		if (!Properties.IsNormalOnly)
		{
			InverseBijectionArray[cleanedIndex] = i;
			BijectionArray[i] = cleanedIndex;
			cleanedIndex++;
		}
		else
		{
			BijectionArray[i] = -1;
		}

		Vertices.pop_front();
		VerticesProperties.pop_front();
	}
	const int RealVerticesCount = cleanedIndex;

	TArray<int> TrianglesArray;
	TArray<FVector> NormalsArray;
	TArray<FVector> TangentsArray;
	TrianglesArray.SetNumUninitialized(TrianglesCount);
	NormalsArray.SetNumZeroed(RealVerticesCount);
	TangentsArray.SetNumZeroed(RealVerticesCount);

	// Compute normals from real triangles & Add triangles
	int i = 0;
	for (auto it = Triangles.begin(); it != Triangles.end(); ++it)
	{
		int a = *it;
		int ba = BijectionArray[a];
		++it;
		int b = *it;
		int bb = BijectionArray[b];
		++it;
		int c = *it;
		int bc = BijectionArray[c];

		FVector A = VerticesArray[a];
		FVector B = VerticesArray[b];
		FVector C = VerticesArray[c];
		FVector N = FVector::CrossProduct(C - A, B - A).GetSafeNormal();

		// Add triangles
		if (ba != -1 && bb != -1 && bc != -1)
		{
			TrianglesArray[i] = ba;
			TrianglesArray[i + 1] = bb;
			TrianglesArray[i + 2] = bc;
			i += 3;
		}

		//TODO: better tangents
		if (ba != -1)
		{
			NormalsArray[ba] += N;
			TangentsArray[ba] += C - A;
		}
		if (bb != -1)
		{
			NormalsArray[bb] += N;
			TangentsArray[bb] += C - A;
		}
		if (bc != -1)
		{
			NormalsArray[bc] += N;
			TangentsArray[bc] += C - A;
		}
	}
	TrianglesArray.SetNumUninitialized(i);

	// Normalize & convert to FRuntimeMeshTangent
	TArray<FRuntimeMeshTangent> RealTangentsArray;
	RealTangentsArray.SetNumUninitialized(RealVerticesCount);
	for (int i = 0; i < RealVerticesCount; i++)
	{
		RealTangentsArray[i] = FRuntimeMeshTangent(TangentsArray[i].GetSafeNormal());
		NormalsArray[i].Normalize();
	}

	// Compute final vertice array
	TArray<FVector> CleanedVerticesArray;
	CleanedVerticesArray.SetNumUninitialized(RealVerticesCount);
	for (int i = 0; i < RealVerticesCount; i++)
	{
		int j = InverseBijectionArray[i];
		CleanedVerticesArray[i] = GetTranslated(VerticesArray[j], NormalsArray[i], VerticesPropertiesArray[j]);
	}


	TArray<FVector2D> UV0;
	TArray<FColor> VertexColors;

	check(RealTangentsArray.Num() == CleanedVerticesArray.Num() && NormalsArray.Num() == CleanedVerticesArray.Num());

	if (VerticesArray.Num() != 0)
	{
		if (mesh->DoesSectionExist(0))
		{
			mesh->UpdateMeshSection(0, CleanedVerticesArray, TrianglesArray, NormalsArray, UV0, VertexColors, RealTangentsArray, ESectionUpdateFlags::MoveArrays);
		}
		else
		{
			mesh->CreateMeshSection(0, CleanedVerticesArray, TrianglesArray, NormalsArray, UV0, VertexColors, RealTangentsArray, bCreateCollision, EUpdateFrequency::Frequent);
		}
	}
	bCollisionDirty = true;
}

void AVoxelChunk::Unload()
{
	if (this->IsValidLowLevel())
	{
		if (!this->IsPendingKill())
		{
			if (!this->CollisionChunk->IsPendingKill())
			{
				this->CollisionChunk->Destroy();
			}
			this->Destroy();
		}
	}
}

void AVoxelChunk::SaveVertex(int x, int y, int z, short edgeIndex, int index)
{
	auto NewCache = NewCacheIs1 ? Cache1 : Cache2;
	NewCache[1 + x][1 + y][edgeIndex] = index;
}

int AVoxelChunk::LoadVertex(int x, int y, int z, short direction, short edgeIndex)
{
	auto NewCache = NewCacheIs1 ? Cache1 : Cache2;
	auto OldCache = NewCacheIs1 ? Cache2 : Cache1;

	bool xIsDifferent = direction & 0x01;
	bool yIsDifferent = direction & 0x02;
	bool zIsDifferent = direction & 0x04;
	return (zIsDifferent ? OldCache : NewCache)[1 + x - (xIsDifferent ? 1 : 0)][1 + y - (yIsDifferent ? 1 : 0)][edgeIndex];
}

int AVoxelChunk::GetDepth()
{
	return Depth;
}

signed char AVoxelChunk::GetValue(int x, int y, int z)
{
	return World->GetValue(Position + FIntVector(x, y, z));
}

bool AVoxelChunk::HasChunkHigherRes(int x, int y, int z)
{
	FIntVector P = Position + FIntVector(x, y, z);
	if (World->IsInWorld(P))
	{
		return Depth > World->GetDepthAt(P);
	}
	else
	{
		return false;
	}
}

FVector AVoxelChunk::GetTranslated(FVector V, FVector N, VertexProperties P)
{
	// If an adjacent block is rendered at the same resolution, return primary position
	if ((P.IsNearXMin && !XMinChunkHasHigherRes) || (P.IsNearXMax && !XMaxChunkHasHigherRes) ||
		(P.IsNearYMin && !YMinChunkHasHigherRes) || (P.IsNearYMax && !YMaxChunkHasHigherRes) ||
		(P.IsNearZMin && !ZMinChunkHasHigherRes) || (P.IsNearZMax && !ZMaxChunkHasHigherRes))
	{
		return V;
	}


	float DeltaX = 0;
	float DeltaY = 0;
	float DeltaZ = 0;

	float TwoPowerK = 1 << Depth;
	float w = TwoPowerK / 4;

	if ((P.IsNearXMin && XMinChunkHasHigherRes) || (P.IsNearXMax && XMaxChunkHasHigherRes))
	{
		if (V.X < TwoPowerK)
		{
			DeltaX = (1 - V.X / TwoPowerK) * w;
		}
		else if (V.X > TwoPowerK * (16 - 1))
		{
			DeltaX = (16 - 1 - V.X / TwoPowerK) * w;
		}
	}
	if ((P.IsNearYMin && YMinChunkHasHigherRes) || (P.IsNearYMax && YMaxChunkHasHigherRes))
	{
		if (V.Y < TwoPowerK)
		{
			DeltaY = (1 - V.Y / TwoPowerK) * w;
		}
		else if (V.Y > TwoPowerK * (16 - 1))
		{
			DeltaY = (16 - 1 - V.Y / TwoPowerK) * w;
		}
	}
	if ((P.IsNearZMin && ZMinChunkHasHigherRes) || (P.IsNearZMax && ZMaxChunkHasHigherRes))
	{
		if (V.Z < TwoPowerK)
		{
			DeltaZ = (1 - V.Z / TwoPowerK) * w;
		}
		else if (V.Z > TwoPowerK * (16 - 1))
		{
			DeltaZ = (16 - 1 - V.Z / TwoPowerK) * w;
		}
	}

	FVector Q = FVector(
		(1 - N.X * N.X) * DeltaX - N.X * N.Y * DeltaY - N.X * N.Z * DeltaZ,
		-N.X * N.Y * DeltaX + (1 - N.Y * N.Y) * DeltaY - N.Y * N.Z * DeltaZ,
		-N.X * N.Z * DeltaX - N.Y * N.Z * DeltaY + (1 - N.Z * N.Z) * DeltaZ);
	return V + FVector(DeltaX, DeltaY, DeltaZ);
}

bool AVoxelChunk::IsNormalOnly(FVector vertex)
{
	int Step = 1 << Depth;
	return vertex.X < 0 || vertex.Y < 0 || vertex.Z < 0 || vertex.X > 16 * Step || vertex.Y > 16 * Step || vertex.Z > 16 * Step;
}