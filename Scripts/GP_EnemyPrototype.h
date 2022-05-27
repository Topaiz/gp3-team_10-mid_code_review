// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "SmartActor.h"
#include "Kismet/GameplayStatics.h"
#include "GP_EnemyPrototype.generated.h"



UCLASS()
class GP3_UPROJECT_API AGP_EnemyPrototype : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGP_EnemyPrototype();

protected:
	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float Damage(float DamageAmount);

	virtual void GoToHeart(AActor* Heart);

	AAIController* AIController;

	UPROPERTY(VisibleAnywhere)
	APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensor;

	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class ASmartActor* SmartActor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	AActor* Heart;


	UPROPERTY(VisibleAnywhere)
	float CurHealth = 0;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 20;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//////////////////////////////////////////////////////////////////////////
	// UPawnSensingComponent Delegates

	UFUNCTION()
	void OnSeePawn(APawn* OtherPawn);

	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);

	virtual void PostInitializeComponents();

	
};