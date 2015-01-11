// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundBoarding.h"
#include "MySoundWaveStreaming.h"



//DECLARE_CYCLE_STAT(TEXT("Sound Mod Generate Data"), STAT_SoundModGeneratePCMData, STATGROUP_Audio);

int32 UMySoundWaveStreaming::GeneratePCMData(uint8* PCMData, const int32 SamplesNeeded)
{
	//int32 BytesGenerated = Super::GeneratePCMData(PCMData, SamplesNeeded);
	/*
	int32 BytesDesired = (SamplesNeeded * sizeof(int16)) - BytesGenerated;

	if (BytesDesired > 0)
	{
	//get more data, blocking?
	}
	*/

	return 0;
	//return BytesGenerated;
}