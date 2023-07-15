// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetupAttachment(GetRootComponent());

	ItemParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ItemParticles"));
	ItemParticles->SetupAttachment(ItemMesh);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::Rotate(float DeltaTime)
{
	SetActorRotation(GetActorRotation() + FRotator(0.f, RotationRate * DeltaTime, 0.f));
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotate(DeltaTime);
}

