// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Pickup.generated.h"

class UItemBase;
class AInventoryCharacter;

UCLASS()
class INVENTORY_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================

	
	//=====================================================================================
	//                                   FUNCTION
	//=====================================================================================
	
	// Sets default values for this actor's properties
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	virtual void BeginFocus() override;
	
	virtual void EndFocus() override;

	//=====================================================================================
	//                            FORCEINLINE FUNCTIONS
	//=====================================================================================
	FORCEINLINE UItemBase* GetItemData() const { return ItemReference; }

protected:
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;
	
	//=====================================================================================
	//                                   FUNCTION
	//=====================================================================================
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact(AInventoryCharacter* PlayerCharacter) override;

	void UpdateInteractableData();
	
	void TakePickup(const AInventoryCharacter* Taker);
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
