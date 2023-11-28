// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	//������ ��ȯ�� ���� ������
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
	//CharacterControlData �����Ϳ����� �Է����� �޴� SetCharacterControlData
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	//�ΰ��� ���� ������Ʈ�� �ٷ� �޾ƿ� �� �ֵ��� Map���� ����
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;
};