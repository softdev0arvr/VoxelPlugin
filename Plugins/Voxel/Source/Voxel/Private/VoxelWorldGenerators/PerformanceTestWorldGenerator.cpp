#include "VoxelPrivatePCH.h"
#include "PerformanceTestWorldGenerator.h"
#include "Engine/World.h"

float UPerformanceTestWorldGenerator::GetDefaultValue(int X, int Y, int Z)
{
	float CurrentRadius = (1 + FMath::Sin(Speed * World->GetWorld()->TimeSeconds)) * Radius / 2;
	return FVector(FMath::Abs(X) % (2 * Radius) - Radius, FMath::Abs(Y) % (2 * Radius) - Radius, FMath::Abs(Z) % (2 * Radius) - Radius).Size() - CurrentRadius;
}

FVoxelMaterial UPerformanceTestWorldGenerator::GetDefaultMaterial(int X, int Y, int Z)
{
	return FVoxelMaterial();
}

void UPerformanceTestWorldGenerator::SetVoxelWorld(AVoxelWorld* VoxelWorld)
{
	World = VoxelWorld;
}