// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundBoarding.h"
#include "MySoundNode.h"
#include "ActiveSound.h"

#define LOCTEXT_NAMESPACE "MySoundNode"


UMySoundNode::UMySoundNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMySoundNode::ParseNodes(FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances)
{
	if (SoundMod)
	{
		// The SoundWave's bLooping is only for if it is directly referenced, so clear it
		// in the case that it is being played from a player
		bool bModIsLooping = SoundMod->bLooping;
		SoundMod->bLooping = false;

		if (bLooping)
		{
			FSoundParseParameters UpdatedParams = ParseParams;
			UpdatedParams.bLooping = true;
			SoundMod->Parse(AudioDevice, NodeWaveInstanceHash, ActiveSound, UpdatedParams, WaveInstances);
		}
		else
		{
			SoundMod->Parse(AudioDevice, NodeWaveInstanceHash, ActiveSound, ParseParams, WaveInstances);
		}

		SoundMod->bLooping = bModIsLooping;
	}
}

float UMySoundNode::GetDuration()
{
	float Duration = 0.f;
	if (SoundMod)
	{
		if (bLooping)
		{
			Duration = INDEFINITELY_LOOPING_DURATION;
		}
		else
		{
			Duration = SoundMod->Duration;
		}
	}
	return Duration;
}

#if WITH_EDITOR
FString UMySoundNode::GetTitle() const
{
	FText SoundModName;
	if (SoundMod)
	{
		SoundModName = FText::FromString(SoundMod->GetFName().ToString());
	}
	else
	{
		SoundModName = LOCTEXT("NoSoundMod", "NONE");
	}

	FString Title;

	if (bLooping)
	{
		FFormatNamedArguments Arguments;
		Arguments.Add(TEXT("Description"), FText::FromString(Super::GetTitle()));
		Arguments.Add(TEXT("SoundModName"), SoundModName);
		Title = FText::Format(LOCTEXT("LoopingSoundWaveDescription", "Looping {Description} : {SoundModName}"), Arguments).ToString();
	}
	else
	{
		Title = Super::GetTitle() + FString(TEXT(" : ")) + SoundModName.ToString();
	}

	return Title;
}
#endif

// A Mod Player is the end of the chain and has no children
int32 UMySoundNode::GetMaxChildNodes() const
{
	return 0;
}


#undef LOCTEXT_NAMESPACE
