// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABFountain.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AABFountain::AABFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//����� ������ ��üȭ
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));

	//��Ʈ ������Ʈ ���� �ʼ�
	RootComponent = Body;
	//Water�� Body�� �ڽ����� ���� �ϹǷ�,
	Water->SetupAttachment(Body);
	//���� Ʈ������(Body���� �����ġ)����(0,0,132)
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));

	//������ �����Ǿ����Ƿ� �� ���ο� ����ƽ �޽��� ���� �������ִ� ����

	//BodyMeshRef�� �޽��� �ҷ�����
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));

	if (BodyMeshRef.Object)//BodyMeshRef�� Object�� �� ���������ٸ�
	{
		//�ش� �Ž��� Body������Ʈ�� ����
		Body->SetStaticMesh(BodyMeshRef.Object);
	}

	//WaterMeshRef�� �޽��� �ҷ�����
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));

	if (WaterMeshRef.Object)//WaterMeshRef�� Object�� �� ���������ٸ�
	{
		//�ش� �Ž��� Water������Ʈ�� ����
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

