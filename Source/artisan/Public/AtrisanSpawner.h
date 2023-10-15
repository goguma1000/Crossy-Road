// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtisanCar.h"
#include "Engine/EngineTypes.h"
#include "AtrisanSpawner.generated.h"

UCLASS()
class ARTISAN_API AAtrisanSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAtrisanSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = Spawner)
	TArray<TSubclassOf<AArtisanCar>> items;
	UPROPERTY(EditAnywhere, Category = Spawner)
	float spawnNum = 1;
	UPROPERTY(EditAnywhere, Category = Spawner)
	float spawnDir = 1;
	UPROPERTY(EditAnywhere, Category = Spawner)
	float spawnDelay = 1;

private:
	FTimerHandle SpawnCarTimer = { };
	void SpawnCar();
};
