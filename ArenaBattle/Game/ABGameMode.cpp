// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include "ABGameMode.h"
//#include "Player/ABPlayerController.h"


AABGameMode::AABGameMode()
{
	//Default Pawn Class 설정
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	//기본 제공되는 ThirdPersonBP의 Pawn의 에셋정보를 가져오는 과정
	//끝의 _C는 클래스의 정보를 가져오기 위해서
	// 
	//if (ThirdPersonClassRef.Class)//잘 가져왔다면
	//{
	//	DefaultPawnClass = ThirdPersonClassRef.Class;
	//}

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/ArenaBattle.ABCharacterPlayer"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	//PlayerControllerClass를 include 없이 연결하는 과정(위의 에셋정보 가져오는 과정과 흡사함)
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArenaBattle.ABPlayerController"));//기본적으로 Class정보이므로 _C 필요없음
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	////Player Controller Class 설정
	//PlayerControllerClass = AABPlayerController::StaticClass();
	//클래스를 설정 가능하도록
}
