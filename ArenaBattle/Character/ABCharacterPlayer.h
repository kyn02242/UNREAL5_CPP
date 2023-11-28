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

	
protected:
	//Character Control Section
	// 
	//컨트롤 전환하는 함수
	void ChangeCharacterControl();

	//컨트롤이 전환되었을때, 실제로 컨트롤에 관련된 모든 설정을 진행하는 함수
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);

	//카메라와 입력에 관련된 속성들을 오버라이드 된 함수에 추가
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData) override;
	
protected:
	// Camera Section
	//카메라를 지탱해주는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))//Meta---는 private로 선언된 언리얼 오브젝트 객체들을 블루프린트에서도 접근 가능하도록 하는 지시자
	TObjectPtr<class USpringArmComponent> CameraBoom;

	//실제 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	
protected:
	//Input Section
	// 
	//Jump는 변동 없음
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	//컨트롤 전환
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	//숄더뷰 Move와 Look
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	//쿼터뷰 Move
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

};

