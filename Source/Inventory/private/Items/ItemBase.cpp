// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

#include "Components/InventoryComponent.h"

UItemBase::UItemBase() :
bIsCopy(false),
bIsPickup(false)
{
}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ID = this->ID;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(
			NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);
		
		if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemBase OwningInventory was null (Item may be a pickup)."))
		}
	}
}

void UItemBase::Use(AInventoryCharacter* Character)
{
	
}
