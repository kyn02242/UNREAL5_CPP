// Fill out your copyright notice in the Description page of Project Settings.



#pragma once



#include "CoreMinimal.h"

#include "Character/ABCharacterBase.h"

#include "InputActionValue.h"

#include "ABCharacterPlayer.generated.h"



/**

 *

 */

UCLASS()

class ARENABATTLE_API AABCharacterPlayer : public AABCharacterBase

{

	GENERATED_BODY()



public:

	AABCharacterPlayer();



protected:

	virtual void BeginPlay() override;//입력 매핑 컨텍스트를 할당하는 역할



public:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;//언리얼 엔진의 입력시스템에서 입력액션과 내가 선언한 Move,Look을 매핑



	// Camera Section

protected:

	//카메라를 지탱해주는 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))//Meta---는 private로 선언된 언리얼 오브젝트 객체들을 블루프린트에서도 접근 가능하도록 하는 지시자

		TObjectPtr<class USpringArmComponent> CameraBoom;

	//실제 카메라 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))

		TObjectPtr<class UCameraComponent> FollowCamera;



	// Input Section

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))

		TObjectPtr<class UInputMappingContext> DefaultMappingContext;//키보드로? 패드로? 설정

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))

		TObjectPtr<class UInputAction> JumpAction;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))

		TObjectPtr<class UInputAction> MoveAction;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))

		TObjectPtr<class UInputAction> LookAction;



	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

};

