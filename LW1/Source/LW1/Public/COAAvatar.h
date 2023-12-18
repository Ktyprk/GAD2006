// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COABaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "COAAvatar.generated.h"

/**
 * 
 */
UCLASS()
class LW1_API ACOAAvatar : public ACOABaseCharacter
{
	GENERATED_BODY()

public:
	ACOAAvatar();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "COA")
	UCameraComponent* mCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "COA")
	USpringArmComponent* mSpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "COA")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "COA")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "COA")
	float StaminaRegeningRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "COA")
	float StaminaDrainingRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "COA")
	bool bStaminaDrained;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "COA")
	bool bRunning;

	UPROPERTY(EditAnywhere, Category = "COA")
	float RunSpeed;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

private:
	void MoveForward(float value);
	void MoveRight(float value);
	void RunPressed();
	void RunReleased();
};
