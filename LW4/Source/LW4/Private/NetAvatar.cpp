// Fill out your copyright notice in the Description page of Project Settings.


#include "NetAvatar.h"
#include "GameFramework/CharacterMovementComponent.h"

ANetAvatar::ANetAvatar() :
    RunSpeed(600.0f),
    WalkSpeed(300.0f),
    isRunning(false)
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    
}

void ANetAvatar::BeginPlay()
{
    Super::BeginPlay();

    Camera->bUsePawnControlRotation = false;
    SpringArm->bUsePawnControlRotation = true;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ANetAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("Lookup", this, &ACharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("MoveForward", this, &ANetAvatar::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ANetAvatar::MoveRight);
    PlayerInputComponent->BindAction("Run",EInputEvent::IE_Pressed, this, &ANetAvatar::RunPressed);
    PlayerInputComponent->BindAction("Run",EInputEvent::IE_Released, this, &ANetAvatar::RunReleased);
    
}

void ANetAvatar::MoveForward(float Scale)
{
    FRotator Rotation = GetController()->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, Scale);
}

void ANetAvatar::MoveRight(float Scale)
{
    FRotator Rotation = GetController()->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(ForwardDirection, Scale);
}

void ANetAvatar::RunPressed()
{
    if (HasAuthority())
    {
        isRunning = true;
        OnRepRun();
    }
    else
    {
        ServerStartRun();
    }
}

void ANetAvatar::RunReleased()
{
    if (HasAuthority())
    {
        isRunning = false;
        OnRepRun();
    }
    else
    {
        ServerStopRun();
    }
}

void ANetAvatar::ServerStartRun_Implementation()
{
    RunPressed();
}

void ANetAvatar::ServerStopRun_Implementation()
{
    RunReleased();
}

void ANetAvatar::OnRepRun()
{
    if (isRunning)
    {
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}
