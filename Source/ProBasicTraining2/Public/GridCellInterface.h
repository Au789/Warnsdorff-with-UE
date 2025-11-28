// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridCellInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGridCellInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROBASICTRAINING2_API IGridCellInterface
{
	GENERATED_BODY()

public:
	virtual void HighGridCell();
	virtual void UnHighGridCell();
};
