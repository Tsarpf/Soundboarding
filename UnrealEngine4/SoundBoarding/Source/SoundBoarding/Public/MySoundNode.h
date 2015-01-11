// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound/SoundNode.h"
#include "MySoundBase.h"
#include "MySoundNode.generated.h"


/**
 * 
 */
UCLASS(hidecategories = Object, editinlinenew, MinimalAPI, meta = (DisplayName = "Mod Player"))
class UMySoundNode : public USoundNode
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditAnywhere, Category = ModPlayer)
		UMySoundBase* SoundMod;
		//USoundBase* SoundMod;

	UPROPERTY(EditAnywhere, Category = ModPlayer)
		uint32 bLooping : 1;

public:
	// Begin USoundNode Interface
	virtual int32 GetMaxChildNodes() const override;
	virtual float GetDuration() override;
	virtual void ParseNodes(FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances) override;
#if WITH_EDITOR
	virtual FString GetTitle() const override;
#endif
	// End USoundNode Interface

};