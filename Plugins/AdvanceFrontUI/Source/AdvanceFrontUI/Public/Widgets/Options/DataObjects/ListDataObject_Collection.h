// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListDataObject_Base.h"
#include "ListDataObject_Collection.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_Collection : public UListDataObject_Base
{
	GENERATED_BODY()

public:

	void AddChildListData(UListDataObject_Base* InChildListData);


	virtual TArray<UListDataObject_Base*> GetAllChildListData() const;
	virtual bool HasAnyChildListData() const;

private:

	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;
	
};
