// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtisanGameMode.h"
#include "ArtisanCharacter.h"

AArtisanGameMode::AArtisanGameMode() {
	DefaultPawnClass = AArtisanCharacter::StaticClass();
	//static ConstructorHelpers::FClassFinder<HUD>
}