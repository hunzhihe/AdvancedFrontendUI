# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Unreal Engine 5.7 project with an **AdvanceFrontUI** plugin — a frontend/menu UI framework built on **CommonUI**. Provides options screens, key remapping, confirm dialogs, loading screens, and a data-driven settings architecture with GameplayTag-based widget routing.

## Build

This is an Unreal project. Build via the Unreal Editor or:

```bash
# Generate project files (if needed)
<UE_ROOT>/Engine/Build/BatchFiles/RunUAT.bat GenerateProjectFiles -project="AdvancedFrontendUI.uproject" -game -engine

# Build from command line
<UE_ROOT>/Engine/Build/BatchFiles/RunUAT.bat BuildCookRun -project="AdvancedFrontendUI.uproject" -platform=Win64 -build
```

The plugin depends on: `CommonUI`, `EnhancedInput`, `CommonInput`, `GameplayTags`, `PreLoadScreen`, `UMG`, `InputCore`, `PropertyPath`, `DeveloperSettings`.

## Architecture

### Widget Stack System (GameplayTag-driven)

All frontend widgets live in named stacks identified by GameplayTags. The `UFrontendUISubsystem` (GameInstanceSubsystem) owns this flow:

1. `UWidget_PrimaryLayout` registers stack containers (`UCommonActivatableWidgetContainerBase`) against GameplayTags (`Frontend.WidgetStack.*`)
2. Widgets are pushed to stacks via `UFrontendUISubsystem::PushSoftWidgetToStackAsync()` — async loads a soft class reference, creates the widget, pushes to the target stack
3. `UAsyncActionSoftWidget` exposes this as a Blueprint async node
4. `UFrontendDeveloperSettings` maps widget GameplayTags (`Frontend.Widget.*`) to soft class references, so widgets can be resolved by tag

### Options/Settings System (Data-Driven)

The options screen uses a data-object-to-widget-entry mapping pattern:

- **Data layer**: `UListDataObject_Base` subclasses hold option metadata (display name, description, edit conditions, dependency relationships). Concrete types: `UListDataObject_Scalar` (float slider), `UListDataObject_String` (text options via rotator), `UListDataObject_StringEnum`, `UListDataObject_Stringbool`, `UListDataObject_StringResolution`, `UListDataObject_KeyRemap`
- **Value binding**: `UMyListDataObject_Value` subclasses use `FOptionsDataInteractionHelper` — a `FCachedPropertyPath`-based bridge to `UFrontendUIGameUserSettings` getter/setter members — no hardcoded setting names
- **Edit conditions**: `FOptinsDataEditConditionDescriptor` on each `UListDataObject_Base` enables conditional editability with optional forced values when disabled
- **Dependency chain**: `UListDataObject_Base::OnDependencyDataModified` delegate propagates changes through dependent data objects
- **Registry**: `UOptionsDataRegistry` initializes tab collections (Gameplay, Audio, Video, Controls) and provides flat item lists per tab
- **UI layer**: `UWidget_ListEntry_Base` subclasses (extending `UCommonUserWidget` + `IUserObjectListEntry`) render each data object type. `UDataAsset_DataListEntryMapping` maps `TSubclassOf<UListDataObject_Base>` → `TSubclassOf<UWidget_ListEntry_Base>`
- **Flow**: `UWidget_OptionsScreen` → `UFrontendTabListWidgetBase` for tabs → `UFrontendUICommonListView` for items → `UWidget_OptionsDetailView` for selected item info

### Confirm Screen

- `UWidget_ConformScreen` + `UConfirmScreenInfoObject` — supports OK, Yes/No, OK/Cancel variants
- `UAsyncAction_PushConfirmScreen` — Blueprint async node that pushes a confirm screen and returns the clicked button type via delegate

### Key Remapping

- `UWidget_KeyRemapScreen` uses a custom `FKeyRemapScreenInputPreprocessor` to capture raw input
- `UListDataObject_KeyRemap` wraps `FPlayerKeyMapping` from Enhanced Input with reset-to-default support
- Input type filtering via `ECommonInputType`

### Loading Screen

- `UFrontendLoadingScreenSubsystem` (tickable GameInstanceSubsystem) hooks map preload/postload to show/hide a loading screen
- `UFrontendLoadingScreenSettings` configures the widget class and hold duration
- `IFrontendUILoadScreenInterface` notifies widgets of loading screen visibility changes

### Key C++ Base Classes

| Class | Parent | Role |
|---|---|---|
| `UWidget_ActivatableBase` | `UCommonActivatableWidget` | Base for all activatable frontend widgets; caches `AFrontendPlayerController` |
| `UWidget_PrimaryLayout` | `UCommonUserWidget` | Root layout; registers widget stack containers by tag |
| `UFrontendCommonButtonBase` | `UCommonButtonBase` | Styled button; broadcasts description text on hover for info panel |
| `UWidget_ListEntry_Base` | `UCommonUserWidget` + `IUserObjectListEntry` | Base list entry for options items |

### Settings Classes

- `UFrontendDeveloperSettings` (`Config=Game, defaultconfig`) — widget class and image asset tag-to-reference maps
- `UFrontendUIGameUserSettings` — persisted user settings (difficulty, audio, gamma)
- `UFrontendLoadingScreenSettings` (`Config=Game, DefaultConfig`) — loading screen widget class and hold time

### Gameplay Tags

All tags declared natively in `FrontendGameplayTags.h` under namespace `FrontendGameplayTags`:

- Widget stacks: `Frontend_WidgetStack_Modal`, `_GameMenu`, `_GameHud`, `_Frontend`
- Widgets: `Frontend_Widget_PressAnyKeyScreen`, `_MainMenuScreen`, `_ConfirmScreen`, `_StoryScreen`, `_OptionsScreen`, `_KeyRemapScreen`
- Images: `Frontend_Image_TestIamge`

## Code Conventions

- Plugin source is under `Plugins/AdvanceFrontUI/Source/AdvanceFrontUI/` with standard UE Public/Private split
- Blueprint-callable functions use `meta = (Categories = "...")` to restrict GameplayTag dropdowns in the editor
- `LIST_DATA_ACCESSOR(DataType, PropertyName)` macro generates inline getter/setter pairs on data objects
- Widget bindings use `meta = (BindWidget)` or `meta = (BindWidgetOptional, AllowPrivateAccess = "true")`
- Abstract base widgets use `UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))`
- Transient `UPROPERTY` for runtime-only references (never serialized)
- Comments are in Chinese (the developer's native language)
