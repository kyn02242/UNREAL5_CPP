// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ABAnimInstance.h"
#include "GameFrameWork/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UABAnimInstance::UABAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UABAnimInstance::NativeInitializeAnimation()
{
	//����� �޾ұ� ������ Super
	Super::NativeInitializeAnimation();

	//GetOwningActor�� ���� Actor������ ���� �� �ִµ�, �̰��� Character���� �� �� ���� ������ Character�� ����ȯ
	Owner = Cast<ACharacter>(GetOwningActor());
	
	//����ȯ�� ���������� ����Ǿ��ٸ�
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//����� �޾ұ� ������ Super
	Super::NativeUpdateAnimation(DeltaSeconds);

	//������ ������ ����
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
}
