// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectTestGameMode.h"
#include "MyProjectTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyProjectTestGameMode::AMyProjectTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
