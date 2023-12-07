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
	//상속을 받았기 때문에 Super
	Super::NativeInitializeAnimation();

	//GetOwningActor를 통해 Actor정보를 얻어올 수 있는데, 이것이 Character인지 알 수 없기 때문에 Character로 형변환
	Owner = Cast<ACharacter>(GetOwningActor());
	
	//형변환이 성공적으로 진행되었다면
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//상속을 받았기 떄문에 Super
	Super::NativeUpdateAnimation(DeltaSeconds);

	//각각의 값변수 지정
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
}
