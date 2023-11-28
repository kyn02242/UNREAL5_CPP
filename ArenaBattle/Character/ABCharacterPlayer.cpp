// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ABCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ABCharacterControlData.h"

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
	//Jump는 변화없음
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	//컨트롤 전환
	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	//숄더뷰(Move,Look)
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	//쿼터뷰(Move)
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	//현재 어떤 컨트롤을 사용하고 있는지 확인용(Default는 쿼터뷰)
	CurrentCharacterControlType = ECharacterControlType::Quater;
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);

	//// 만약 내가 설정한 EnhancedInput을 사용하지 않을 경우, 에러(CastChecked)
	//// 반드시 사용하도록
	//APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//{
	//	// 추가
	//	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	//	// 제거(필요시 사용)
	//	// Subsystem->RemoveMappingContext(DefaultMappingContext);
	//}
}

void AABCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Jump와 StopJumping을 직접 바인딩
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	//컨트롤 전환 함수에 바인딩
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ChangeCharacterControl);

	// 우리가 구현할 Move와 Look함수에 바인딩
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuaterMove);
}

void AABCharacterPlayer::ChangeCharacterControl()
{
	//쿼터뷰였을 경우 숄더뷰로
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	//숄더뷰였을 경우 쿼터뷰로
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

//실제 컨트롤의 전환
void AABCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	//상위 클래스의 컨트롤 매니저에 있는 컨트롤 데이터 에셋 가져오기
	UABCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	//컨트롤 데이터가 있는지 확인
	check(NewCharacterControl);

	//기본 속성 호출
	SetCharacterControlData(NewCharacterControl);

	//BeginPlay에 있던 함수들
	//반드시 가져올 수 았어야 하므로 CastChecked
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

	//향상된 인풋 시스템에서 서브시스템 가져오기
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//기존의 모든 매핑 컨텍스트 제거
		Subsystem->ClearAllMappings();

		//컨트롤 데이터 에셋에 선언된 인풋 매핑 컨텍스트 추가
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
	//컨트롤 타입 전환이 완료되었으므로
	CurrentCharacterControlType = NewCharacterControlType;
}

void AABCharacterPlayer::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

//ShoulderMove
void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	//컨트롤러로부터 ControllRotation을 받아와 X와 Y이동을 설정
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}
//ShoulderLook
void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	//컨트롤러로부터 ControllRotation을 받아와 Yaw와 Pitch를 설정
	//F12로 Yaw와 Pitch를 받아오는 과정 확인해보자
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

//QuaterMove
void AABCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	//컨트롤러로부터 ControllRotation을 받아와 X와 Y이동을 설정
	FVector2D MovementVector = Value.Get<FVector2D>();

	//MovementVector받아오기
	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	//크기가 1이 되도록 조정
	float MovementVectorSize = 1.0f;
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	//X와 Y를 모디파이어를 통해 조절
	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);

	//현재 ControlRotation을 정면으로
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());

	//Movement Component에서 설정한 옵션에 따라 캐릭터 회전
	AddMovementInput(MoveDirection, MovementVectorSize);
}
