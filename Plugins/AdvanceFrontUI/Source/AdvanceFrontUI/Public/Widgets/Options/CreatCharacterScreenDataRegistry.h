// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CreatCharacterScreenDataRegistry.generated.h"

class UListDataObject_Collection;
class UListDataObject_Base;
class UPlayerSaveData;

/**
 * <角色创建界面>数据注册表
 * 负责初始化所有选项卡的数据（CreateCharacterInfo 等），
 * 提供按选项卡 ID 查询列表源数据的能力。
 * 支持树形数据结构，可递归查找子级列表数据。
 * 数据绑定桥接到 UPlayerSaveData（而非 UFrontendUIGameUserSettings）。
 */
UCLASS()
class ADVANCEFRONTUI_API UCreatCharacterScreenDataRegistry : public UObject
{
    GENERATED_BODY()

public:
    /** 初始化所有选项选项卡集合 */
    void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

    /** 获取已注册的所有选项选项卡集合 */
    const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const { return RegisteredCharacterCreateTabCollections; }

    /** 根据选中的选项卡 ID 获取该选项卡下的所有列表项（递归展开子级） */
    TArray<UListDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;

    /** 刷新所有数据对象的本地化文本（语言切换时调用，原地更新不重建对象） */
    void RefreshAllLocalizedText();

    /** 获取角色保存数据实例（供 Widget_CreatCharacter 在停用时保存到磁盘） */
    UPlayerSaveData* GetPlayerSaveData() const { return CreatedPlayerSaveData; }

private:
    /** 递归查找子级列表数据 */
    void FindChildListDataRecursively(UListDataObject_Base* InParantData, TArray<UListDataObject_Base*>& OutFoundChildListData) const;

    /** 递归刷新子级列表数据的本地化文本 */
    void RefreshChildListDataRecursively(UListDataObject_Base* InParantData);

    /** 初始化角色基础信息选项卡的数据集合 */
    void InitGameCharacterCollectionTab();

    // ========== 数据实例 ==========

    /** 已注册的选项选项卡集合（Transient，运行时持有） */
    UPROPERTY(Transient)
    TArray<UListDataObject_Collection*> RegisteredCharacterCreateTabCollections;

    /** 运行时的角色存档数据实例，所有数据对象通过 FOptionsDataInteractionHelper 桥接到此实例 */
    UPROPERTY(Transient)
    UPlayerSaveData* CreatedPlayerSaveData;
};
