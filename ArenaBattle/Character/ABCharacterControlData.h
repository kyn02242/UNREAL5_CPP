// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABCharacterControlData.generated.h"

/**
 *
 */
UCLASS()

class ARENABATTLE_API UABCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:
	UABCharacterControlData();

	//Pawn에서 Yaw값을 주로 사용하기 떄문에 해당값 체크 여부를 결정하는 변수
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;

	//Character Movement에서 사용되는 변수들
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bOrientRotationToMovement : 1;
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	//입력매핑 컨텍스트를 담을 멤버변수 선언
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	//SpringArm에 관한 여러 변수
	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;
};