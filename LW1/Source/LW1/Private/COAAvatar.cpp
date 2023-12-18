// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"

ACOAAvatar::ACOAAvatar() :
Stamina(100.0f),
MaxStamina(100.0f),
StaminaRegeningRate(0.1f),
StaminaDrainingRate(0.2f),
RunSpeed(600.0f),
bStaminaDrained(false),
bRunning(false)
{
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	mSpringArm->TargetArmLength = 350.0f;
	mSpringArm->SetupAttachment(RootComponent);

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);

	mCamera->bUsePawnControlRotation = false;
	mSpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
}

void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("Move Forward", this, &ACOAAvatar::MoveForward);
	PlayerInputComponent->BindAxis("Move Right", this, &ACOAAvatar::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACOAAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACOAAvatar::RunReleased);
}

void ACOAAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, FString::Printf(TEXT("Stamina = % f"), Stamina));

	if (bRunning == true)
	{
		Stamina = Stamina - StaminaDrainingRate;
		if (Stamina <= 0)
		{
			bRunning = false;
			bStaminaDrained = true;
			Stamina = 0;
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
	else if (bRunning == false)
	{
		Stamina = Stamina + StaminaRegeningRate;
		if (Stamina >= MaxStamina)
		{
			Stamina = MaxStamina;
			bStaminaDrained = false;
		}
	}
}

void ACOAAvatar::BeginPlay()
{
	Super::BeginPlay();
}

void ACOAAvatar::MoveForward(float value)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, value);

}

void ACOAAvatar::MoveRight(float value)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, value);
}

void ACOAAvatar::RunPressed()
{
	if (bStaminaDrained == false && Stamina > StaminaDrainingRate)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		bRunning = true;
	}
}

void ACOAAvatar::RunReleased()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bRunning = false;
}
