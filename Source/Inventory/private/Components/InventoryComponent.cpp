// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Items/ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate(
			[&ItemIn](const UItemBase* InventoryItem)
			{
				return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
			}))
	{
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt(
		(GetInventoryWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItem(UItemBase* InputItem)
{
	// 아이템 무게 확인
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			"Could not add {0} to the inventory. Item has invalid weight value."),
			InputItem->TextData.Name));
	}

	// 무게가 가득찰때
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetInventoryWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			"Could not add {0} to the inventory. Item would overflow weight limit."),
			InputItem->TextData.Name));
	}

	// 슬롯이 가득찰때
	if (InventoryContents.Num() + 1 > InventorySlotCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			"Could not add {0} to the inventory. All inventory slots are full."),
			InputItem->TextData.Name));
	}

	AddNewItem(InputItem, 1);
	
	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString(
			"Successfully added a single {0} to the inventory."),
			InputItem->TextData.Name));
}

int32 UInventoryComponent::HandleStackableItem(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		// 무효한 아이템 데이터
		return 0;
	}
	
	int32 AmountToDistribute = RequestedAddAmount;

	// 아이템이 인벤토리에 이미 존재하고 풀스택이 아닌지 확인
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

	// 아이템 풀스택 되었을때 나누기
	while (ExistingItemStack)
	{
		// 풀스택이 되려면 필요수량이 어느정도인지 계산
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		// 무게 한도를 기준으로 들고올 수 있는 'AmountToMakeFullStack'의 수를 계산
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		// 아이템의 남은 양이 무게 용량을 초과하지 않을때
		if (WeightLimitAddAmount > 0)
		{
			// 현재 아이템 스택과 인벤토리 총 무게를 조정
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

			// 분배할 수 있도록 수 조정
			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);
			
			// 인벤토리 무게 한도가 다른 아이템으로 초과가 된다면 리턴
			if (InventoryTotalWeight + ExistingItemStack->GetItemSingleWeight() > InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if (WeightLimitAddAmount <= 0)
		{
			if (AmountToDistribute != RequestedAddAmount)
			{
				// 여러 스택에 분산시키고 그 과정에서 무게 제한에 부딪히면 이곳에 도달
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
			
			// 스택이 덜 찼지만 전혀 추가할 수 없는 경우 도달
			return 0;
		}

		if (AmountToDistribute <= 0)
		{
			// 아이템들이 기존 스택에 분산
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		// 다른 유효한 부분 스택이 있는지 확인
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// 다른 스택을 찾을 수 없을때, 새로운 스택을 추가할 수 있는지 확인
	if (InventoryContents.Num() + 1 <= InventorySlotCapacity)
	{
		// 남아있는 아이템에서 인벤토리 무게에 맞게 최대한 많은 양을 추가하려고 시도
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{
			// 분배할 아이템이 더 있는데 무게 제한에 도달한 경우
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				// 아이템을 조정하고 보유할 수 있는 만큼의 새 스택을 추가합니다
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);

				// 부분 스택만 추가되므로 사본 만들기
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}

			// 스택의 나머지 전체를 추가 가능
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}

		// 사용 가능한 슬롯이 있지만 남은 무게 용량이 없는 경우 도달
		return RequestedAddAmount - AmountToDistribute;
	}

	// 사용 가능한 슬롯이 없고 남은 무게 용량도 없는 경우 도달
	return 0;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// handle non-stackable items
		if (!InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItem(InputItem);
		}
		
		// handle stackable
		const int32 StackableAmountAdded = HandleStackableItem(InputItem, InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(FText::FromString(
				"Successfully added {0} {1} to the inventory."),
				InitialRequestedAddAmount,
				InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(FText::FromString(
			"Partial amount of {0} added to the inventory. Number added = {1}"),
			InputItem->TextData.Name,
			StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(FText::FromString(
			"Could not add {0} to the inventory. No remaining inventory slots, or invalid item."),
			InputItem->TextData.Name));
		}
	}

	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed."));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		// if the item is already a copy, or is a world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// used when splitting or dragging to/from another inventory
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}


