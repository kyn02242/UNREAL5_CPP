// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	//설정의 변환을 위한 열거형
	Shoulder,
	Quater
};

UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter
{
	GENERATED_BODY()

	public:
	// Sets default values for this character's properties
	AABCharacterBase();

protected:
	//CharacterControlData 데이터에셋을 입력으로 받는 SetCharacterControlData
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	//두가지 에셋 오브젝트를 바로 받아올 수 있도록 Map으로 설정
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;
};