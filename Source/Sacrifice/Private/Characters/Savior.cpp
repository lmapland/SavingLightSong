// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Savior.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASavior::ASavior()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 0;
	CameraBoom->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	ItemSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSphere"));
	ItemSphere->SetupAttachment(GetRootComponent());
	ItemSphere->SetSphereRadius(75.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

void ASavior::OnItemSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AItem* Overlapped = Cast<AItem>(OtherActor);
	if (Overlapped)
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
		}

		int32 ItemType = Overlapped->GetItemType();
		if (ItemType == 0)
		{
			ArtifactCount++;
			TotalArtifactCount++;
			UpdateArtifactCount(ArtifactCount);
			UE_LOG(LogTemp, Warning, TEXT("ArtifactCount: %i"), ArtifactCount);
		}
		else if (ItemType == 1)
		{
			AntiMindControlCount++;
			UpdateMindControlCount(AntiMindControlCount);
			UE_LOG(LogTemp, Warning, TEXT("AntiMindControlCount: %i"), AntiMindControlCount);
		}

		Overlapped->Destroy();
	}
}

void ASavior::UpdateArtifactCount(int32 Count)
{
	OnArtifactUpdatedDelegate.Broadcast(Count);
}

void ASavior::UpdateMindControlCount(int32 Count)
{
	OnMindControlUpdatedDelegate.Broadcast(Count);
}

void ASavior::IncreaseArtifacts()
{
	ArtifactCount++;
	TotalArtifactCount++;
	UpdateArtifactCount(ArtifactCount);
}

void ASavior::IncreaseAMC()
{
	AntiMindControlCount++;
	UpdateMindControlCount(AntiMindControlCount);
}

void ASavior::ClearItemCounters()
{
	ArtifactCount = 0;
	UpdateArtifactCount(ArtifactCount);

	AntiMindControlCount = 0;
	UpdateMindControlCount(AntiMindControlCount);
}

// Called when the game starts or when spawned
void ASavior::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(CharMappingContext, 0);
		}
	}
	
	ItemSphere->OnComponentBeginOverlap.AddDynamic(this, &ASavior::OnItemSphereOverlap);
}

void ASavior::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ASavior::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(LookAxisValue.Y);
}

void ASavior::Jump()
{
	Super::Jump();
}

// Called every frame
void ASavior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASavior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASavior::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASavior::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASavior::Jump);
	}
}

