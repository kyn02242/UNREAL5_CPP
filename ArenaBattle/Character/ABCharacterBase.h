// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	//������ ��ȯ�� ���� ������
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
	//CharacterControlData �����Ϳ����� �Է����� �޴� SetCharacterControlData
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	//�ΰ��� ���� ������Ʈ�� �ٷ� �޾ƿ� �� �ֵ��� Map���� ����
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;


protected:
	//�޺��׼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;

	void ProcessComboCommand();

	//�޺� �׼��� ����
	void ComboActionBegin();

	//�޺� �׼��� ����
	//��Ÿ�ֿ� ������ ��������Ʈ�� ���� �ٷ� ȣ��� �� �ֵ��� �Ķ���͸� �����.
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	//Ÿ�̸� �ߵ�
	void SetComboCheckTimer();

	//�޺� �Է��� ���Դ��� �ƴ��� üũ
	void ComboCheck();

	//���� �޺��� ������ ����Ǿ��°�?
	//���������θ� ����Ұ��̹Ƿ� UPROPERTY ��� ����
	//0 : �������� ����
	//1~4 : n��° �޺� ������
	int32 CurrentCombo = 0;

	//�޺��� ���� Ÿ�̸� ����
	//�������� �������� �ʿ��ϹǷ� UPROPERTY ���� ����
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

};