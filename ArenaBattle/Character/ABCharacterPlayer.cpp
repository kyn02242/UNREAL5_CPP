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
	// USpringArm�� ���, ��Ʈ������Ʈ�� �����ϰ�, 4m�� ���̸� ��������
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// UCamera�� ���, USpringArm�� ����
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	//Jump�� ��ȭ����
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	//��Ʈ�� ��ȯ
	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	//�����(Move,Look)
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

	//���ͺ�(Move)
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	//���� � ��Ʈ���� ����ϰ� �ִ��� Ȯ�ο�(Default�� ���ͺ�)
	CurrentCharacterControlType = ECharacterControlType::Quater;
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);

	//// ���� ���� ������ EnhancedInput�� ������� ���� ���, ����(CastChecked)
	//// �ݵ�� ����ϵ���
	//APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//{
	//	// �߰�
	//	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	//	// ����(�ʿ�� ���)
	//	// Subsystem->RemoveMappingContext(DefaultMappingContext);
	//}
}

void AABCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Jump�� StopJumping�� ���� ���ε�
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	//��Ʈ�� ��ȯ �Լ��� ���ε�
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ChangeCharacterControl);

	// �츮�� ������ Move�� Look�Լ��� ���ε�
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuaterMove);
}

void AABCharacterPlayer::ChangeCharacterControl()
{
	//���ͺ俴�� ��� ������
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	//����俴�� ��� ���ͺ��
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

//���� ��Ʈ���� ��ȯ
void AABCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	//���� Ŭ������ ��Ʈ�� �Ŵ����� �ִ� ��Ʈ�� ������ ���� ��������
	UABCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	//��Ʈ�� �����Ͱ� �ִ��� Ȯ��
	check(NewCharacterControl);

	//�⺻ �Ӽ� ȣ��
	SetCharacterControlData(NewCharacterControl);

	//BeginPlay�� �ִ� �Լ���
	//�ݵ�� ������ �� �Ҿ�� �ϹǷ� CastChecked
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

	//���� ��ǲ �ý��ۿ��� ����ý��� ��������
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//������ ��� ���� ���ؽ�Ʈ ����
		Subsystem->ClearAllMappings();

		//��Ʈ�� ������ ���¿� ����� ��ǲ ���� ���ؽ�Ʈ �߰�
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
	//��Ʈ�� Ÿ�� ��ȯ�� �Ϸ�Ǿ����Ƿ�
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
	//��Ʈ�ѷ��κ��� ControllRotation�� �޾ƿ� X�� Y�̵��� ����
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
	//��Ʈ�ѷ��κ��� ControllRotation�� �޾ƿ� Yaw�� Pitch�� ����
	//F12�� Yaw�� Pitch�� �޾ƿ��� ���� Ȯ���غ���
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

//QuaterMove
void AABCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	//��Ʈ�ѷ��κ��� ControllRotation�� �޾ƿ� X�� Y�̵��� ����
	FVector2D MovementVector = Value.Get<FVector2D>();

	//MovementVector�޾ƿ���
	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	//ũ�Ⱑ 1�� �ǵ��� ����
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

	//X�� Y�� ������̾ ���� ����
	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);

	//���� ControlRotation�� ��������
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());

	//Movement Component���� ������ �ɼǿ� ���� ĳ���� ȸ��
	AddMovementInput(MoveDirection, MovementVectorSize);
}
