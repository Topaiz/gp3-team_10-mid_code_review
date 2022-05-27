// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GP_Astronaut_AIController.generated.h"

/**
 * 
 */
UCLASS()
class GP3_UPROJECT_API AGP_Astronaut_AIController : public AAIController
{
	GENERATED_BODY()

public: 

	virtual void BeginPlay() override;

	AGP_Astronaut_AIController(const FObjectInitializer& ObjectInitializer);

	virtual void OnPossess(APawn* InPawn) override;

	class AGP_EnemyPrototype* Agent;

	UPROPERTY(transient)
	class UBlackboardComponent* blackboard_component;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* behavior_tree_component;

	uint8 EnemyKeyId;
	uint8 LocationKeyId;
	//uint8 ContactKeyId;

};
