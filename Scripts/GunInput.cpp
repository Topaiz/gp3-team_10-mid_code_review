#include "GunInput.h"

#include "Components/SceneComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GPPlayerController.h"
#include "GP_EnemyPrototype.h"
#include "Camera/CameraComponent.h"


class AGP_EnemyPrototype;



UGunInput::UGunInput()
{

	//FP_Gun->SetupAttachment(RootComponent);


	//FP_Gun->SetupAttachment(Owner->GetRootComponent());

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	//FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);
	
}
void UGunInput::BeginPlay()
{
	//FP_Gun->AttachToComponent(Owner->Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	
	Owner =Cast <AGPPlayerController> (GetOwner()) ;

	Owner->GetWorldTimerManager().SetTimer(TimerHandle, this, &UGunInput::FireShot, TimeBetweenShots, true);
	Owner->GetWorldTimerManager().PauseTimer(TimerHandle);
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));


}

void UGunInput::StartFire()
{
	if (Autofire)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("Health: %f"), GetWorldTimerManager().GetTimerElapsed(TimerHandle_HandleRefire)));
		if (Owner->GetGameTimeSinceCreation() - LastShotTime > TimeBetweenShots)
		{
			FireShot();
			Owner->GetWorldTimerManager().UnPauseTimer(TimerHandle);
		}
		else
		{
			float remainingtime = Owner->GetWorldTimerManager().GetTimerRemaining(TimerHandle);
			Owner->GetWorldTimerManager().SetTimer(TimerHandle, this, &UGunInput::FireShot, TimeBetweenShots, true, remainingtime);
		}
	}
	else
	{
		FireShot();
	}
}

void UGunInput::StopFire()
{

	if (Autofire)
	{
		Owner->GetWorldTimerManager().SetTimer(TimerHandle, this, &UGunInput::FireShot, TimeBetweenShots, true);
		Owner->GetWorldTimerManager().PauseTimer(TimerHandle);
	}
}

void UGunInput::FireShot()
{
	UE_LOG(LogTemp, Warning, TEXT("were are shooting"));
	
	if (Autofire)
	{
		LastShotTime = Owner->GetGameTimeSinceCreation();
	}
	
	FHitResult Hit;
	const float WeaponRange = 200000.f;
	const FVector StartTrace = Owner->playerCameraComponent->GetComponentLocation();
	const FVector EndTrace = (Owner->playerCameraComponent->GetForwardVector() * WeaponRange) + StartTrace;
	//Set Spawn Collision Handling Override
	//FActorSpawnParameters ActorSpawnParams;
	//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, Owner);
	//
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
	{
		if (ImpactParticles != nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint));
		}

		if (AGP_EnemyPrototype* Enemy = Cast<AGP_EnemyPrototype>(Hit.GetActor()))
		{
			Enemy->Damage(10.f);
		}
	}
	
	if (MuzzleParticles)
	{
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticles, FP_Gun->GetSocketTransform(FName("Muzzle")));
	}
	
	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Owner->GetActorLocation());
	}

	if (Owner->FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Owner->Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(Owner->FireAnimation, 1.f);
		}
	}
}

void UGunInput::TimerElapsed()
{
}

void UGunInput::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	//Super::SetupPlayerInputComponent(InputComponent);

	// Bind fire event
	//PlayerInputComponent->BindAction("GPFire", IE_Pressed, this, &UGunInput::StartFire);
	//PlayerInputComponent->BindAction("GPFire", IE_Released, this, &UGunInput::StopFire);
}


