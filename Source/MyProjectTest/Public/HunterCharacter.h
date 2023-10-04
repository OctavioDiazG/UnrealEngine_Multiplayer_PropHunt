// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "HunterProjectile.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "HunterCharacter.generated.h"

UCLASS()
class MYPROJECTTEST_API AHunterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHunterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Clase del proyectil a spawnear
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AHunterProjectile> ProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Inputs para mover al personaje hacia adelante y atras
	UFUNCTION()
	void MoveFordward(float Value);

	//Inputs para mover al personaje hacia la izquierda y derecha
	UFUNCTION()
	void MoveRight(float Value);

	//Input para saltar cuando se presiona la tecla
	UFUNCTION()
	void StartJump();

	//Funcion para dejar de saltar al soltar la tecla
	UFUNCTION()
	void StopJump();

	//Input para disparar
	UFUNCTION()
	void Fire();
	
	//Camara FPS
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	//Mesh en primera persona (brazos), visible solo para el jugador
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FPSMesh;

	//Offset para el posicionamiento del arma
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
	
	IOnlineSessionPtr OnlineSessionInterface;


protected:
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

	UFUNCTION(BlueprintCallable)
	void JoinGameSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccess);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
