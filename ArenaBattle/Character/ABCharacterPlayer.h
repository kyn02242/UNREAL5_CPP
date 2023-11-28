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

	virtual void BeginPlay() override;//�Է� ���� ���ؽ�Ʈ�� �Ҵ��ϴ� ����



public:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;//�𸮾� ������ �Է½ý��ۿ��� �Է¾׼ǰ� ���� ������ Move,Look�� ����



	// Camera Section

protected:

	//ī�޶� �������ִ� ������Ʈ

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))//Meta---�� private�� ����� �𸮾� ������Ʈ ��ü���� �������Ʈ������ ���� �����ϵ��� �ϴ� ������

		TObjectPtr<class USpringArmComponent> CameraBoom;

	//���� ī�޶� ������Ʈ

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))

		TObjectPtr<class UCameraComponent> FollowCamera;



	// Input Section

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))

		TObjectPtr<class UInputMappingContext> DefaultMappingContext;//Ű�����? �е��? ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))

		TObjectPtr<class UInputAction> JumpAction;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))

		TObjectPtr<class UInputAction> MoveAction;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))

		TObjectPtr<class UInputAction> LookAction;



	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

};

