// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class UTextBlock;
class UImage;
class UBorder;
class UInventoryTooltip;
class UDragItemVisual;
class UItemBase;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================
	
	//=====================================================================================
	//                                   FUNCTION
	//=====================================================================================
	
	//=====================================================================================
	//                            FORCEINLINE FUNCTIONS
	//=====================================================================================
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; }
	
	FORCEINLINE void SetItemReference(UItemBase* ItemIn) { ItemReference = ItemIn; }

protected:
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UInventoryTooltip> ToolTipClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UItemBase* ItemReference;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UTextBlock* ItemQuantity;
	//=====================================================================================
	//                                   FUNCTION
	//=====================================================================================
	virtual void NativeOnInitialized() override;
	
	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
