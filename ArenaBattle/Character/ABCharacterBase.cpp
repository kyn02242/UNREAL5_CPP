// Fill out your copyright notice in the Description page of Project Settings.





#include "Character/ABCharacterBase.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"



// Sets default values

AABCharacterBase::AABCharacterBase()

{

	// Pawn

	bUseControllerRotationPitch = false;

	bUseControllerRotationYaw = false;

	bUseControllerRotationRoll = false;



	// Capsule

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));



	// Movement

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;

	GetCharacterMovement()->AirControl = 0.35f;

	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;

	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;



	// Mesh

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));



	//메쉬 불러오기(분수대와 마찬가지로 오브젝트로 불러온다)

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));

	if (CharacterMeshRef.Object)

	{

		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);

	}

	//애니메이션 불러오기(클래스로 불러와야함)

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));

	if (AnimInstanceClassRef.Class)

	{

		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);

	}

}





