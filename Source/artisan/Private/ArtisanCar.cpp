// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtisanCar.h"
#include "ArtisanCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "Components/TextBlock.h"

TSubclassOf<UUserWidget> GameOverUI;
// Sets default values
AArtisanCar::AArtisanCar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX_COLLISION"));
	SetRootComponent(BoxCollision);
	BoxCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	BoxCollision->CanCharacterStepUpOn = ECB_No;
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AArtisanCar::OnCollisionEnter);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATIC_MESH"));
	StaticMesh->SetupAttachment(BoxCollision);
	
	static ConstructorHelpers::FClassFinder<UUserWidget>GAMEOVER_UI(TEXT("/Game/Blueprints/UI/GameOverUI.GameOverUI_C"));
	if (GAMEOVER_UI.Succeeded()) {
		GameOverUI = GAMEOVER_UI.Class;
	}

}

// Called when the game starts or when spawned
void AArtisanCar::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AArtisanCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector currentPos = GetActorLocation();
	if ((movingDir < 0 && currentPos.X > 55.f) || (movingDir > 0 && currentPos.X < -55.f)) SetActorLocation(FVector(55.0f * movingDir, currentPos.Y, currentPos.Z));
	AddActorWorldOffset(FVector(-MovingSpeed * movingDir*DeltaTime, 0.0f ,0.0f));
}

void AArtisanCar::OnCollisionEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult) {
	GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, TEXT("Collision Enter"));
	MovingSpeed = 0.0f;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
	AArtisanCharacter* character = Cast<AArtisanCharacter>(OtherActor);
	character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	character->movingSpeed = 0.0f;
	UUserWidget* currentWidget = CreateWidget(GetWorld(), GameOverUI);
	Cast<UTextBlock>(currentWidget->GetWidgetFromName(TEXT("Message")))->SetText(FText::FromString("Game Over!"));
	currentWidget->AddToViewport();
}

