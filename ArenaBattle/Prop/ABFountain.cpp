// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABFountain.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AABFountain::AABFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//헤더의 포인터 실체화
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));

	//루트 컴포넌트 지정 필수
	RootComponent = Body;
	//Water는 Body의 자식으로 들어가야 하므로,
	Water->SetupAttachment(Body);
	//로컬 트랜스폼(Body와의 상대위치)설정(0,0,132)
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));

	//구조가 설정되었으므로 이 내부에 스태틱 메쉬를 직접 지정해주는 과정

	//BodyMeshRef로 메쉬를 불러오기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));

	if (BodyMeshRef.Object)//BodyMeshRef의 Object가 잘 가져와졌다면
	{
		//해당 매쉬를 Body오브젝트와 연결
		Body->SetStaticMesh(BodyMeshRef.Object);
	}

	//WaterMeshRef로 메쉬를 불러오기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));

	if (WaterMeshRef.Object)//WaterMeshRef의 Object가 잘 가져와졌다면
	{
		//해당 매쉬를 Water오브젝트와 연결
		Water->SetStaticMesh(WaterMeshRef.Object);
	}


}

// Called when the game starts or when spawned
void AABFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

