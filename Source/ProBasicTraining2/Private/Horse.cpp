// Fill out your copyright notice in the Description page of Project Settings.

#include "Horse.h"

AHorse::AHorse()
{
	PrimaryActorTick.bCanEverTick = false;

	HorseMesh = CreateDefaultSubobject<UStaticMeshComponent>("HorseMesh");
	HorseCenterPoint = CreateDefaultSubobject<USceneComponent>("HorseCenterPoint");
	SetRootComponent(HorseCenterPoint);
	HorseMesh->SetupAttachment(RootComponent);
	HorseMesh->SetRelativeLocation(FVector(0, 0, -200));
}

void AHorse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AHorse::BeginPlay()
{
	Super::BeginPlay();
	
}


