// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtisanCharacter.h"
#include "Camera/CameraActor.h"
#include "CinematicCamera/Public/CineCameraActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealClient.h"


TArray<AActor*> ActorsToFind;
ACineCameraActor* camera;
FVector cameraOffset;
APlayerController* OurPlayerController;

// Sets default values
AArtisanCharacter::AArtisanCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_RUNNING(TEXT("/Game/character/running/Running.Running"));
	if (SK_RUNNING.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_RUNNING.Object);
		GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>CHARACTER_ANIM(TEXT("/Game/Blueprints/CharacterAnim.CharacterAnim_C"));
	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	this->SetActorScale3D(FVector(0.035f, 0.035f, 0.035f));
	GetCapsuleComponent()->SetCapsuleHalfHeight(84.0f);
	SetControlMode();
}


// Called when the game starts or when spawned
void AArtisanCharacter::BeginPlay()
{
	Super::BeginPlay();
	camera = NULL;
	OurPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

// Called every frame
void AArtisanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), DeltaTime);
	if (camera == NULL) {
		OurPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACineCameraActor::StaticClass(), FName("CineCamera"), ActorsToFind);
		if (!ActorsToFind.IsEmpty()) {
			camera = Cast<ACineCameraActor>(ActorsToFind[0]);
			OurPlayerController->SetViewTarget(camera);
			cameraOffset = camera->GetActorLocation() - GetActorLocation();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("not found"));
			GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Red, TEXT("not found"), false);
		}
	}
}

// Called to bind functionality to input
void AArtisanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AArtisanCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AArtisanCharacter::LeftRight);

}

void AArtisanCharacter::UpDown(float NewAxisValue) {
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue * movingSpeed);
	if (camera != nullptr) {
		FVector newCameraPosition = GetActorLocation() + cameraOffset;
		newCameraPosition = FVector(camera->GetActorLocation().X, newCameraPosition.Y, camera->GetActorLocation().Z);
		camera->SetActorLocation(newCameraPosition);
	}
}

void AArtisanCharacter::LeftRight(float NewAxisValue) {
	FVector2D screenLocation;
	FVector2D screenSize;
	screenSize = GEngine->GameViewport->Viewport->GetSizeXY();
	screenLocation = TransformViewCoord(screenSize);
	screenSize.X = screenSize.Y * 9 / 16;
	if ((0.05f > screenLocation.X / screenSize.X && NewAxisValue < 0) || (screenLocation.X / screenSize.X> 0.95f && NewAxisValue > 0)) {
		//ignore input
	}
	else {
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue * movingSpeed);
	}
}

void AArtisanCharacter::SetControlMode() {
	UCharacterMovementComponent* CharacterMoveComponent = GetCharacterMovement();
	bUseControllerRotationYaw = false;
	CharacterMoveComponent->bOrientRotationToMovement = true;
	CharacterMoveComponent->bUseControllerDesiredRotation = false;
	CharacterMoveComponent->RotationRate = FRotator(0, 720.0f, 0);
}

FVector2D AArtisanCharacter::TransformViewCoord(FVector2D screenSize) {
	FVector2D result;
	OurPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	OurPlayerController->ProjectWorldLocationToScreen(GetActorLocation(), result);
	result.X = result.X - (screenSize.X / 2) + (screenSize.Y * 9 / 16) / 2;
	return result;
}