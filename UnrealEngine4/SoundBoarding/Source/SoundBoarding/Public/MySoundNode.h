// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound/SoundNode.h"
#include "MySoundWaveStreaming.h"
#include "MySoundNode.generated.h"

/**
 * 
 */
UCLASS()
class UMySoundNode : public USoundNode
{
	GENERATED_UCLASS_BODY()

	// Volume of the sound [0-1]
	UPROPERTY(EditAnywhere, Category = "Sound Source Properties")
	float Volume;

	// Frequency of the test sound [Hz]
	UPROPERTY(EditAnywhere, Category = "Sound Source Properties")
	float Frequency;	// [Hz]

	UMySoundWaveStreaming *SoundWaveProcedural;

	// Begin USoundNode Interface

	virtual int32 GetMaxChildNodes() const override;
	virtual float GetDuration() override;
	virtual void ParseNodes(FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances) override;
	//virtual FString GetUniqueString() const override;
#if WITH_EDITOR
	virtual FString GetTitle() const override;
#endif

protected:

	bool SoundWaveIsInitialized;

	void CreateSoundWaveStreaming();

};


/*

// File: SoundNodeProceduralTest.h
#pragma once

#include "Sound/SoundNode.h"
#include "SoundWaveProceduralTest.h"
#include "SoundNodeProceduralTest.generated.h"

UCLASS()
class AUDIOTESTPROJECT_API USoundNodeProceduralTest : public USoundNode
{
GENERATED_UCLASS_BODY()

// Volume of the sound [0-1]
UPROPERTY(EditAnywhere, Category="Sound Source Properties")
float Volume;

// Frequency of the test sound [Hz]
UPROPERTY(EditAnywhere, Category="Sound Source Properties")
float Frequency;	// [Hz]

USoundWaveProceduralTest *SoundWaveProcedural;

// Begin USoundNode Interface

virtual int32 GetMaxChildNodes() const override;
virtual float GetDuration() override;
virtual void ParseNodes( FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances ) override;
virtual FString GetUniqueString() const override;
#if WITH_EDITOR
virtual FString GetTitle() const override;
#endif

protected:

bool SoundWaveIsInitialized;

void CreateSoundWaveStreaming();
};
// EndFile: SoundNodeProceduralTest.h


*/