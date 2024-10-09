// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InventoryHUD.generated.h"
#pragma region 
struct FInteractableData;
class UInventoryPanel;
class UInteractionWidget;
class UMainMenu;
#pragma endregion 
/**
 * 
 */
UCLASS()
class INVENTORY_API AInventoryHUD : public AHUD
{
	GENERATED_BODY()

public:
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================
#pragma region
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;
	
	bool bIsMenuVisible;
#pragma endregion 
	//=====================================================================================
	//                                   FUNCTIONS
	//=====================================================================================
#pragma region 
	AInventoryHUD();

	void ShowMenu();
	void HideMenu();

	void ShowCrosshair();
	void HideCrosshair();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;
#pragma endregion 
protected:
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================
#pragma region 
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	UPROPERTY()
	UUserWidget* CrosshairWidget;
#pragma endregion 
	//=====================================================================================
	//                                   FUNCTIONS
	//=====================================================================================
#pragma region 
	virtual void BeginPlay() override;
#pragma endregion 
};
