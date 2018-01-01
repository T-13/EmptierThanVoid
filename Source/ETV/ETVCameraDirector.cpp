// Copyright (C) Team13. All rights reserved.

#include "ETVCameraDirector.h"
#include "ConstructorHelpers.h"
#include <algorithm>
#include "ETVGameModeBase.h"

// Sets default values
AETVCameraDirector::AETVCameraDirector()
{
	bCanMove = false;
	MoveSpeed = 10.0f;

	ZoomStep = 25.0f;
	ZoomMin = 100.0f;
	ZoomMax = 500.0f;

	// Create subobjects
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	/* Uncomment to enable visual representation (debug)
	// Create and position a mesh component so we can see where our sphere is
	VisualRepresentation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	VisualRepresentation->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		VisualRepresentation->SetStaticMesh(SphereVisualAsset.Object);
		VisualRepresentation->SetWorldScale3D(FVector(0.2f));
	}
	*/

	// Use a spring arm to give the camera smooth, natural-feeling motion
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, -90.0f)); // Orient to tile map (pitch, yaw, roll vs editor's roll, pitch, yaw)
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bEnableCameraLag = true;

	// Create a camera and attach to our spring arm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AETVCameraDirector::BeginPlay()
{
	Super::BeginPlay();

	// Setup binds through player controller input component
	UInputComponent* InputComponent = GetWorld()->GetFirstPlayerController()->InputComponent;

	InputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &AETVCameraDirector::OnZoomIn);
	InputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &AETVCameraDirector::OnZoomOut);
	FInputActionBinding MoveModifierBinding = InputComponent->BindAction("CameraMoveModifier", IE_Pressed, this, &AETVCameraDirector::OnMoveModifier);
	MoveModifierBinding.bConsumeInput = false;
	InputComponent->BindAction("CameraMoveModifier", IE_Released, this, &AETVCameraDirector::OnMoveModifier);
	InputComponent->BindAxis("CameraMoveHorizontal", this, &AETVCameraDirector::OnMoveHorizontal);
	InputComponent->BindAxis("CameraMoveVertical", this, &AETVCameraDirector::OnMoveVertical);
}

void AETVCameraDirector::OnZoomIn()
{
	SpringArm->TargetArmLength = std::max(SpringArm->TargetArmLength - ZoomStep, ZoomMin);
}

void AETVCameraDirector::OnZoomOut()
{
	SpringArm->TargetArmLength = std::min(SpringArm->TargetArmLength + ZoomStep, ZoomMax);
}

void AETVCameraDirector::OnMoveModifier()
{
	bCanMove = !bCanMove;
}

void AETVCameraDirector::OnMoveHorizontal(float AxisValue)
{
	if (bCanMove)
	{
		AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
		
		AxisValue *= -MoveSpeed * std::max(GetZoom(true), 0.2f);
		FVector Location = GetActorLocation();
		Location.X += AxisValue;
		if (GameMode->IsPositionOnTileMap(Location)) {
			AddActorLocalOffset(FVector(AxisValue, 0.0f, 0.0f));
		}
	}
}

void AETVCameraDirector::OnMoveVertical(float AxisValue)
{
	if (bCanMove)
	{
		AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());

		AxisValue *= MoveSpeed * std::max(GetZoom(true), 0.2f);
		FVector Location = GetActorLocation();
		Location.Y += AxisValue;
		if (GameMode->IsPositionOnTileMap(Location)) {
			AddActorLocalOffset(FVector(0.0f, AxisValue, 0.0f));
		}
	}
}

float AETVCameraDirector::GetZoom(bool bInPercentage)
{
	if (bInPercentage)
	{
		return (SpringArm->TargetArmLength - ZoomMin) / (ZoomMax - ZoomMin);
	}

	return SpringArm->TargetArmLength;
}

void AETVCameraDirector::MoveAboveTile(int32 X, int32 Y, float ZoomPercentage)
{
	// Get world position from tile coordinates
	AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
	FVector WorldTilePos = GameMode->GetPosition(X, Y);

	// Set position
	WorldTilePos.Z = GetActorLocation().Z; // Keep current height
	SetActorLocation(WorldTilePos);

	// Set zoom
	if (ZoomPercentage >= 0.0f)
	{
		SpringArm->TargetArmLength = ZoomMax * ZoomPercentage;
	}
}

