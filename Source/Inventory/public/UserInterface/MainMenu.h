// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class AInventoryCharacter;
/**
 * 
 */
UCLASS()
class INVENTORY_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AInventoryCharacter* PlayerCharacter;
	
};
