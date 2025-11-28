// Fill out your copyright notice in the Description page of Project Settings.


#include "HorsePlayerController.h"
#include "GridCellInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GridCell.h"
#include "Horse.h"
#include "HorseGameMode.h"
#include "Kismet/GameplayStatics.h"

void AHorsePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedComponent = Cast<UEnhancedInputComponent>(InputComponent);
	
	EnhancedComponent->BindAction(PlaceHorseAction, ETriggerEvent::Triggered, this, &AHorsePlayerController::PlaceHorse);
	
}

void AHorsePlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(HorseMappingContext)
	
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(HorseMappingContext, 1);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

void AHorsePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AHorsePlayerController::CursorTrace()
{
	if (HasPlacedHorse) return;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = Cast<IGridCellInterface>(CursorHit.GetActor());

	if (LastActor != CurrentActor)
	{
		if (LastActor) LastActor->UnHighGridCell();
		if (CurrentActor) CurrentActor->HighGridCell();
	}
}

void AHorsePlayerController::PlaceHorse(const FInputActionInstance& Instance)
{
	
	if (!CurrentActor || HasPlacedHorse) return;

	AGridCell* SpawnGridCell = Cast<AGridCell>(CurrentActor);
	const FVector HorseSpawnLocation = SpawnGridCell->GetActorLocation();
	FTransform HorseSpawnTransform;
	HorseSpawnTransform.SetLocation(FVector(400.f, 400.f, 1050.f));
	HorseSpawnTransform.SetRotation(FQuat::Identity);
	AHorse* Horse = GetWorld()->SpawnActor<AHorse>(HorseClass, HorseSpawnTransform);
	Horse->SetShouldMove(true);
	Horse->SetTargetLocation(HorseSpawnLocation);
	
	AHorseGameMode* GameMode = Cast<AHorseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->SetHorse(Horse);
	GameMode->SetCurrentLocation(SpawnGridCell->LogicLocation);
	GameMode->SetStartLocation(SpawnGridCell->LogicLocation);
	GameMode->SetFirstGridCell(SpawnGridCell);
	
	SpawnGridCell->SetIsVisited(true);
	SpawnGridCell->SetIsVisitedDuringCal(true);
	SpawnGridCell->HighGridCell();
	
	GameMode->HasPlacedHorseSignature.Broadcast();

	HasPlacedHorse = true;
}
