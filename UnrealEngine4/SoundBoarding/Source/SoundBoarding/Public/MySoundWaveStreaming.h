// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound/SoundWaveStreaming.h"
#include "ThreadSafeQueue.h"
#include "MySoundWaveStreaming.generated.h"

/**
 * 
 */
UCLASS()
class SOUNDBOARDING_API UMySoundWaveStreaming : public USoundWaveStreaming
{
	GENERATED_BODY()

public:
	//UMySoundWaveStreaming();
	UMySoundWaveStreaming(const class FPostConstructInitializeProperties&);
	virtual int32 GeneratePCMData(uint8* PCMData, const int32 SamplesNeeded) override;
	// Time of the last sample that was copied to the PCMData buffer [s]
	float Time;

	// Time between two consecutive samples in the audio signal [s]
	float DeltaTime;

	// Angular frequency of the single tone [rad/s]
	float Omega;

	float Frequency;	// [Hz]
	//UPROPERTY()
	//class USoundMod* SoundMod;
	//class UMySoundNode* SoundMod;

	// Begin USoundWave interface.
	// End USoundWave interface.


private:
	ThreadSafeQueue m_queue;
	static void StreamClient(ThreadSafeQueue*);
	
};


/*
#pragma once

#include "Sound/SoundWave.h"
#include "SoundWaveProceduralTest.generated.h"

//////////////////////////////////////////////////////////////////////////
/// USoundWaveProceduralTest :
///		A USoundWave that generates a single tone procedurally. This is
///		A test class that demonstrates the ability to create audio
///		procedurally (e.g. other algorithms could be used to generate
///		wind sounds etc.)
//////////////////////////////////////////////////////////////////////////
UCLASS()
class AUDIOTESTPROJECT_API USoundWaveProceduralTest : public USoundWave
{
GENERATED_UCLASS_BODY()

float Frequency;	// [Hz]

// UObject interface

virtual void Serialize(FArchive& Ar) override;
virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;
virtual SIZE_T GetResourceSize(EResourceSizeMode::Type Mode) override;

// USoundWave interface

virtual int32 GeneratePCMData(uint8* PCMData, const int32 SamplesNeeded) override;
virtual int32 GetResourceSizeForFormat(FName Format) override;
virtual FByteBulkData* GetCompressedData(FName Format) override;
virtual void InitAudioResource(FByteBulkData& CompressedData) override;
virtual bool InitAudioResource(FName Format) override;

private:

// Time of the last sample that was copied to the PCMData buffer [s]
float Time;

// Time between two consecutive samples in the audio signal [s]
float DeltaTime;

// Angular frequency of the single tone [rad/s]
float Omega;
};
*/