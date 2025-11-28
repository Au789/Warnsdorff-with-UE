// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HorseGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHasPlacedHorseDelegate);
class AHorse;
class AGridCell;
/**
 * 
 */
UCLASS()
class PROBASICTRAINING2_API AHorseGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 ReadyGridCellNumber = 0;

	UFUNCTION(BlueprintCallable)
	void AllReady();
	
	UFUNCTION(BlueprintCallable)
	TArray<AGridCell*> GetGridCells(){return BlueprintGridCells;}

	void SetHorse(AHorse* NewHorse){Horse = NewHorse;}

	UFUNCTION(BlueprintCallable)
	AHorse* GetHorse(){return Horse;}
	
	void SetCurrentLocation(FVector2D NewLocation){CurrentLocation = NewLocation;}

	void SetStartLocation(FVector2D NewLocation){StartLocation = NewLocation;}
	
	UPROPERTY(BlueprintAssignable, Category="Delagate")
	FHasPlacedHorseDelegate HasPlacedHorseSignature;

	void SetFirstGridCell(AGridCell* First);

	UFUNCTION(BlueprintCallable)
	void RestartChessBroad();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterial> BlackMaterial;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterial> WhiteMaterial;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterial> VisitedMaterial;

	UPROPERTY(EditDefaultsOnly)
	float GridCellWidth = 0.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGridCell> GridCellClass;

	void SpawnChessBoard();
	
	TArray<TArray<AGridCell*>> GridCells;

	TArray<AGridCell*> BlueprintGridCells;
	
	TObjectPtr<AHorse> Horse;

	UFUNCTION()
	void SetVisitedGridCellMaterial(FVector2D Location);

	void InitGridCellsTArraySize();

	UFUNCTION(blueprintCallable)
	bool SafeSolve();
	
	bool Solve();

	int32 MovementCount = 1;

	FVector2D CurrentLocation;

	int CalculateDegree(FVector2D Location);

	void SortCandidates(TArray<AGridCell*>& Candidates);
	
	FVector2D StartLocation = FVector2D::ZeroVector;

	bool CanReturnToStart(FVector2D Location);

	AGridCell* FirstGridCell;

};
