// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_EnemyPrototype.h"
#include "AIController.h"



// Sets default values
AGP_EnemyPrototype::AGP_EnemyPrototype()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurHealth = MaxHealth;

	//Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	Capsule = FindComponentByClass<UCapsuleComponent>();
	if (Capsule) {
		RootComponent = Capsule;
	}

	else
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("No Maidens ")));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	Sphere->SetupAttachment(RootComponent);



	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor Epic"));
}

// Called when the game starts or when spawned
void AGP_EnemyPrototype::BeginPlay()
{
	Super::BeginPlay();

	GoToHeart(Heart);

	PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));

	if(!PlayerController)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No Player Controller ")));

	if(!Heart)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No Heart ")));
}

// Called every frame
void AGP_EnemyPrototype::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


float AGP_EnemyPrototype::Damage(float DamageAmount)
{
	CurHealth -= DamageAmount;

	if (CurHealth <= 0)
	{
		Destroy();
	}

	if (!AIController->MoveToActor(PlayerController))
	{
		GoToHeart(Heart);
	}


	return DamageAmount;
}

void AGP_EnemyPrototype::GoToHeart(AActor* Target)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Going To Heart")));

	AIController = Cast<AAIController>(GetController());

	if (AIController)
	{
		AIController->MoveToActor(Target);
		//AIController->MoveToLocation(Target->GetActorLocation());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("FOUND HEART GOINT TO NOW")));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Found AI Controller")));
	}

	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No AI Controller")));
	}

}


void AGP_EnemyPrototype::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Overlapped with %s"), OtherActor->GetFName()));
	UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *OtherActor->GetName());
}

void AGP_EnemyPrototype::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GoToHeart(Heart);
}



void AGP_EnemyPrototype::OnSeePawn(APawn* OtherPawn)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Can See Pawn")));

	if(!OtherPawn)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Pawn Missing")));

	if (AIController->MoveToActor(OtherPawn))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Can reach Pawn?")));
	}

	else {
		GoToHeart(Heart);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Lost pwaywer, gwoing two hweart")));
	}
}

void AGP_EnemyPrototype::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("HEARD PAWN")));

}

void AGP_EnemyPrototype::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensor->OnSeePawn.AddDynamic(this, &AGP_EnemyPrototype::OnSeePawn);
	PawnSensor->OnHearNoise.AddDynamic(this, &AGP_EnemyPrototype::OnHearNoise);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGP_EnemyPrototype::OnOverlapBegin);
}

