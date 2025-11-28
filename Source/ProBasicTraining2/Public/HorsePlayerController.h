// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HorsePlayerController.generated.h"

struct FInputActionInstance;
class AHorse;
class UInputAction;
class IGridCellInterface;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class PROBASICTRAINING2_API AHorsePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetHasPlaceHorse(const bool bInHasPlaceHorse){HasPlacedHorse = bInHasPlaceHorse;}

protected:
	bool HasPlacedHorse = true;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHorse> HorseClass;

	FHitResult CursorHit;

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> HorseMappingContext;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> PlaceHorseAction;

	virtual void PlayerTick(float DeltaTime) override;

	void CursorTrace();

	IGridCellInterface* CurrentActor;
	IGridCellInterface* LastActor;

	UFUNCTION()
	void PlaceHorse(const FInputActionInstance& Instance);
};
