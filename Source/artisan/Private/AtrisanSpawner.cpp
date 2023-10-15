// Fill out your copyright notice in the Description page of Project Settings.


#include "AtrisanSpawner.h"

// Sets default values
AAtrisanSpawner::AAtrisanSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("LOOP")));
}

// Called when the game starts or when spawned
void AAtrisanSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (items.IsEmpty()) return;
	GetWorld()->GetTimerManager().SetTimer(SpawnCarTimer, this, &AAtrisanSpawner::SpawnCar, spawnDelay, true, FMath::RandRange(0.0f, 5.0f));
	
}

// Called every frame
void AAtrisanSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAtrisanSpawner::SpawnCar() {
	if (spawnNum-- <= 0) {
		GetWorldTimerManager().ClearTimer(SpawnCarTimer);
	}
	else {
		int rand = FMath::RandRange(0, items.Num() - 1);
		AArtisanCar* obj = GetWorld()->SpawnActor<AArtisanCar>(items[rand], GetActorLocation(), FRotator::ZeroRotator);
		if (spawnDir < 0)
			obj->SetActorRotation(FRotator(0, 180, 0));
		obj->movingDir = spawnDir;
	}
}

