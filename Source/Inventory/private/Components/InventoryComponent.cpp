// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

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

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemIn)
{
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
}

FItemAddResult UInventoryComponent::HandleNonStackableItem(UItemBase*, int32 RequestedAddAmount)
{
}

int32 UInventoryComponent::HandleStackableItem(UItemBase*, int32 RequestedAddAmount)
{
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase*, int32 RequestedAddAmount)
{
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* ExistingItem, int32 InitialRequestedAddAmount)
{
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
}


