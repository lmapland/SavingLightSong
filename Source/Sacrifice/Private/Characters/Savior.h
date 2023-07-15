// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Savior.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArtifactUpdated, int32, Amount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMindControlUpdated, int32, Amount);

class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
class USphereComponent;

UCLASS()
class ASavior : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASavior();
	
	UFUNCTION()
	virtual void OnItemSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartLevelOne();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CompleteLevelOne();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartLevelTwo();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CompleteLevelTwo();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartLevelThree();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CompleteLevelThree();

	void UpdateArtifactCount(int32 Count);

	void UpdateMindControlCount(int32 Count);
	
	UFUNCTION(BlueprintCallable)
	void IncreaseArtifacts();

	UFUNCTION(BlueprintCallable)
	void IncreaseAMC();
	
	UFUNCTION(BlueprintCallable)
	void ClearItemCounters();

	UPROPERTY(BlueprintAssignable)
	FOnArtifactUpdated OnArtifactUpdatedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnMindControlUpdated OnMindControlUpdatedDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value); // handles forward, back, and side-to-side
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* CharMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* ItemSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	USoundBase* PickupSound;


	APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 ArtifactCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 AntiMindControlCount = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 TotalArtifactCount = 0;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetArtifactCount() const { return ArtifactCount; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetAntiMindControlCount() const { return AntiMindControlCount; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetTotalArtifactCount() const { return TotalArtifactCount; }

};
