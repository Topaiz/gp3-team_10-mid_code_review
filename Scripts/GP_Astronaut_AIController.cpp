// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_Astronaut_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GP_EnemyPrototype.h"
#include "SmartActor.h"

AGP_Astronaut_AIController::AGP_Astronaut_AIController(FObjectInitializer const& object_initializer)
{
	behavior_tree_component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
	blackboard_component = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void AGP_Astronaut_AIController::BeginPlay()
{
	Super::BeginPlay();

	if (!Agent)
	{
		AGP_EnemyPrototype* Chr = Cast<AGP_EnemyPrototype>(GetPawn());
		if (!Chr)
		{
			return;
		}

		Agent = Chr;
	}

	if (Agent->SmartActor)
	{
		FGameplayTag SubTag;
		behavior_tree_component->SetDynamicSubtree(SubTag, Agent->SmartActor->SubTree);
	}
}



void AGP_Astronaut_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGP_EnemyPrototype* Chr = Cast<AGP_EnemyPrototype>(InPawn);

	/*if (Chr != nullptr && Chr->BehaviorTreeAsset->BlackboardAsset)
	{
		Agent = Chr;

		//SOMETHING IN HERE CRASHES UNREAL. WHY? WHY? WHY???? WHY???????? WHY????????????????????????????????????

		blackboard_component->InitializeBlackboard(*Chr->BehaviorTreeAsset->BlackboardAsset);

		EnemyKeyId = blackboard_component->GetKeyID("TargetActor");
		LocationKeyId = blackboard_component->GetKeyID("MoveToLocation");

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("behavior_tree_component = %f"), behavior_tree_component));

		behavior_tree_component->StartTree(*Chr->BehaviorTreeAsset);
	}*/
}
