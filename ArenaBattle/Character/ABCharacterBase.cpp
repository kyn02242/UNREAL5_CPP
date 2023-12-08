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

    // 메쉬 불러오기(분수대와 마찬가지로 오브젝트로 불러온다)
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
    }

    // 애니메이션 불러오기(클래스로 불러와야함)
    //AnimBluprint 클래스 형으로 변환할것이 아니기 때문에 작은 따옴표x
    //클래스 정보이므로 마지막에 _C 붙여줄것
    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }

    //ABC_Shoulder 데이터 에셋에서 데이터를 가져온다
    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(TEXT("/Script/ArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder'"));
    if (ShoulderDataRef.Object)
    {
        CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
    }

    //ABC_Quater 데이터 에셋에서 데이터를 가져온다
    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataRef(TEXT("/Script/ArenaBattle.ABCharacterControlData'/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater'"));
    if (QuaterDataRef.Object)
    {
        CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
    }
}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
    // Pawn에 관련된 설정
    bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

    // CharacterMovement에 관련된 설정
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
    //더 이상 진행할 필요가 없거나, 입력 타이밍을 놓친 경우
    if (!ComboTimerHandle.IsValid())
    {
        HasNextComboCommand = false;
    }
    //타이머가 유효한 경우 : 입력이 제 타이밍에 들어온 경우
    else
    {
        HasNextComboCommand = true;
    }
}

void AABCharacterBase::ComboActionBegin()
{
    //콤보 상태
    CurrentCombo = 1;

    //Movement 지정
    //Movement 없앤다
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    //애니메이션 지정
    //애니메이션 속도 지정
    const float AttackSpeedRate = 1.0f;
    //몽타주 재생을 위해 AnimInstance의 포인터 가져오기
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    //몽타주 재생
    AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

    //몽타주 재생이 끝날때, ComboActionEnd 실행
    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

    //콤보 시작할떄 타이머 시작
    ComboTimerHandle.Invalidate();
    SetComboCheckTimer();
}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
    //Current Combo는 콤보가 끝날때 절대 0일 수 없음
    //0이면 에러 메세지
    ensure(CurrentCombo != 0);

    //CurrentCombo를 0으로 만들기
    CurrentCombo = 0;
    //꺼놓고있던 Movement 되돌리기
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
    //만약 입력이 들어와서 HasNextComboCommand가 True라면 다음으로 진행
    if (HasNextComboCommand)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        //CurrentCombo를 하나 증가시키되, MaxComboCount를 넘지 않도록 클램핑
        CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
        //다음 섹션으로 이름 변경
        FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
        //다음 섹션으로 점프
        AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
        SetComboCheckTimer();
        //입력값 초기화
        HasNextComboCommand = false;
    }
}
