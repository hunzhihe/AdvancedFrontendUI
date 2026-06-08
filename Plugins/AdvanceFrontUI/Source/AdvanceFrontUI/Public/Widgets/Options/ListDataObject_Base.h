// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FrontendTypes/FrontendEnumType.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "ListDataObject_Base.generated.h"

#define LIST_DATA_ACCESSOR(DataType, PropertyName)\
       FORCEINLINE DataType Get##PropertyName() const { return PropertyName;}\
	   void Set##PropertyName(DataType In##PropertyName){PropertyName = In##PropertyName;}


/**
 * 
 */
UCLASS(Abstract)
class ADVANCEFRONTUI_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()

	
public:

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UListDataObject_Base*, EOptionsLsitDataModifyReason);


	FOnListDataModifiedDelegate OnListDataModified;
	FOnListDataModifiedDelegate OnDependencyDataModified;


	LIST_DATA_ACCESSOR(FName, DataID);
	LIST_DATA_ACCESSOR(FText, DataDisplayName);
	LIST_DATA_ACCESSOR(FText, DescriptionRichText);
	LIST_DATA_ACCESSOR(FText, DisabledRichText);
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDiscriptionImage);
	LIST_DATA_ACCESSOR(UListDataObject_Base*, ParentData);


	void InitDataObject();



	//将在子类中重写该函数
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const
	{
		return TArray<UListDataObject_Base*>();
	}

	virtual bool HasAnyChildListData() const { return false; }

	void SetShouldApplySettingsImmediately(bool bShouldApplyRightAway) {
	
		bShouldApplyChangeImmediatly = bShouldApplyRightAway;
	};

	virtual bool HasDefaultValue() const { return false; }
	virtual bool CanResetBackToDefaultValue() const { return false; }
	virtual bool TryResetBackToDefaultValue()  { return false; }

	//Gets called from OptionsDataRegister for adding in edit conditions for the constructed list data objects
	void AddEditCondition(const FOptinsDataEditConditionDescriptor& InEditCondition);

	//gets called from OptionsDataRegistry to add in dependency data
	void AddEditDependencyData(UListDataObject_Base* InDependencyData);

	bool IsDataCurrentlyEditable();


protected:
    
	virtual void OnDataObjectInitialized();

	virtual void NotifyListDataModified(UListDataObject_Base* InModifiedListDataObject, EOptionsLsitDataModifyReason ModifyReason = EOptionsLsitDataModifyReason::DirectlyModified);

	//the child class should override this to allow the value beset to the forced string value
	virtual bool CanSetToForcedStringvalue(const FString& InForcedValue) const { return false; }
	//the child class should override this to specify how to set the current value to the forced calue; 
	virtual void OnSetToForcedStringvalue(const FString& InForcedValue) {}
	//This func will be called when the value of the dependency data has changed,the child class can overide this func to handle the custom logic need, super call is expected.
	virtual void OnEditDependencyDataModifiied(UListDataObject_Base* InModifiedDependencyData, EOptionsLsitDataModifyReason ModifyReason);

private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDiscriptionImage;

	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;

	bool bShouldApplyChangeImmediatly = false;


	UPROPERTY(Transient)
	TArray<FOptinsDataEditConditionDescriptor> EditConditionDescArray;

};
