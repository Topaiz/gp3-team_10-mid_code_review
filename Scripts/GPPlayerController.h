// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
//#include "GameFramework/Character.h"
#include "GPPlayerController.generated.h"

class UCameraComponent;
class UCapsuleComponent;
class USceneComponent;
class UFloatingPawnMovement;
class UGunInput;

UCLASS()
class GP3_UPROJECT_API AGPPlayerController : public ACharacter
{
	GENERATED_BODY()

public:
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* playerCameraComponent;

	//movement
	UPROPERTY(EditAnywhere, Category = "values")
	float RunningSpeed = 1000;

	UPROPERTY(EditAnywhere, Category = "values")
	float FloatingGravityScale = 0.1f;

	UPROPERTY(EditAnywhere, Category = "values")
	float FallGravityScale = 1.5f;

	//movement options bool
	UPROPERTY(EditAnywhere, Category = "values")
	bool CanRun = true;

	UPROPERTY(EditAnywhere, Category = "values")
	bool CanFloat = true;

	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	//GunInput
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleAnywhere)
	UGunInput* GunInputController;
	
private:
	UPROPERTY()
	float defaultWalkSpeed;

	UPROPERTY()
	float defaultGravityScale;

	UPROPERTY()
	bool isFloating;
	
protected:
	void MoveForward(float moveValue);
	void MoveRight(float moveValue);

	void OnRunStart();
	void OnRunStop();

	UFUNCTION()
	void OnStartJump();
	
	UFUNCTION()
	void OnStopJump();

	void OnJumpFloating();

	void IncreaseGravityOnFall(float fallGravityVal);
	
	void StartGunFire();
	void StopGunFire();

	void DashForward();
	void DashBack();
	void DashRight();
	void DashLeft();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	AGPPlayerController();
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
