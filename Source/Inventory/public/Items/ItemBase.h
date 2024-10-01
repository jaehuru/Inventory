// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDataStructs.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================
	// UPROPERTY()
	// UInventoryComponent* OwningInventory;

	/** 아이템 양 */
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;

	/** 아이템 특성 */
	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemQuality ItemQuality;

	/** 아이템 타입 */
	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemStatistics ItemStatistics;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssetData AssetData;

	//=====================================================================================
	//                                   FUNCTIONS
	//=====================================================================================
	UItemBase();

	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(AInventoryCharacter* Character);
	
	//=====================================================================================
	//                            FORCEINLINE FUNCTIONS
	//=====================================================================================
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; }
	
protected:
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================
	
	//=====================================================================================
	//                                   FUNCTIONS
	//=====================================================================================
};
