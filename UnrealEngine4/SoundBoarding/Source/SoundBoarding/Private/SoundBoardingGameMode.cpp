// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "SoundBoarding.h"
#include "SoundBoardingGameMode.h"
#include "SoundBoardingHUD.h"
#include "SoundBoardingCharacter.h"

ASoundBoardingGameMode::ASoundBoardingGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/MyCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASoundBoardingHUD::StaticClass();
}
