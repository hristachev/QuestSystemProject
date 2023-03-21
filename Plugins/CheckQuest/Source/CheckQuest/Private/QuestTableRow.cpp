#include "QuestTableRow.h"
#include "CheckQuest.h"
#include "Widgets/SWidget.h"
#include "Widgets/Layout/SWrapBox.h"
#include "ISinglePropertyView.h"
#include "SlateOptMacros.h"
#include <basetyps.h>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<SWidget> SQuestTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FSinglePropertyParams Params;
	Params.NamePlacement = EPropertyNamePlacement::Hidden;

	if (InColumnName == "Name")
	{
		return SNew(SBox)
			[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, Name), Params).ToSharedRef()	
			];
	}
	if (InColumnName == "Description")
	{
		return SNew(SBox)
			[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, Description), Params).ToSharedRef()	
			];
	}
	if (InColumnName == "IsStory")
	{
		return SNew(SBox)
			[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, bIsStoryQuest), Params).ToSharedRef()	
			];
	}
	if (InColumnName == "Objectives")
	{
		return SNew(SBox)
			[
				SNew(SButton)
				.OnPressed_Raw(this, &SQuestTableRow::OpenObjectives)
				[
					SNew(SBox)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Show Objectives"))
					]
				]
			];
	}
	if (InColumnName == "KeepOrder")
	{
		return SNew(SBox)
			[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, bKeepObjectivesOrder), Params).ToSharedRef()
			];
	}
	if (InColumnName == "PreviousQuest")
	{
		return SNew(SBox)
			[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, PreviousQuest), Params).ToSharedRef()
			];
	}

	return SNew(SBox)[SNew(STextBlock).Text(FText::FromString("Wrong Field"))];
}


void SQuestTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	Quest = InArgs._Quest;
	ObjectivesHolder = InArgs._ObjectivesHolder;
	FSuperRowType::Construct(FSuperRowType::FArguments(), InOwnerTableView);
}

void SQuestTableRow::OpenObjectives()
{
	ObjectivesHolder->SetContent(
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		[
			SNew(SWrapBox)
			.Orientation(Orient_Horizontal)
			+SWrapBox::Slot()[GetCreateObjectiveButton(EObjectiveType::Ot_Interact)]
			+SWrapBox::Slot()[GetCreateObjectiveButton(EObjectiveType::Ot_Location)]
			+SWrapBox::Slot()[GetCreateObjectiveButton(EObjectiveType::Ot_Collect)]
			+SWrapBox::Slot()[GetCreateObjectiveButton(EObjectiveType::Ot_Kill)] 
		]
		+ SVerticalBox::Slot()
		[
			 SNew(SBox)
			 [
				SAssignNew(ObjectiveList, SListView<UObjective*>)
				.ItemHeight(24)
				.ListItemsSource(&Quest->GetObjectives())
				.OnGenerateRow_Raw(this, &SQuestTableRow::GenerateObjectiveRow)
				.HeaderRow
				(
					SNew(SHeaderRow)
					+ SHeaderRow::Column("Description")[FCheckQuestModule::GetRowHeader("Description")]
					+ SHeaderRow::Column("Type")[FCheckQuestModule::GetRowHeader("Type")]
					+ SHeaderRow::Column("Target")[FCheckQuestModule::GetRowHeader("Target")]
					+ SHeaderRow::Column("Delete")[FCheckQuestModule::GetRowHeader("Delete")]
					+ SHeaderRow::Column("Move")[FCheckQuestModule::GetRowHeader("Move")]
				)

			 ]
		]
	);
}

TSharedRef<ITableRow> SQuestTableRow::GenerateObjectiveRow(UObjective* Objective,
	const TSharedRef<STableViewBase>& Table) const
{
	return SNew(SObjectiveTableRow, Table)
	.Objective(Objective)
	.Quest(Quest);
}

TSharedRef<SWidget> SQuestTableRow::GetCreateObjectiveButton(EObjectiveType Type)
{
	FString TypeName;
	switch (Type)
	{
	case EObjectiveType::Ot_Interact: TypeName = "Interact"; break;
	case EObjectiveType::Ot_Collect: TypeName = "Collect"; break;
	case EObjectiveType::Ot_Kill: TypeName = "Kill"; break;
	case EObjectiveType::Ot_Location: TypeName = "Location"; break;
	default: return SNullWidget::NullWidget;	
	}

	return SNew(SButton)
	.OnPressed_Raw(this, &SQuestTableRow::AddObjective, Type)
	[
		SNew(STextBlock)
		.Text(FText::FromString("Add " + TypeName + " Objective"))
	];
}

void SQuestTableRow::AddObjective(EObjectiveType Type)
{
	switch (Type)
	{
	case EObjectiveType::Ot_Interact: Quest->AddInteractObjective(); break;
	case EObjectiveType::Ot_Collect: Quest->AddCollectObjective(); break;
	case EObjectiveType::Ot_Kill: Quest->AddKillObjective(); break;
	case EObjectiveType::Ot_Location: Quest->AddLocationObjective(); break;
		default: return;
	}

	ObjectiveList->RequestListRefresh();
}

TSharedRef<SWidget> SObjectiveTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FSinglePropertyParams Params;
	Params.NamePlacement = EPropertyNamePlacement::Hidden;
	
	if (InColumnName == "Description")
	{
		return SNew(SBox)
			[
				PropertyModule.CreateSingleProperty(Objective, GET_MEMBER_NAME_CHECKED(UObjective, Description), Params).ToSharedRef()	
			];
	}
	if (InColumnName == "Type")
	{
		return SNew(SBox)
			.IsEnabled(false)
			[
				PropertyModule.CreateSingleProperty(Objective, GET_MEMBER_NAME_CHECKED(UObjective, Type), Params).ToSharedRef()	
			];
	}
	if (InColumnName == "Target")
	{
		return SNew(SBox)
			[
				PropertyModule.CreateSingleProperty(Objective, Objective->WorldTargetName, Params).ToSharedRef()
			];
	}
	if (InColumnName == "Delete")
	{
		return SNew(SBox)
			[
				SNew(SButton)
				.OnPressed_Raw(this, &SObjectiveTableRow::RemoveObjective)
				[
					SNew(SBox)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Delete Objective"))
					]
				]
			];
	}
	if (InColumnName == "Move")
	{
		return SNew(SBox)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SButton)
					.OnPressed_Raw(this, &SObjectiveTableRow::MoveUp)
					.IsEnabled(Quest->Objectives[0] != Objective)
					[
						SNew(SBox)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Up"))
						]
					]
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SButton)
					.OnPressed_Raw(this, &SObjectiveTableRow::MoveDown)
					.IsEnabled(Quest->Objectives.Last() != Objective)
					[
						SNew(SBox)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Down"))
						]
					]
				]
			];
	}

	return SNew(SBox)[SNew(STextBlock).Text(FText::FromString("Wrong Field"))];
}

void SObjectiveTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	Objective = InArgs._Objective;
	Quest = InArgs._Quest;
	FSuperRowType::Construct(FSuperRowType::FArguments(), InOwnerTableView);
	ParentTable = &InOwnerTableView.Get();
}

void SObjectiveTableRow::RemoveObjective()
{
	Quest->Objectives.Remove(Objective);
	Quest->Modify();
	ParentTable->RequestListRefresh();
}

void SObjectiveTableRow::MoveUp()
{
	const int32 Index = Quest->Objectives.Find(Objective);
	if (Index == 0)
	{
		return;
	}

	Quest->Objectives.Swap(Index, Index - 1);
	Quest->Modify();
	ParentTable->RebuildList();
}

void SObjectiveTableRow::MoveDown()
{
	const int32 Index = Quest->Objectives.Find(Objective);
	if (Index == Quest->Objectives.Num() - 1)
	{
		return;
	}

	Quest->Objectives.Swap(Index, Index + 1);
	Quest->Modify();
	ParentTable->RebuildList();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION