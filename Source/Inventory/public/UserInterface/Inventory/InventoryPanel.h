// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/InventoryCharacter.h"
#include "InventoryPanel.generated.h"

class UInventoryItemSlot;
class UWrapBox;
class UTextBlock;
class UInventoryCharacter;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================
	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryWrapBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeightInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CapacityInfo;

	UPROPERTY()
	AInventoryCharacter* PlayerCharacter;

	UPROPERTY()
	UInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;
	
	//=====================================================================================
	//                                   FUNCTION
	//=====================================================================================
	UFUNCTION()
	void RefreshInventory();

	//=====================================================================================
	//                            FORCEINLINE FUNCTIONS
	//=====================================================================================

protected:
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================

	//=====================================================================================
	//                                   FUNCTION
	//=====================================================================================
	void SetInfoText() const;
	
	virtual void NativeOnInitialized() override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
