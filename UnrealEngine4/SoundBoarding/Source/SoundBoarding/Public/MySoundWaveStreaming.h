// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound/SoundWaveStreaming.h"
#include "MySoundWaveStreaming.generated.h"

/**
 * 
 */
UCLASS()
class SOUNDBOARDING_API UMySoundWaveStreaming : public USoundWaveStreaming
{
	GENERATED_BODY()
	

	//UPROPERTY()
	//class USoundMod* SoundMod;
	//class UMySoundNode* SoundMod;

	// Begin USoundWave interface.
	virtual int32 GeneratePCMData(uint8* PCMData, const int32 SamplesNeeded) override;
	// End USoundWave interface.
	
};
