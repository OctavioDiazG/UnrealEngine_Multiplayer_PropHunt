// Fill out your copyright notice in the Description page of Project Settings.
#include "HunterHUD.h"

void AHunterHUD::DrawHUD()
{
	Super::DrawHUD();

	//Comprobacion y creacion del dibujado de textura
	if (CrosshairTexture)
	{
		//Buscar el centro del canvas
		FVector2d Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		//Offset para que la textura este exactamente en medio
		FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

		//Dibujar la mira en el centro
		FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

