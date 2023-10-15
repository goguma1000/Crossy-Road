// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtisanAnimInstance.h"
#include "ArtisanCharacter.h"

void UArtisanAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	APawn* pawn = TryGetPawnOwner();

	if (!::IsValid(pawn)) return;

	float pawnSpeed = pawn->GetVelocity().Size();
	if (pawnSpeed != 0) isRun = true;
	else isRun = false;
}

void UArtisanAnimInstance::SetisWin(bool value) {
	isWin = value;
}