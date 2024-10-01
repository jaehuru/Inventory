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
	UPROPERTY(VisibleAnywhere, Category = "Item Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	/** 아이템 특성 */
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	/** 아이템 타입 */
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
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
		return ID == OtherID;
	}
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================
	
	//=====================================================================================
	//                                   FUNCTIONS
	//=====================================================================================
};
