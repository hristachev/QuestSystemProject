#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "Objective.h"

class SQuestTableRow : public SMultiColumnTableRow<AQuest*>
{
public:

	SLATE_BEGIN_ARGS(SQuestTableRow) {}
	
		SLATE_ARGUMENT(TSharedPtr<SBox>, ObjectivesHolder)
	
		SLATE_ARGUMENT(AQuest*, Quest)
	
	SLATE_END_ARGS()

public:
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);


private:
	void OpenObjectives();
	
	TSharedRef<ITableRow> GenerateObjectiveRow(UObjective* Objective, const TSharedRef<STableViewBase>& Table) const;

	TSharedRef<SWidget> GetCreateObjectiveButton(EObjectiveType Type);

	void AddObjective(EObjectiveType Type);

	AQuest* Quest = nullptr;

	TSharedPtr<SBox> ObjectivesHolder;

	/*TSharedPtr<SBorder> ObjectivesHolder2;*/

	TSharedPtr<SListView<UObjective*>> ObjectiveList;
};

class SObjectiveTableRow : public SMultiColumnTableRow<UObjective*>
{
public:

	SLATE_BEGIN_ARGS(SObjectiveTableRow) {}

		SLATE_ARGUMENT(UObjective*, Objective)

		SLATE_ARGUMENT(AQuest*, Quest)

	SLATE_END_ARGS()

public:
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);

private:

	void RemoveObjective();

	void MoveUp();

	void MoveDown();

	UObjective* Objective = nullptr;
	AQuest* Quest = nullptr;
	STableViewBase* ParentTable = nullptr;
};
