// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OptionsDataRegistry.generated.h"


class UListDataObject_Collection;
class UListDataObject_Base;
/**
 * 选项数据注册表
 * 负责初始化所有选项选项卡的数据（Gameplay、Audio、Video、Controls），
 * 提供按选项卡 ID 查询列表源数据的能力。
 * 支持树形数据结构，可递归查找子级列表数据。
 */
UCLASS()
class ADVANCEFRONTUI_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:

	/** 初始化所有选项选项卡集合 */
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	/** 获取已注册的所有选项选项卡集合 */
	const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections; }

	/** 根据选中的选项卡 ID 获取该选项卡下的所有列表项（递归展开子级） */
	TArray<UListDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;

private:

	/** 递归查找子级列表数据 */
	void FindChildListDataRecursively(UListDataObject_Base* InParantData, TArray<UListDataObject_Base*>& OutFoundChildListData) const;

	/** 初始化 Gameplay 选项卡的数据集合 */
	void InitGameplayCollectionTab();
	/** 初始化 Audio 选项卡的数据集合 */
	void InitAudioCollectionTab();
	/** 初始化 Video 选项卡的数据集合 */
	void InitVideoCollectionYab();
	/** 初始化 Controls（按键映射）选项卡的数据集合 */
	void InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer);

	//维护该变量主要是为了获取UISubsystem,以便于语言切换在初始化时直接生效
	TWeakObjectPtr<UWorld> CachedWorld;

	/** 已注册的选项选项卡集合（Transient，运行时持有） */
	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabCollections;

};
