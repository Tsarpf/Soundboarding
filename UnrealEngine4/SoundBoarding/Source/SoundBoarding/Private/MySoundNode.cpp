// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundBoarding.h"
#include "MySoundNode.h"
#include "ActiveSound.h"

#define LOCTEXT_NAMESPACE "MySoundNode"


UMySoundNode::UMySoundNode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
	, Volume(0.5f)
	, Frequency(100.0f)
{
}

int32 UMySoundNode::GetMaxChildNodes() const
{
	return 0;
}

float UMySoundNode::GetDuration()
{
	return INDEFINITELY_LOOPING_DURATION;
}

void UMySoundNode::ParseNodes(FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances)
{
	// method 1 : works "sort of" , but seems like the clip stops playing and restarts
	if (!SoundWaveIsInitialized)
	{
		CreateSoundWaveStreaming();
	}

	if (SoundWaveIsInitialized)
	{
		if (SoundWaveProcedural)
		{
			SoundWaveProcedural->Frequency = Frequency;
			SoundWaveProcedural->Volume = Volume;
			SoundWaveProcedural->Parse(AudioDevice, NodeWaveInstanceHash, ActiveSound, ParseParams, WaveInstances);
		}
	}
}


void UMySoundNode::CreateSoundWaveStreaming()
{
	SoundWaveProcedural = NewObject<UMySoundWaveStreaming>();
	SoundWaveIsInitialized = true;
}

/*
FString UMySoundNode::GetUniqueString() const
{
	return TEXT("FOO UNIQUE STRING");
}
*/

#if WITH_EDITOR

FString UMySoundNode::GetTitle() const
{
	return TEXT("Procedural Sound Wave Source");
}
#endif


#undef LOCTEXT_NAMESPACE

/*

#include "AudioTestProject.h"
#include "SoundNodeProceduralTest.h"

USoundNodeProceduralTest::USoundNodeProceduralTest(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
, Volume(0.5f)
, Frequency(100.0f)
{
}

int32 USoundNodeProceduralTest::GetMaxChildNodes() const
{
return 0;
}

float USoundNodeProceduralTest::GetDuration()
{
return INDEFINITELY_LOOPING_DURATION;
}

void USoundNodeProceduralTest::ParseNodes(FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances)
{
// method 1 : works "sort of" , but seems like the clip stops playing and restarts
if(!SoundWaveIsInitialized)
{
CreateSoundWaveStreaming();
}

if(SoundWaveIsInitialized)
{
if(SoundWaveProcedural)
{
SoundWaveProcedural->Frequency = Frequency;
SoundWaveProcedural->Volume = Volume;
SoundWaveProcedural->Parse(AudioDevice, NodeWaveInstanceHash, ActiveSound, ParseParams, WaveInstances);
}
}
}


void USoundNodeProceduralTest::CreateSoundWaveStreaming()
{
SoundWaveProcedural = NewObject<USoundWaveProceduralTest>();
SoundWaveIsInitialized = true;
}

FString USoundNodeProceduralTest::GetUniqueString() const
{
return TEXT("FOO UNIQUE STRING");
}

#if WITH_EDITOR

FString USoundNodeProceduralTest::GetTitle() const
{
return TEXT("Procedural Sound Wave Source");
}

*/