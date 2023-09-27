// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "HunterHUD.generated.h"

UCLASS()
class MYPROJECTTEST_API AHunterHUD : public AHUD
{
	GENERATED_BODY()

protected:
	//Esto sera dibujado en el centro de la pantalla
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTexture;

public:
	//Primera llamada para dibujar el HUD
	virtual void DrawHUD() override;
};
