// Copyright Epic Games, Inc. All Rights Reserved.

#include "CheckQuest.h"

#include "CheckQuestCommands.h"
#include "CheckQuestStyle.h"
#include "LevelEditor.h"
#include "QuestTableRow.h"
#include "Selection.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"

static const FName CheckQuestTabName("CheckQuest");

#define LOCTEXT_NAMESPACE "FCheckQuestModule"

void FCheckQuestModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCheckQuestStyle::Initialize();
	FCheckQuestStyle::ReloadTextures();

	FCheckQuestCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FCheckQuestCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FCheckQuestModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCheckQuestModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CheckQuestTabName, FOnSpawnTab::CreateRaw(this, &FCheckQuestModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FCheckQuestTabTitle", "CheckQuest"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	TSharedPtr<FExtender> MenuExtender = MakeShareable( new FExtender());
	MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands,
		FMenuExtensionDelegate::CreateRaw(this, &FCheckQuestModule::AddMenuExtension));

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable( new FExtender());
	ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands,
		FToolBarExtensionDelegate::CreateRaw(this, &FCheckQuestModule::AddToolbarExtension));

	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}

void FCheckQuestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCheckQuestStyle::Shutdown();

	FCheckQuestCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CheckQuestTabName);
}

void FCheckQuestModule::AddMenuExtension(FMenuBuilder& Build)
{
	Build.AddMenuEntry(FCheckQuestCommands::Get().OpenPluginWindow);
}

void FCheckQuestModule::AddToolbarExtension(FToolBarBuilder& Build)
{
	Build.AddToolBarButton(FCheckQuestCommands::Get().OpenPluginWindow);
}

TSharedRef<SDockTab> FCheckQuestModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::FromString(TEXT("Move Objects"));

	UpdateQuest();

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.OnTabClosed_Raw(this, &FCheckQuestModule::TabClosed)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SButton)
					.OnPressed_Raw(this, &FCheckQuestModule::UpdateQuestsLists)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Update Quest List from Select"))
					]
				]
				+ SVerticalBox::Slot()
				.FillHeight(1.f)
				[
					/*SNew(SBox)
					[
						SNew(SVerticalBox)
						+SVerticalBox::Slot()
						.AutoHeight()
							[
							SAssignNew(QuestsList, SVerticalBox)
							]
					]	*/
					SNew(SBox)
					[
						SAssignNew(QuestsList, SListView<AQuest*>)
						.ItemHeight(24)
						.ListItemsSource(&Quests)
						.OnGenerateRow_Raw(this, &FCheckQuestModule::GenerateQuestRow)
						.HeaderRow
						(
							SNew(SHeaderRow)
							+ SHeaderRow::Column("Name")[GetRowHeader("Name")]
							+ SHeaderRow::Column("Description")[GetRowHeader("Description")]
							+ SHeaderRow::Column("IsStory")[GetRowHeader("IsStory")]
							+ SHeaderRow::Column("Objectives")[GetRowHeader("Objectives")]
							+ SHeaderRow::Column("KeepOrder")[GetRowHeader("KeepOrder")]
							+ SHeaderRow::Column("Previous Quest")[GetRowHeader("Previous Quest")]
						)
						
					]
					
				]
				+ SVerticalBox::Slot()
				.FillHeight(1.f)
				[
					SAssignNew(ObjectivesHolder, SBox)
				]
			]
		];
}

void FCheckQuestModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(CheckQuestTabName);
}

TSharedRef<SWidget> FCheckQuestModule::GetRowHeader(FString Text)
{
	return SNew(SBorder)
		.HAlign(HAlign_Center)
		.Padding(1.f)
		.Content()
		[
		SNew(STextBlock)
		.Text(FText::FromString(Text))
		];
}

TSharedRef<ITableRow> FCheckQuestModule::GenerateQuestRow(AQuest* Quest, const TSharedRef<STableViewBase>& Table) const
{
		return SNew(SQuestTableRow, Table)
		.Quest(Quest)
		.ObjectivesHolder(ObjectivesHolder);
}

void FCheckQuestModule::UpdateQuest()
{
	Quests.Empty();
	for (FSelectionIterator Iter(*GEditor->GetSelectedActors()); Iter; ++Iter)
	{
		if (auto* Quest = Cast<AQuest>(*Iter))
		{
			Quests.AddUnique(Quest);
		}
	}
	if (Quests.Num() > 0)
	{
		GEditor->SelectNone(true, true);
		for (auto* Quest : Quests)
		{
			GEditor->SelectActor(Quest, true, false);
		}
	}
	
}

void FCheckQuestModule::UpdateQuestsLists()
{
	/*QuestsList->ClearChildren();

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	FSinglePropertyParams Params;
	Params.NamePlacement = EPropertyNamePlacement::Hidden;

	for (auto* Quest : Quests)
	{
		QuestsList->AddSlot()
		[
			SNew(SBox)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, Name), Params).ToSharedRef()
				]
				+SHorizontalBox::Slot()
				[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, Description), Params).ToSharedRef()
				]
				+SHorizontalBox::Slot()
				[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, bIsStoryQuest), Params).ToSharedRef()
				]
				+SHorizontalBox::Slot()
				[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, bKeepObjectivesOrder), Params).ToSharedRef()
				]
				+SHorizontalBox::Slot()
				[
					SNew(SButton)
					.OnPressed(this,
					SAssignNew(ObjectivesHolder, SBox)
					)
					[
					SNew(SBox)
						[
						SNew(STextBlock)
						.Text(FText::FromString("Show Objectives"))
						]
					]
				]	
				+SHorizontalBox::Slot()
				[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, PreviousQuest), Params).ToSharedRef()
				]
			]
		];
	}*/
	ObjectivesHolder->SetContent(SNullWidget::NullWidget);
	UpdateQuest();
	QuestsList->RequestListRefresh();
}

void FCheckQuestModule::TabClosed(TSharedRef<SDockTab> Tab)
{
	ObjectivesHolder.Reset();
	QuestsList.Reset();
}

void FCheckQuestModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	/*{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FCheckQuestCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FCheckQuestCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}*/
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCheckQuestModule, CheckQuest)

