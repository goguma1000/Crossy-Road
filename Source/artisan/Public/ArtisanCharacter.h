// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/Character.h"
#include "ArtisanCharacter.generated.h"

UCLASS()
class ARTISAN_API AArtisanCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AArtisanCharacter();
	//virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = State)
	float movingSpeed = 0.035;

	UPROPERTY(EditAnyWhere)
	FRotator cameraRotation;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void SetControlMode();
	FVector2D TransformViewCoord(FVector2D Resolution);
};
