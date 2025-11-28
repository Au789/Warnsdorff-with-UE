// Fill out your copyright notice in the Description page of Project Settings.

#include "GridCell.h"

#include "Components/WidgetComponent.h"

AGridCell::AGridCell()
{
	PrimaryActorTick.bCanEverTick = false;

	GridCellMesh = CreateDefaultSubobject<UStaticMeshComponent>("GridCellMesh");
	SetRootComponent(GridCellMesh);
	GridCellMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	StepDisplay = CreateDefaultSubobject<UWidgetComponent>("StepDisplay");
	StepDisplay->SetupAttachment(RootComponent);
	GridCellMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
}

void AGridCell::SetIsVisitedDuringCal(const bool NewIsVisited)
{
	bIsVisitedDuringCal = NewIsVisited;
}

void AGridCell::SetIsVisited(const bool NewIsVisited)
{
	bIsVisited = NewIsVisited;
	if (NewIsVisited) OnbIsVisitedChangedSignature.Execute(LogicLocation);
}

void AGridCell::HighGridCell()
{
	GridCellMesh->SetRenderCustomDepth(true);
	GridCellMesh->SetCustomDepthStencilValue(250.f);
}

void AGridCell::UnHighGridCell()
{
	GridCellMesh->SetRenderCustomDepth(false);
}

void AGridCell::BeginPlay()
{
	Super::BeginPlay();
	
}

