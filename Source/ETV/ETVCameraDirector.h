// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ETVGameModeBase.h"
#include "ETVCameraDirector.generated.h"

UCLASS()
class ETV_API AETVCameraDirector : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AETVCameraDirector();

	bool bCanMove;

protected:
	// Scene component (Root Component)
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ETV Camera")
	USceneComponent* SceneComponent;

	// Spring Arm component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ETV Camera")
	USpringArmComponent* SpringArm;

	// Camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ETV Camera")
	UCameraComponent* Camera;

	// Visual Representation component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ETV Camera")
	UStaticMeshComponent* VisualRepresentation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Camera")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Camera")
	float ZoomStep;

	// Minimal zoom
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Camera")
	float ZoomMin;

	// Maximal zoom
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Camera")
	float ZoomMax;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnZoomIn();

	UFUNCTION()
	void OnZoomOut();
	
	UFUNCTION()
	void OnMoveModifier();

	UFUNCTION()
	void OnMoveHorizontal(float AxisValue);

	UFUNCTION()
	void OnMoveVertical(float AxisValue);

	// Move camera above given tile with specified zoom (keep current by default)
	UFUNCTION(BlueprintCallable, Category = "ETV Camera")
	void MoveAboveTile(int32 X, int32 Y, float ZoomPercentage = -1.0f);
	
	// Returns SceneComponent (RootComponent) subobject
	FORCEINLINE USceneComponent* GetSceneComponent() const { return SceneComponent; }

	// Returns SpringArm subobject
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }

	// Returns Camera subobject
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

	// Returns VisualRepresentation subobject
	FORCEINLINE UStaticMeshComponent* GetVisualRepresentation() const { return VisualRepresentation; }
};
