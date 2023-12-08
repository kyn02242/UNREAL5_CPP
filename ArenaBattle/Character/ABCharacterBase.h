// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	//설정의 변환을 위한 열거형
	Shoulder,
	Quater
};

UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter
{
	GENERATED_BODY()

	public:
	// Sets default values for this character's properties
	AABCharacterBase();

protected:
	//CharacterControlData 데이터에셋을 입력으로 받는 SetCharacterControlData
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	//두가지 에셋 오브젝트를 바로 받아올 수 있도록 Map으로 설정
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;


protected:
	//콤보액션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;

	void ProcessComboCommand();

	//콤보 액션의 시작
	void ComboActionBegin();

	//콤보 액션의 종료
	//몽타주에 설정된 델리게이트를 통해 바로 호출될 수 있도록 파라미터를 맞춘다.
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	//타이머 발동
	void SetComboCheckTimer();

	//콤보 입력이 들어왔는지 아닌지 체크
	void ComboCheck();

	//현재 콤보가 어디까지 진행되었는가?
	//내부적으로만 사용할것이므로 UPROPERTY 사용 안함
	//0 : 시작하지 않음
	//1~4 : n번째 콤보 진행중
	int32 CurrentCombo = 0;

	//콤보를 위한 타이머 설정
	//내부적인 로직에만 필요하므로 UPROPERTY 선언 안함
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

};