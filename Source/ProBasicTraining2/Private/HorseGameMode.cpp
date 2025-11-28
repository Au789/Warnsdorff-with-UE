// Fill out your copyright notice in the Description page of Project Settings.


#include "HorseGameMode.h"
#include "ProBasicTraining2/ProBasicTraining2.h"
#include "GridCell.h"
#include "Horse.h"
#include "HorsePlayerController.h"
#include "Kismet/GameplayStatics.h"

void AHorseGameMode::AllReady()
{
	AHorsePlayerController* PC = Cast<AHorsePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PC->SetHasPlaceHorse(false);
}

void AHorseGameMode::SetFirstGridCell(AGridCell* First)
{
	FirstGridCell = First;
}

void AHorseGameMode::RestartChessBroad()
{
	AHorsePlayerController* PC = Cast<AHorsePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PC->SetHasPlaceHorse(false);

	Horse->Destroy();
	CurrentLocation = FVector2D::Zero();
	StartLocation = FVector2D::Zero();

	FirstGridCell->UnHighGridCell();
	FVector2D FirstGridCellLocation = FirstGridCell->LogicLocation;
	if (fmod((FirstGridCellLocation.X + FirstGridCellLocation.Y), double(2)) == 0)
	{
		FirstGridCell->SetMaterial(WhiteMaterial);
	}
	else
	{
		FirstGridCell->SetMaterial(BlackMaterial);
	}
	FirstGridCell->SetIsVisited(false);
	FirstGridCell->SetIsVisitedDuringCal(false);
	FirstGridCell = nullptr;

	MovementCount = 1;
	BlueprintGridCells.Empty();
	for (auto RowGridCells : GridCells)
	{
		for (auto GridCell : RowGridCells)
		{
			GridCell->SetIsVisited(false);
			GridCell->SetIsVisitedDuringCal(false);
		}
	}
}

void AHorseGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitGridCellsTArraySize();
	SpawnChessBoard();
}

void AHorseGameMode::SpawnChessBoard()
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			FVector SpawnLocation = FVector(x * GridCellWidth, y * GridCellWidth, 0);
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SpawnLocation);
			SpawnTransform.SetRotation(FQuat::Identity);
			AGridCell* GridCell = GetWorld()->SpawnActor<AGridCell>(GridCellClass, SpawnTransform);

			GridCell->SetActorHiddenInGame(true);
			if ((x + y) % 2 == 0)
			{
				GridCell->SetMaterial(WhiteMaterial);
			}
			else
			{
				GridCell->SetMaterial(BlackMaterial);
			}
			GridCell->LogicLocation = FVector2D(x, y);
			GridCells[x][y] = GridCell;
			BlueprintGridCells.Add(GridCell);

			GridCell->OnbIsVisitedChangedSignature.BindDynamic(this, &AHorseGameMode::SetVisitedGridCellMaterial);
		}
	}
}

void AHorseGameMode::SetVisitedGridCellMaterial(FVector2D Location)
{
	GridCells[Location.X][Location.Y]->SetMaterial(VisitedMaterial);
}

void AHorseGameMode::InitGridCellsTArraySize()
{
	GridCells.SetNum(CHESSBOARD_SIZE);
	for (int32 i = 0; i < GridCells.Num(); ++i)
	{
		GridCells[i].SetNum(CHESSBOARD_SIZE);
	}
}

bool AHorseGameMode::SafeSolve()
{
	BlueprintGridCells.Empty();
	BlueprintGridCells.Add(FirstGridCell);
	if (Solve())
	{
		BlueprintGridCells.Add(FirstGridCell);
		return true;
	}
	return false;
}

bool AHorseGameMode::Solve()
{
	if (MovementCount == CHESSBOARD_SIZE * CHESSBOARD_SIZE && CanReturnToStart(CurrentLocation))
	{
		return true;
	}

	TArray<AGridCell*> Candidates;
	for (int i = 0; i < 8; ++i)
	{
		int Nx = CurrentLocation.X + dx[i];
		int Ny = CurrentLocation.Y + dy[i];
		if (Nx >= 0 && Nx < CHESSBOARD_SIZE && Ny >= 0 && Ny < CHESSBOARD_SIZE && !GridCells[Nx][Ny]->
			GetIsVisitedDuringCal())
		{
			GridCells[Nx][Ny]->LastMove = FVector2D(dx[i], dy[i]);
			Candidates.Add(GridCells[Nx][Ny]);
		}
	}

	if (Candidates.IsEmpty())
		return false;

	SortCandidates(Candidates);

	for (int32 i = 0; i < Candidates.Num(); i++)
	{
		CurrentLocation.X = Candidates[i]->LogicLocation.X;
		CurrentLocation.Y = Candidates[i]->LogicLocation.Y;
		GridCells[CurrentLocation.X][CurrentLocation.Y]->SetIsVisitedDuringCal(true);
		MovementCount++;
		GridCells[CurrentLocation.X][CurrentLocation.Y]->Step = MovementCount;
		BlueprintGridCells.Add(GridCells[CurrentLocation.X][CurrentLocation.Y]);

		if (Solve())
			return true;

		GridCells[CurrentLocation.X][CurrentLocation.Y]->SetIsVisitedDuringCal(false);
		GridCells[CurrentLocation.X][CurrentLocation.Y]->Step = 0;
		FVector2D LastMove = GridCells[CurrentLocation.X][CurrentLocation.Y]->LastMove;
		BlueprintGridCells.RemoveAt(--MovementCount);
		CurrentLocation.X = fmod(CurrentLocation.X - LastMove.X + CHESSBOARD_SIZE, CHESSBOARD_SIZE);
		CurrentLocation.Y = fmod(CurrentLocation.Y - LastMove.Y + CHESSBOARD_SIZE, CHESSBOARD_SIZE);
	}

	return false;
}

int AHorseGameMode::CalculateDegree(FVector2D Location)
{
	int32 Degree = 0;
	for (int i = 0; i < 8; ++i)
	{
		int32 Nx = Location.X + dx[i];
		int32 Ny = Location.Y + dy[i];
		if (Nx >= 0 && Nx < CHESSBOARD_SIZE && Ny >= 0 && Ny < CHESSBOARD_SIZE && !GridCells[Nx][Ny]->
			GetIsVisitedDuringCal())
		{
			GridCells[Nx][Ny]->LastMove = FVector2D(dx[i], dy[i]);
			Degree++;
		}
	}
	return Degree;
}

void AHorseGameMode::SortCandidates(TArray<AGridCell*>& Candidates)
{
	for (int i = 0; i < Candidates.Num() - 1; i++)
	{
		for (int j = 0; j < Candidates.Num() - i - 1; j++)
		{
			const int32 DegreeA = CalculateDegree(Candidates[j]->LogicLocation);
			const int32 DegreeB = CalculateDegree(Candidates[j + 1]->LogicLocation);
			if (DegreeA > DegreeB)
			{
				auto temp = Candidates[j];
				Candidates[j] = Candidates[j + 1];
				Candidates[j + 1] = temp;
			}
		}
	}
}

bool AHorseGameMode::CanReturnToStart(FVector2D Location)
{
	for (int i = 0; i < 8; ++i)
	{
		int Nx = Location.X + dx[i];
		int Ny = Location.Y + dy[i];
		if (Nx == StartLocation.X && Ny == StartLocation.Y)
		{
			return true;
		}
	}
	return false;
}
