// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundBoarding.h"
#include "MySoundBase.h"

UMySoundBase::UMySoundBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMySoundBase::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	RawData.Serialize(Ar, this);
}

void UMySoundBase::Parse(class FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances)
{
	//FWaveInstance* WaveInstance = ActiveSound.FindWaveInstance(NodeWaveInstanceHash);

	//// Create a new WaveInstance if this SoundWave doesn't already have one associated with it.
	//if (WaveInstance == NULL)
	//{
	//	const int32 SampleRate = 44100;

	//	// Create a new wave instance and associate with the ActiveSound
	//	WaveInstance = new FWaveInstance(&ActiveSound);
	//	WaveInstance->WaveInstanceHash = NodeWaveInstanceHash;
	//	ActiveSound.WaveInstances.Add(NodeWaveInstanceHash, WaveInstance);

	//	// Create streaming wave object
	//	USoundModWave* ModWave = NewObject<USoundModWave>();
	//	ModWave->SampleRate = SampleRate;
	//	ModWave->NumChannels = 2;
	//	ModWave->Duration = INDEFINITELY_LOOPING_DURATION;
	//	ModWave->bLooping = bLooping;

	//	if (ResourceData == NULL)
	//	{
	//		RawData.GetCopy((void**)&ResourceData, true);
	//	}

	//	ModWave->xmpContext = xmp_create_context();
	//	xmp_load_module_from_memory(ModWave->xmpContext, ResourceData, RawData.GetBulkDataSize());
	//	xmp_start_player(ModWave->xmpContext, SampleRate, 0);

	//	WaveInstance->WaveData = ModWave;
	//}

	//WaveInstance->WaveData->Parse(AudioDevice, NodeWaveInstanceHash, ActiveSound, ParseParams, WaveInstances);
}

bool UMySoundBase::IsPlayable() const
{
	return true;
}

float UMySoundBase::GetMaxAudibleDistance()
{
	return (AttenuationSettings ? AttenuationSettings->Attenuation.GetMaxDimension() : WORLD_MAX);
}


