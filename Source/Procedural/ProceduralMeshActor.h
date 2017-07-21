// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshActor.generated.h"

UCLASS()
class PROCEDURAL_API AProceduralMeshActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProceduralMeshActor();
	UFUNCTION(BlueprintCallable, Category = "Example Nodes")
		void SetLevel(float level);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = Materials)
		UProceduralMeshComponent* mesh;

};

//General Log
DECLARE_LOG_CATEGORY_EXTERN(YourLog, Log, All);

//Logging during game startup
DECLARE_LOG_CATEGORY_EXTERN(YourInit, Log, All);

//Logging for your AI system
DECLARE_LOG_CATEGORY_EXTERN(YourAI, Log, All);

//Logging for Critical Errors that must always be addressed
DECLARE_LOG_CATEGORY_EXTERN(YourCriticalErrors, Log, All);
