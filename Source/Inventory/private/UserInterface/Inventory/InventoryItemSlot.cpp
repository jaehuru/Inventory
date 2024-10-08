// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryItemSlot.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryTooltip.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ToolTipClass)
	{
		UInventoryTooltip* Tooltip = CreateWidget<UInventoryTooltip>(this, ToolTipClass);
		Tooltip->InventorySlotBeingHovered = this;
		SetToolTip(Tooltip);
	}
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemReference)
	{
		switch (ItemReference->ItemQuality)
		{
		case EItemQuality::EIQ_Common:
			ItemBorder->SetBrushColor(FLinearColor::White);
			break;
		case EItemQuality::EIQ_Rare:
			ItemBorder->SetBrushColor(FLinearColor::Green);
			break;
		case EItemQuality::EIQ_Unique:
			ItemBorder->SetBrushColor(FLinearColor::Blue);
			break;
		case EItemQuality::EIQ_Legendary:
			ItemBorder->SetBrushColor(FLinearColor(1.f, 0.45f, 0.f)); //orange color
			break;
		case EItemQuality::EIQ_Epic:
			ItemBorder->SetBrushColor(FLinearColor::Red);
			break;
		}

		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

		if (ItemReference->NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
