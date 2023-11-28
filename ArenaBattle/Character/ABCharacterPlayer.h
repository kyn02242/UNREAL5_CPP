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

	
protected:
	//Character Control Section
	// 
	//��Ʈ�� ��ȯ�ϴ� �Լ�
	void ChangeCharacterControl();

	//��Ʈ���� ��ȯ�Ǿ�����, ������ ��Ʈ�ѿ� ���õ� ��� ������ �����ϴ� �Լ�
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);

	//ī�޶�� �Է¿� ���õ� �Ӽ����� �������̵� �� �Լ��� �߰�
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData) override;
	
protected:
	// Camera Section
	//ī�޶� �������ִ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))//Meta---�� private�� ����� �𸮾� ������Ʈ ��ü���� �������Ʈ������ ���� �����ϵ��� �ϴ� ������
	TObjectPtr<class USpringArmComponent> CameraBoom;

	//���� ī�޶� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	
protected:
	//Input Section
	// 
	//Jump�� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	//��Ʈ�� ��ȯ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	//����� Move�� Look
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	//���ͺ� Move
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

};

