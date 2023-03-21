// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "QuestTableRow.h"

class FToolBarBuilder;
class FMenuBuilder;
class SQuestTableRow;
class STableViewBase;
class ITableRow;

class FCheckQuestModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void AddMenuExtension(FMenuBuilder& Build);
	void AddToolbarExtension(FToolBarBuilder& Build);
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	static TSharedRef<SWidget> GetRowHeader(FString Text);

private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	
	TArray<AQuest*> Quests;
	

	TSharedRef<ITableRow> GenerateQuestRow(AQuest* Quest, const TSharedRef<STableViewBase>& Table) const;
	
	void UpdateQuest();
	void UpdateQuestsLists();
	void TabClosed(TSharedRef<SDockTab> Tab);

	TSharedPtr<SListView<AQuest*>> QuestsList;

	/*TSharedPtr<SVerticalBox> QuestsList;*/
	
	TSharedPtr<SBox> ObjectivesHolder;
};
