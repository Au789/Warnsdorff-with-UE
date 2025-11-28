// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Horse.generated.h"

UCLASS()
class PROBASICTRAINING2_API AHorse : public AActor
{
	GENERATED_BODY()
	
public:	
	AHorse();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetShouldMove(bool NewShouldMove){ShouldMove = NewShouldMove;}

	UFUNCTION(BlueprintCallable)
	FVector GetMoveTargetLocation(){return TargetLocation;}

	void SetTargetLocation(const FVector& NewTargetLocation){TargetLocation = NewTargetLocation;}
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> HorseMesh;

	TObjectPtr<USceneComponent> HorseCenterPoint;

	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	bool ShouldMove = false;
};
