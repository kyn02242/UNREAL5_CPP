// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/ABCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ABCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "ABComboActionData.h"

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

    // �޽� �ҷ�����(�м���� ���������� ������Ʈ�� �ҷ��´�)
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
    }

    // �ִϸ��̼� �ҷ�����(Ŭ������ �ҷ��;���)
    //AnimBluprint Ŭ���� ������ ��ȯ�Ұ��� �ƴϱ� ������ ���� ����ǥx
    //Ŭ���� �����̹Ƿ� �������� _C �ٿ��ٰ�
    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }

    //ABC_Shoulder ������ ���¿��� �����͸� �����´�
    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(TEXT("/Script/ArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder'"));
    if (ShoulderDataRef.Object)
    {
        CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
    }

    //ABC_Quater ������ ���¿��� �����͸� �����´�
    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataRef(TEXT("/Script/ArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater'"));
    if (QuaterDataRef.Object)
    {
        CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
    }
}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
    // Pawn�� ���õ� ����
    bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

    // CharacterMovement�� ���õ� ����
    GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
    GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
    GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AABCharacterBase::ProcessComboCommand()
{
    if (CurrentCombo == 0)
    {
        ComboActionBegin();
        return;
    }
    //�� �̻� ������ �ʿ䰡 ���ų�, �Է� Ÿ�̹��� ��ģ ���
    if (!ComboTimerHandle.IsValid())
    {
        HasNextComboCommand = false;
    }
    //Ÿ�̸Ӱ� ��ȿ�� ��� : �Է��� �� Ÿ�ֿ̹� ���� ���
    else
    {
        HasNextComboCommand = true;
    }
}

void AABCharacterBase::ComboActionBegin()
{
    //�޺� ����
    CurrentCombo = 1;

    //Movement ����
    //Movement ���ش�
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    //�ִϸ��̼� ����
    //�ִϸ��̼� �ӵ� ����
    const float AttackSpeedRate = 1.0f;
    //��Ÿ�� ����� ���� AnimInstance�� ������ ��������
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    //��Ÿ�� ���
    AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

    //��Ÿ�� ����� ������, ComboActionEnd ����
    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

    //�޺� �����ҋ� Ÿ�̸� ����
    ComboTimerHandle.Invalidate();
    SetComboCheckTimer();
}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
    //Current Combo�� �޺��� ������ ���� 0�� �� ����
    //0�̸� ���� �޼���
    ensure(CurrentCombo != 0);

    //CurrentCombo�� 0���� �����
    CurrentCombo = 0;
    //�������ִ� Movement �ǵ�����
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::SetComboCheckTimer()
{
    int32 ComboIndex = CurrentCombo - 1;
    ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

    const float AttackSpeedRate = 1.0f;
    float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
    if (ComboEffectiveTime > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectiveTime, false);
    }
}

void AABCharacterBase::ComboCheck()
{
    ComboTimerHandle.Invalidate();
    //���� �Է��� ���ͼ� HasNextComboCommand�� True��� �������� ����
    if (HasNextComboCommand)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        //CurrentCombo�� �ϳ� ������Ű��, MaxComboCount�� ���� �ʵ��� Ŭ����
        CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
        //���� �������� �̸� ����
        FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
        //���� �������� ����
        AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
        SetComboCheckTimer();
        //�Է°� �ʱ�ȭ
        HasNextComboCommand = false;
    }
}
