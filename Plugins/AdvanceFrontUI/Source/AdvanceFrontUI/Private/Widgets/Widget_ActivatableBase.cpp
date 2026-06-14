// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_ActivatableBase.h"
#include "Controllers/FrontendPlayerController.h"

AFrontendPlayerController *UWidget_ActivatableBase::GetOwningFrontendPlayerController()
{
	// 缓存无效时重新查找（懒加载缓存模式）
    if(!CacheOwningPlayerController.IsValid())
    {
        CacheOwningPlayerController = GetOwningPlayer<AFrontendPlayerController>();
    }

    return CacheOwningPlayerController.IsValid() ? CacheOwningPlayerController.Get() : nullptr;
}
