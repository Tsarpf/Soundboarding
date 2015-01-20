// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundBoarding.h"
#include "MySoundWaveStreaming.h"



//DECLARE_CYCLE_STAT(TEXT("Sound Mod Generate Data"), STAT_SoundModGeneratePCMData, STATGROUP_Audio);

UMySoundWaveStreaming::UMySoundWaveStreaming(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP), Time(0.0f), Frequency(100.0f)
{
	SampleRate = 44100;
	NumChannels = 2;
	Duration = INDEFINITELY_LOOPING_DURATION;
	bProcedural = true;
	bLooping = false;
	DeltaTime = 1.0f / SampleRate;
}

int32 UMySoundWaveStreaming::GeneratePCMData(uint8* PCMData, const int32 SamplesNeeded)
{
	float TimeStart = Time;

	Omega = 2.0f * PI * Frequency;	// angular frequency [rad/s]

	for (int i = 0; i < SamplesNeeded; i++)
	{
		Time = TimeStart + i * DeltaTime;
		int32 a = FMath::RoundToInt(65535.0f * (1.0f + FMath::Sin(Omega*Time)) * 0.5f);
		if (a > 65535)
		{
			a = 65535;
		}
		else if (a < 0)
		{
			a = 0;
		}
		uint16 ua = (uint16)a;
		PCMData[2 * i] = ua;			// Low Byte
		PCMData[2 * i + 1] = ua >> 8;	// High Byte
	}

	//Time = Time + SamplesNeeded * DeltaTime; // INCORRECT LINE
	Time = TimeStart + SamplesNeeded * DeltaTime; // CORRECTED LINE

	int32 BytesProvided = SamplesNeeded * 2;
	return BytesProvided;
}


/*

#include "AudioTestProject.h"
#include "SoundWaveProceduralTest.h"

USoundWaveProceduralTest::USoundWaveProceduralTest(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
, Time(0.0f)
, Frequency(100.0f)
{
SampleRate = 44100;
NumChannels = 1;
Duration = INDEFINITELY_LOOPING_DURATION;
bProcedural = true;
bLooping = false;
DeltaTime = 1.0f / SampleRate;    // time increment between samples [s]
}

int32 USoundWaveProceduralTest::GeneratePCMData(uint8* PCMData, const int32 SamplesNeeded)
{
float TimeStart = Time;

Omega = 2.0f * PI * Frequency;	// angular frequency [rad/s]

for(int i = 0; i < SamplesNeeded; i++)
{
Time = TimeStart + i * DeltaTime;
int32 a = FMath::RoundToInt( 65535.0f * (1.0f + FMath::Sin(Omega*Time)) * 0.5f);
if(a > 65535)
{
a = 65535;
}
else if( a < 0)
{
a = 0;
}
uint16 ua = (uint16)a;
PCMData[2*i] = ua;			// Low Byte
PCMData[2*i+1] = ua >> 8;	// High Byte
}

Time = Time + SamplesNeeded * DeltaTime;

int32 BytesProvided = SamplesNeeded * 2;
return BytesProvided;
}

SIZE_T USoundWaveProceduralTest::GetResourceSize(EResourceSizeMode::Type Mode)
{
return 0;
}

int32 USoundWaveProceduralTest::GetResourceSizeForFormat(FName Format)
{
return 0;
}

void USoundWaveProceduralTest::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
// should never be in asset registry
check(false);
}

FByteBulkData* USoundWaveProceduralTest::GetCompressedData(FName Format)
{
return NULL;
}

void USoundWaveProceduralTest::Serialize(FArchive& Ar)
{
// do not call the USoundWave version of serialize
USoundBase::Serialize(Ar);
}

void USoundWaveProceduralTest::InitAudioResource(FByteBulkData& CompressedData)
{
// should never be pushing compressed data
check(false);
}

bool USoundWaveProceduralTest::InitAudioResource(FName Format)
{
//nothing to be done to initialize
return true;
}
// EndFile:SoundWaveProceduralTest.cpp

*/