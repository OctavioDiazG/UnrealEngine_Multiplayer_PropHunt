// Fill out your copyright notice in the Description page of Project Settings.
#include "HunterCharacter.h"

// Sets default values
AHunterCharacter::AHunterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Crear el componente de camara en primera persona
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	//Attach la camara al compoente de capsula
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	//Posicionar la camara ligeramente superior a la altura de los ojos
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	//Habilitar al pawn el control de la rotacion de la camara
	FPSCameraComponent->bUsePawnControlRotation = true;

	//Crear el mesh en primera persona para el propio jugador
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

	//Solo el propio jugador puede ver el mesh
	FPSMesh->SetOnlyOwnerSee(true);

	//Attach el mesh FPS a la camara
	FPSMesh->SetupAttachment(FPSCameraComponent);

	//Desactivar algunas sombras para mantener la ilusion de tener un solo mesh
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	//Desactivar la vision del jugador del mesh en tercera persona
	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AHunterCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	//Mostrar mensaje para comprovar que se esta usando el character
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using HunterCharacter"));
}

// Called every frame
void AHunterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHunterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Incluir los bindings de movimiento
	PlayerInputComponent->BindAxis("MoveForward", this, &AHunterCharacter::MoveFordward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHunterCharacter::MoveRight);

	//Incluir los bindings de vision (camara)
	PlayerInputComponent->BindAxis("Turn", this, &AHunterCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AHunterCharacter::AddControllerPitchInput);

	//Incluir los bindings de salto
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHunterCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AHunterCharacter::StopJump);

	//Incluir binding de disparo
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHunterCharacter::Fire);
}

//Definicion de la funcion para mover al jugador hacia adelante y hacia atras
void AHunterCharacter::MoveFordward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

//Definicion de la funcion para mover al jugador hacia la izquierda y derecha
void AHunterCharacter::MoveRight(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

//Funcion para declarar true la accion de saltar
void AHunterCharacter::StartJump()
{
	bPressedJump = true;
}

//Funcion para declarar true la accion de saltar
void AHunterCharacter::StopJump()
{
	bPressedJump = false;
}

//Funcion para que el jugador dispare
void AHunterCharacter::Fire()
{
	//Intentar disparar un proyectil
	if (ProjectileClass)
	{
		//Obtener el transform de la camara
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		//Definir el punto de spawn de los proyectiles
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		//Definir el transform de donde salen los proyectiles
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		//Inclinar el objetivo un poco al disparar
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 5.0f;

		UWorld* World = GetWorld();
		if(World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			//Spawnear el proyectil
			AHunterProjectile* Projectile = World->SpawnActor<AHunterProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				//Definir trayectoria inicial del proyectil
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}