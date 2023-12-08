// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UABComboActionData();

	//몽타주 섹션 이름
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	//총 콤보의 개수
	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;
	
	//프레임의 기준 재생속도
	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;
	
	//사전에 입력되었는지 확인하는 프레임
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
	
};
