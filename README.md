# **Crossy Road**
Unreal Engine 5로 개발한 간단한 미니게임. 엔진 사용법에 익숙해지는 것을 목표로 한 프로젝트.  

코드 경로: [Scripts](https://github.com/goguma1000/Crossy-Road/tree/main/Source/artisan)  

**플레이영상:** [Link](https://youtu.be/u_VU_70GmHw)
## **목차**   
**[C++](#c)**  
**[BluePrint](#blueprint)**

### **C++**
#### **Character Class** 
- **캐릭터 이동**
~~~cpp
void AArtisanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AArtisanCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AArtisanCharacter::LeftRight);

}
~~~   
BindAxis 함수를 통해 각 키에다 함수를 매핑하여 방향키를 눌렀을 때 캐릭터가 움직일 수 있도록 한다.  

- 화면 밖 벗어남 방지(좌우)  
![제목 없는 동영상 - Clipchamp로 제작](https://github.com/goguma1000/Crossy-Road/assets/102130574/7845570d-47df-449b-80d8-61c6ef71a03d)
~~~cpp
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

FVector2D AArtisanCharacter::TransformViewCoord(FVector2D screenSize) {
	FVector2D result;
	OurPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	OurPlayerController->ProjectWorldLocationToScreen(GetActorLocation(), result);
	result.X = result.X - (screenSize.X / 2) + (screenSize.Y * 9 / 16) / 2;
	return result;
}
~~~  
</br>  
GetSizeXY 함수를 통해 현재 모니터의 width와 height를 받는다.  
화면을 9 : 16 비율로 고정하였기 때문에 TransformViewCoord 함수를 통해   
캐릭터 위치를 World Coordinate에서 9: 16 View Coordinate로 변환한다.  
캐릭터가 좌측 끝이나 우측 끝에 도달하면 입력을 무시하여 화면 밖으로 벗어나지 못하게 한다.  

</br>  

**관련 코드 링크 :**  
    [ArtisanCharacter.cpp](https://github.com/goguma1000/Crossy-Road/blob/main/Source/artisan/Private/ArtisanCharacter.cpp)    

</br>

#### **Car Class**  
- 자동차 움직임
~~~cpp
void AArtisanCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector currentPos = GetActorLocation();
	if ((movingDir < 0 && currentPos.X > 55.f) || (movingDir > 0 && currentPos.X < -55.f)) SetActorLocation(FVector(55.0f * movingDir, currentPos.Y, currentPos.Z));
	AddActorWorldOffset(FVector(-MovingSpeed * movingDir*DeltaTime, 0.0f ,0.0f));
}
~~~  
</br>  

Tick 함수를 통해 자동차의 position을 변경하여 자동차를 움직이고, 일정 위치에 도착하면 처음 위치로 돌아간다.
</br>  


- 플레이어와 충돌
~~~cpp
AArtisanCar::AArtisanCar()
{
 	...
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AArtisanCar::OnCollisionEnter);
	...
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
~~~  

Constructor에서 BeginOverlap이벤트에 OnCollisionEnter 함수를 바인드하여 해당 이벤트가 발생하면 등록된 함수를 호출한다.  
플레이어가 자동차와 충돌하면 PlayerController의 입력모드를 UIOnly로 변경하여 플레이어가 움직이지 못하도록 한다.  
CreateWidget 함수로 UI를 화면에 띄운다.  

</br>  

**관련 코드 링크 :**  
    [ArtisanCar.cpp](https://github.com/goguma1000/Crossy-Road/blob/main/Source/artisan/Private/ArtisanCar.cpp)    

</br>

### **Spawner Class**  
~~~cpp
void AAtrisanSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (items.IsEmpty()) return;
	GetWorld()->GetTimerManager().SetTimer(SpawnCarTimer, this, &AAtrisanSpawner::SpawnCar, spawnDelay, true, FMath::RandRange(0.0f, 5.0f));
	
}
~~~  
게임이 시작되면 타이머를 실행하여 자동차들을 스폰한다.  
이때 SetTimer의 InFirstDelay 변수를 랜덤하게 부여하여 각 Spawner마다 다른 타이밍에 차를 스폰하도록 함.  

</br>  

**관련 코드 링크 :**  
    [AtrisanSpawner.cpp](https://github.com/goguma1000/Crossy-Road/blob/main/Source/artisan/Private/AtrisanSpawner.cpp)    

</br>

---
### **BluePrint**
  #### Game Over UI
  ![그림1](https://github.com/goguma1000/Crossy-Road/assets/102130574/e726ad87-cecd-41f5-a23a-5c117a59630d)  

  각 버튼에 할당 된 맵을 오픈하고 플레이어가 차와 충돌했을 때 바꿔놓은 PlayerController의 입력모드를 변경한다.  

  #### Main UI
  ![그림2](https://github.com/goguma1000/Crossy-Road/assets/102130574/3591685a-d4ad-49a5-a05a-07c140cfd1b4)  

  Start Game버튼을 누르면 게임 레벨을 오픈하고 Exit Game 버튼을 누르면 콘솔 커맨드를 이용하여 게임을 종료한다.
