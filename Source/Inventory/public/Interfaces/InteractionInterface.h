// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class AInventoryCharacter;

UENUM()
enum class EInteractableType : uint8
{
	EIT_Pickup UMETA(DisplayName = "Pickup"),
	EIT_NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"),
	EIT_Device UMETA(DisplayName = "Device"),
	EIT_Toggle UMETA(DisplayName = "Toggle"),
	EIT_Container UMETA(DisplayName = "Container")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	FInteractableData() :
	InteractableType(EInteractableType::EIT_Pickup),
	Name(FText::GetEmpty()),
	Action(FText::GetEmpty()),
	Quantity(0),
	InteractionDuration(0.0)
	{
		
	};

	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	// 픽업에만 사용됨
	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	// 상호작용 타이머가 필요한 밸브, 도어 등의 용도로 사용됨
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//=====================================================================================
	//                            PROPERTIES & VARIABLES
	//=====================================================================================
	FInteractableData InteractableData;
	
	//=====================================================================================
	//                                   FUNCTIONS
	//=====================================================================================
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteract();
	virtual void EndInteract();
	virtual void Interact(AInventoryCharacter* PlayerCharacter);
	
	
};
