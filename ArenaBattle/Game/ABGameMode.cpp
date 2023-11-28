// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include "ABGameMode.h"
//#include "Player/ABPlayerController.h"


AABGameMode::AABGameMode()
{
	//Default Pawn Class ����
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	//�⺻ �����Ǵ� ThirdPersonBP�� Pawn�� ���������� �������� ����
	//���� _C�� Ŭ������ ������ �������� ���ؼ�
	// 
	//if (ThirdPersonClassRef.Class)//�� �����Դٸ�
	//{
	//	DefaultPawnClass = ThirdPersonClassRef.Class;
	//}

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/ArenaBattle.ABCharacterPlayer"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	//PlayerControllerClass�� include ���� �����ϴ� ����(���� �������� �������� ������ �����)
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArenaBattle.ABPlayerController"));//�⺻������ Class�����̹Ƿ� _C �ʿ����
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	////Player Controller Class ����
	//PlayerControllerClass = AABPlayerController::StaticClass();
	//Ŭ������ ���� �����ϵ���
}
