// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "TBPlayerController.h"
#include "MoveCommand.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController = GWorld->GetFirstPlayerController<ATBPlayerController>())
	{
		PlayerController->GameManager = this;
	}

	if (Levels.IsValidIndex(CurrentLevel))
	{
		CreateLevelActors(Levels[CurrentLevel]);
	}

}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentCommand.IsValid())
	{
		CurrentCommand->Update(DeltaTime);
	}

}

bool AGameManager::UndoLastMove()
{
	if (CommandPool.IsEmpty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void AGameManager::GoBack()
{
	if (!ThePlayer) 
	{ UE_LOG(LogTemp, Error, TEXT("Player can't find")); }	
	if (UndoLastMove()) {
		TSharedPtr<Command> LastMove = CommandPool.Pop();
		TSharedPtr<MoveCommand> LastMoveAsMoveCommand = StaticCastSharedPtr<MoveCommand>(LastMove);
	if (LastMoveAsMoveCommand.IsValid()) 
	{ 
		TSharedRef<MoveCommand> Cmd = MakeShared<MoveCommand>(ThePlayer->Slot->GridPosition, LastMoveAsMoveCommand->Source);			
	   Cmd->Revert();			
	   CurrentCommand = Cmd; }
	}
	else { UE_LOG(LogTemp, Error, TEXT("Its your first move")); }

	
}

void AGameManager::OnActorClicked(AActor* Actor, FKey button)
{
	if (CurrentCommand.IsValid() && CurrentCommand->IsExecuting()) return;

	AGameSlot* Slot = Cast<AGameSlot>(Actor);

	if (!Slot) return;

	if (!ThePlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("No Player Unit Detected"));
		return;

		if (Slot->Unit == nullptr)
		{
			TSharedRef<MoveCommand>Cmd =
				MakeShared<MoveCommand>(ThePlayer->Slot->GridPosition, Slot->GridPosition);
			CommandPool.Add(Cmd);
			Cmd->Execute();
			CurrentCommand = Cmd;
		}
	}
}

void AGameManager::CreateLevelActors(FSLevelInfo& Info)
{
	ThePlayer = nullptr;

	for (auto UnitInfo : Info.Units)
	{
		if(AGameSlot* Slot = GameGrid->GetSlot(UnitInfo.StartPosition))
		{
			Slot->SpawnUnitHere(UnitInfo.UnitClass);
				if (Slot->Unit && Slot->Unit->IsControlledByThePlayer())
				{
					ThePlayer = Slot->Unit;
				}
		}
			
	}
}


