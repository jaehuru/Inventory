// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

UItemBase::UItemBase()
{
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

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(
			NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);

		/* 
		if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				OwingInventory->RemoveItem(this);
			}
		}
		*/
	}
}

void UItemBase::Use(AInventoryCharacter* Character)
{
	
}
