// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
     OK,
	 YesNo,
	 OkCancel,
	 Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
     Confirmed,
	 Cancelled,
	 Closed,
	 Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionsLsitDataModifyReason : uint8
{
	DirectlyModified,
	DependencyModified,
	ResetToDefault,
	Unknown UMETA(Hidden)
};
