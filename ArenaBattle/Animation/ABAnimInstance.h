// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UABAnimInstance();

protected:
	//�ʱ�ȭ�� �߻��ϴ� �̺�Ʈ NativeInitializeAnimation
	virtual void NativeInitializeAnimation() override;

	//�����Ӹ��� �߻��ϴ� �̺�Ʈ NativeUpdateAnimation
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//�ִ� �׷����� ������ ����
	//AnimInstance�� �����ϴ� ĳ������ ������ ��� ���� Owner
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	//ĳ���� �����Ʈ ������Ʈ�� ���� ��ü������ Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	//�������̱� ������ EditAnywhere
	//���� ĳ������ �ӵ��� ������ ���� Velocity
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	//�������� �ӵ��� ������ �� �ִ� ���� GroundSpeed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	//Idle ���������� ��Ÿ���� bool�� ���� bIsIdle
	//bool���� ũ�Ⱑ �ָ��ϱ� ������ �𸮾󿡼��� int������ ������ �Ŀ�, ���λ� b�� ���̰�, bitflag�� �������ش�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	//�����̰� �ִ���, ���� �ִ����� ��Ÿ���� MovingThreshould
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	//�������� �ִ���, �ƴ����� Ȯ���� �� �ִ� FallingThreshould
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	//�����ϰ� �ִ����� ��Ÿ���� bIsJumping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	//������������ Ȯ���� �� �ִ� JumpingThreshould
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

};
