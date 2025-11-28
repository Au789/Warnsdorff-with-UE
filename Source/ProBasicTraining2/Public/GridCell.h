// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCellInterface.h"
#include "GameFramework/Actor.h"
#include "GridCell.generated.h"

class UWidgetComponent;
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnbIsVisitedChangedDelegate, FVector2D, ChangedGridCellLocation);
UCLASS()
class PROBASICTRAINING2_API AGridCell : public AActor, public IGridCellInterface
{
	GENERATED_BODY()
	
public:	
	AGridCell();
	
	FOnbIsVisitedChangedDelegate OnbIsVisitedChangedSignature;
	
	bool GetIsVisitedDuringCal() const{return bIsVisitedDuringCal;}
	void SetIsVisitedDuringCal(const bool NewIsVisited);
	UFUNCTION(BlueprintCallable)
	void SetIsVisited(const bool NewIsVisited);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D LogicLocation = FVector2D::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Step = 0;
	FVector2D LastMove = FVector2D::ZeroVector;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaterial(const UMaterial* MaterialInstance);

	virtual void HighGridCell() override;
	virtual void UnHighGridCell() override;

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetMesh(){return GridCellMesh;}
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsVisited = false;

	bool bIsVisitedDuringCal = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> GridCellMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> StepDisplay;
};
