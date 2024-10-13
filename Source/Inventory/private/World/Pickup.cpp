// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Pickup.h"

#include "AudioDevice.h"
#include "Components/BoxComponent.h"
#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "Player/InventoryCharacter.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);
	PickupMesh->SetSimulatePhysics(true);

	// 메쉬에 대한 콜리전 설정
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	// 콜리전 응답 설정 (Visibility, WorldStatic은 Block, 나머지는 Ignore)
	PickupMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	PickupMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePickup(ItemQuantity);
}

void APickup::InitializePickup(const int32 InQuantity)
{
	if (!ItemRowHandle.IsNull())
	{
		const FItemData* ItemData = ItemRowHandle.GetRow<FItemData>(ItemRowHandle.RowName.ToString());

		ItemReference = NewObject<UItemBase>(this, UItemBase::StaticClass());

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->AssetData = ItemData->AssetData;
		ItemReference->NumericData.bIsStackable = ItemData->NumericData.MaxStackSize > 1;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	ItemReference->OwningInventory = nullptr;
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);
	
	UpdateInteractableData();
}

void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::EIT_Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}

void APickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact(AInventoryCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}

void APickup::TakePickup(const AInventoryCharacter* Taker)
{
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (UInventoryComponent* PlayerInventory = Taker->GetPlayerInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				switch (AddResult.OperationResult)
				{
				case EItemAddResult::EAR_NoItemAdded:
					break;
				case EItemAddResult::EAR_ParialAmountItemAdded:
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;
				case EItemAddResult::EAR_AllItemAdded:
					Destroy();
					break;
				}
				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Plater inventory component is null!"));
			}
		}
		else
		{
				UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was somehow null!"));
		}
	}
}

void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ?
		PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, RowName))
	{
		if (!ItemRowHandle.IsNull())
		{
			const FItemData* ItemData = ItemRowHandle.GetRow<FItemData>(ItemRowHandle.RowName.ToString());
			PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
		}
	}
}


