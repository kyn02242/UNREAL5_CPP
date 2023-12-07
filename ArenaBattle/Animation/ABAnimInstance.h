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
	//초기화시 발생하는 이벤트 NativeInitializeAnimation
	virtual void NativeInitializeAnimation() override;

	//프레임마다 발생하는 이벤트 NativeUpdateAnimation
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//애님 그래프가 참조할 변수
	//AnimInstance를 소유하는 캐릭터의 정보를 담는 변수 Owner
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	//캐릭터 무브먼트 컴포넌트에 대한 객체포인터 Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	//값변수이기 때문에 EditAnywhere
	//현재 캐릭터의 속도를 보관할 변수 Velocity
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	//땅에서의 속도를 지정할 수 있는 변수 GroundSpeed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	//Idle 상태인지를 나타내는 bool형 변수 bIsIdle
	//bool형은 크기가 애매하기 때문에 언리얼에서는 int형으로 선언한 후에, 접두사 b를 붙이고, bitflag를 설정해준다
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	//움직이고 있는지, 쉬고 있는지를 나타내는 MovingThreshould
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	//떨어지고 있는지, 아닌지를 확인할 수 있는 FallingThreshould
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	//점프하고 있는지를 나타내는 bIsJumping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	//점핑중인지를 확인할 수 있는 JumpingThreshould
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

};
