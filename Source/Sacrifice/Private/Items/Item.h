// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UNiagaraComponent;
class UCapsuleComponent;

UCLASS()
class AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Rotate(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* ItemParticles;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RotationRate = 15.f;

	// 0 for Artifact, 1 for Anti Mind Control
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 ItemType = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE int32 GetItemType() { return ItemType; }

};
