// Fill out your copyright notice in the Description page of Project Settings.



#include "Character/ABCharacterPlayer.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"

#include "InputMappingContext.h"

#include "EnhancedInputComponent.h"

#include "EnhancedInputSubsystems.h"



AABCharacterPlayer::AABCharacterPlayer()

{

	// Camera

	// USpringArm의 경우, 루트컴포넌트에 부착하고, 4m의 길이를 가진도록

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 400.0f;

	CameraBoom->bUsePawnControlRotation = true;



	// UCamera의 경우, USpringArm에 부착

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	FollowCamera->bUsePawnControlRotation = false;



	// Input

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ArenaBattle/Input/IMC_Default.IMC_Default'"));

	if (nullptr != InputMappingContextRef.Object)

	{

		DefaultMappingContext = InputMappingContextRef.Object;

	}



	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Move.IA_Move'"));

	if (nullptr != InputActionMoveRef.Object)

	{

		MoveAction = InputActionMoveRef.Object;

	}



	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));

	if (nullptr != InputActionJumpRef.Object)

	{

		JumpAction = InputActionJumpRef.Object;

	}



	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Look.IA_Look'"));

	if (nullptr != InputActionLookRef.Object)

	{

		LookAction = InputActionLookRef.Object;

	}

}



void AABCharacterPlayer::BeginPlay()

{

	Super::BeginPlay();



	// 만약 내가 설정한 EnhancedInput을 사용하지 않을 경우, 에러(CastChecked)

	// 반드시 사용하도록

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))

	{

		// 추가

		Subsystem->AddMappingContext(DefaultMappingContext, 0);

		// 제거(필요시 사용)

		// Subsystem->RemoveMappingContext(DefaultMappingContext);

	}

}



void AABCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)

{

	Super::SetupPlayerInputComponent(PlayerInputComponent);



	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);



	// Jump와 StopJumping을 직접 바인딩

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	// 우리가 구현할 Move와 Look함수에 바인딩

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Move);

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Look);

}



void AABCharacterPlayer::Move(const FInputActionValue& Value)

{

	FVector2D MovementVector = Value.Get<FVector2D>();



	const FRotator Rotation = Controller->GetControlRotation();

	const FRotator YawRotation(0, Rotation.Yaw, 0);



	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);



	AddMovementInput(ForwardDirection, MovementVector.X);

	AddMovementInput(RightDirection, MovementVector.Y);

}



void AABCharacterPlayer::Look(const FInputActionValue& Value)

{

	FVector2D LookAxisVector = Value.Get<FVector2D>();



	AddControllerYawInput(LookAxisVector.X);

	AddControllerPitchInput(LookAxisVector.Y);

}