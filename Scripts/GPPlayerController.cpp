// Fill out your copyright notice in the Description page of Project Settings.

#include "GPPlayerController.h"

#include <string>

#include "GunInput.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AGPPlayerController::AGPPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	playerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	playerCameraComponent->SetupAttachment(GetCapsuleComponent());
	playerCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	playerCameraComponent->bUsePawnControlRotation = true;
	
	GunInputController = CreateDefaultSubobject<UGunInput>(TEXT("GunInput"));

	defaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	defaultGravityScale = GetCharacterMovement()->GravityScale;
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(playerCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	
	//GunInputs
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);
}

void AGPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void AGPPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FString a = FString::SanitizeFloat(GetCharacterMovement()->GravityScale);
	
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, a);	
	
	//Set gravity scale to default when the player is on the Ground
	if(GetCharacterMovement()->IsMovingOnGround())
	{
		GetCharacterMovement()->GravityScale = defaultGravityScale;
	}

	//if floating set gravity to FLOATING
	if(isFloating && GetCharacterMovement()->Velocity.Z < 0)
	{
		GetCharacterMovement()->GravityScale = FloatingGravityScale;
	}

	//If no longer floating set gravity to FALLING.
	if(!isFloating && GetCharacterMovement()->Velocity.Z < 0)
	{
		GetCharacterMovement()->GravityScale = FallGravityScale;
	}
}

void AGPPlayerController::IncreaseGravityOnFall(float fallGravityVal)
{
	if(!isFloating && GetCharacterMovement()->Velocity.Z < 0)
	{
		GetCharacterMovement()->GravityScale = fallGravityVal;	
	}
}

void AGPPlayerController::MoveForward(float moveValue)
{
	AddMovementInput(GetActorForwardVector(), moveValue, true);
}

void AGPPlayerController::MoveRight(float moveValue)
{
	AddMovementInput(GetActorRightVector(), moveValue, true);
}

void AGPPlayerController::OnRunStart()
{
	if(!CanRun)
		return;
	
	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
}

void AGPPlayerController::OnRunStop()
{
	if(!CanRun)
		return;
	
	GetCharacterMovement()->MaxWalkSpeed = defaultWalkSpeed;
}

void AGPPlayerController::OnStartJump()
{
	OnJumpFloating();
	isFloating = true;
	bPressedJump = true;
}

void AGPPlayerController::OnStopJump()
{
	isFloating = false;
	bPressedJump = false;
}

void AGPPlayerController::OnJumpFloating()
{
	if(!CanFloat || GetCharacterMovement()->Velocity.Z > 0)
		return;

	//isFloating = true;
	//GetCharacterMovement()->GravityScale = FloatingGravityScale;
}

void AGPPlayerController::StartGunFire()
{
	GunInputController->StartFire();
}

void AGPPlayerController::StopGunFire()
{
	GunInputController->StopFire();
}

//dashing isnt done!!!
void AGPPlayerController::DashForward()
{
	UE_LOG(LogTemp, Warning, TEXT("Forward DASH"));
	GetCharacterMovement()->AddImpulse(GetCharacterMovement()->Velocity * 100, true);
}

void AGPPlayerController::DashBack()
{
	UE_LOG(LogTemp, Warning, TEXT("Back DASH"));
	GetCharacterMovement()->AddImpulse(-GetActorForwardVector() * 10000, true);
}

void AGPPlayerController::DashRight()
{
	UE_LOG(LogTemp, Warning, TEXT("Right DASH"));
	GetCharacterMovement()->AddImpulse(GetActorRightVector() * 10000, true);
}

void AGPPlayerController::DashLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("Left DASH"));
	GetCharacterMovement()->AddImpulse(-GetActorRightVector() * 10000, true);
}

void AGPPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	
	//movements
	PlayerInputComponent->BindAxis("GPMoveForward", this, &AGPPlayerController::MoveForward);
	PlayerInputComponent->BindAxis("GPMoveRight", this, &AGPPlayerController::MoveRight);
	
	//Running
	PlayerInputComponent->BindAction("GPRun", IE_Pressed, this, &AGPPlayerController::OnRunStart);
	PlayerInputComponent->BindAction("GPRun", IE_Released, this, &AGPPlayerController::OnRunStop);

	//Dashing
	PlayerInputComponent->BindAction("GPDashForward", IE_Pressed, this, &AGPPlayerController::DashForward);
	PlayerInputComponent->BindAction("GPDashBack", IE_Pressed, this, &AGPPlayerController::DashBack);
	PlayerInputComponent->BindAction("GPDashRight", IE_Pressed, this, &AGPPlayerController::DashRight);
	PlayerInputComponent->BindAction("GPDashLeft", IE_Pressed, this, &AGPPlayerController::DashLeft);
	
	//Rotation (Camera for pitch)
	PlayerInputComponent->BindAxis("GPLookUp", this, &AGPPlayerController::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("GPTurnRight", this, &AGPPlayerController::AddControllerYawInput);

	//Jumping
	PlayerInputComponent->BindAction("GPJump", IE_Pressed, this, &AGPPlayerController::OnStartJump);
	PlayerInputComponent->BindAction("GPJump", IE_Released, this, &AGPPlayerController::OnStopJump);
	//PlayerInputComponent->BindAction("GPJump", IE_Repeat, this, &AGPPlayerController::OnJumpFloating);

	//fire
	PlayerInputComponent->BindAction("GPFire", IE_Pressed, this, &AGPPlayerController::StartGunFire);
	PlayerInputComponent->BindAction("GPFire", IE_Released, this, &AGPPlayerController::StopGunFire);
}

