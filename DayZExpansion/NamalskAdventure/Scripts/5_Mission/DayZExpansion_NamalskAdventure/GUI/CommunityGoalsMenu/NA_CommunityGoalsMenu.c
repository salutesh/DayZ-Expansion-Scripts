/**
 * NA_CommunityGoalsMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class NA_CommunityGoalsMenu: ExpansionScriptViewMenu
{
	protected ref NA_CommunityGoalsMenuController m_CommunityGoalsMenuController;
	
	protected Widget goals_panel;
	protected Widget contributors_panel;
	protected ButtonWidget exit_button;
	protected TextWidget exit_button_text;
	
	protected GridSpacerWidget back_button_panel;
	protected ButtonWidget back_button;
	protected TextWidget back_button_text
	
	protected int m_SelectedGoalID;

	void NA_CommunityGoalsMenu()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		Class.CastTo(m_CommunityGoalsMenuController, GetController());
		
		ExpansionCommunityGoalsModule.GetModuleInstance().GetCommunityGoalsMenuSI().Insert(SetCommunityGoals);
		ExpansionCommunityGoalsModule.GetModuleInstance().GetCommunityGoalsMenuDetailsSI().Insert(SetCommunityGoalDetails);
	}
	
	void ~NA_CommunityGoalsMenu()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		ExpansionCommunityGoalsModule.GetModuleInstance().GetCommunityGoalsMenuSI().Remove(SetCommunityGoals);
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/NamalskAdventure/GUI/layouts/CommunityGoalsMenu/na_community_goals_menu.layout";
	}

	override typename GetControllerType()
	{
		return NA_CommunityGoalsMenuController;
	}
	
	void SetCommunityGoals(array<ref ExpansionCommunityGoal> goals)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		ExDebugPrint("::SetCommunityGoals - Goals: " + goals.Count());
		
		foreach (ExpansionCommunityGoal goal: goals)
		{
			m_CommunityGoalsMenuController.CommunityGoals.Insert(new NA_CommunityGoalsMenuEntry(this, goal));
		}
	}
	
	void RequestCommunityGoalDetails(int goalID)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		ExDebugPrint("::RequestCommunityGoalDetails - Goal ID: " + goalID);

		m_SelectedGoalID = goalID;
		ExpansionCommunityGoalsModule.GetModuleInstance().RequestCommunityGoalDetails(goalID);
		
		goals_panel.Show(false);
		contributors_panel.Show(true);
		back_button_panel.Show(true);
	}
	
	void SetCommunityGoalDetails(array<ref ExpansionCommunityGoalPlayerData> validContributors)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		ExDebugPrint("::SetCommunityGoalDetails - Contributers: " + validContributors.Count());
		
		m_CommunityGoalsMenuController.CommunityGoalContributers.Clear();
		
		if (m_SelectedGoalID == 0)
			return;
		
		foreach (ExpansionCommunityGoalPlayerData contributor: validContributors)
		{
			if (contributor.GetGoalPoints(m_SelectedGoalID) > 0)
				m_CommunityGoalsMenuController.CommunityGoalContributers.Insert(new NA_CommunityGoalsContributorEntry(contributor, m_SelectedGoalID));
		}
		
		SortContributers();
	}
	
	protected void SortContributers()
	{
		TIntArray points = new TIntArray;
		int i;
		
		for (i = 0; i < m_CommunityGoalsMenuController.CommunityGoalContributers.Count(); i++)
		{
			NA_CommunityGoalsContributorEntry entry = m_CommunityGoalsMenuController.CommunityGoalContributers[i];
			if (!entry)
				continue;

			points.Insert(entry.GetPoints());
		}

		points.Sort(true);

		for (i = 0; i < m_CommunityGoalsMenuController.CommunityGoalContributers.Count(); i++)
		{
			NA_CommunityGoalsContributorEntry currentEntry = m_CommunityGoalsMenuController.CommunityGoalContributers[i];
			if (!currentEntry)
				continue;

			int currentPoints = currentEntry.GetPoints();
			int index = points.Find(currentPoints) + 1;
			currentEntry.SetRank(index);
			currentEntry.SetSort(index, false);
		}
	}
	
	void OnExitClick()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		CloseMenu();
	}
	
	void OnBackClick()
	{
		contributors_panel.Show(false);
		back_button_panel.Show(false);
		goals_panel.Show(true);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		if (w == exit_button)
		{
			exit_button.SetColor(ARGB(200, 220, 220, 220));
			exit_button_text.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == back_button)
		{
			back_button.SetColor(ARGB(200, 220, 220, 220));
			back_button_text.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		if (w == exit_button)
		{
			exit_button.SetColor(ARGB(200, 0, 0, 0));
			exit_button_text.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == back_button)
		{
			back_button.SetColor(ARGB(200, 0, 0, 0));
			back_button_text.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}

		return false;
	}

	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};

class NA_CommunityGoalsMenuController: ExpansionViewController
{
	ref ObservableCollection<ref NA_CommunityGoalsMenuEntry> CommunityGoals = new ObservableCollection<ref NA_CommunityGoalsMenuEntry>(this);
	ref ObservableCollection<ref NA_CommunityGoalsContributorEntry> CommunityGoalContributers = new ObservableCollection<ref NA_CommunityGoalsContributorEntry>(this);
};

class NA_CommunityGoalsMenuEntry: ExpansionScriptView
{
	protected ref NA_CommunityGoalsMenuEntryController m_CommunityGoalsMenuEntryController;

	protected NA_CommunityGoalsMenu m_CommunityGoalsMenu;
	protected ref ExpansionCommunityGoal m_CommunityGoal;
	
	protected ButtonWidget entry_button;
	protected TextWidget name_text;
	protected ProgressBarWidget goal_progress_bar;
	protected GridSpacerWidget panel;
	protected TextWidget goal_progress_value;

	void NA_CommunityGoalsMenuEntry(NA_CommunityGoalsMenu menu, ExpansionCommunityGoal goal)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		Class.CastTo(m_CommunityGoalsMenuEntryController, GetController());
		
		m_CommunityGoalsMenu = menu;
		m_CommunityGoal = goal;
		
		SetView();
	}

	void SetView()
	{
		if (!m_CommunityGoal)
			return;
		
		m_CommunityGoalsMenuEntryController.Name = m_CommunityGoal.GetName();
		m_CommunityGoalsMenuEntryController.NotifyPropertyChanged("Name");
		
		m_CommunityGoalsMenuEntryController.Description = m_CommunityGoal.GetDescription();
		m_CommunityGoalsMenuEntryController.NotifyPropertyChanged("Description");
		
		float progressPercent = (m_CommunityGoal.GetProgress() * 100) / m_CommunityGoal.GetGoal();
		goal_progress_bar.SetCurrent(progressPercent);
		
		m_CommunityGoalsMenuEntryController.ProgressVal = progressPercent.ToString() + "%";
		m_CommunityGoalsMenuEntryController.NotifyPropertyChanged("ProgressVal");
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/NamalskAdventure/GUI/layouts/CommunityGoalsMenu/na_community_goals_menu_entry.layout";
	}

	override typename GetControllerType()
	{
		return NA_CommunityGoalsMenuEntryController;
	}
	
	void OnEntryClick()
	{
		if (!m_CommunityGoalsMenu || !m_CommunityGoal)
			return;

		m_CommunityGoalsMenu.RequestCommunityGoalDetails(m_CommunityGoal.GetID());
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		if (w == entry_button)
		{
			name_text.SetColor(ARGB(255, 220, 220, 220));
			goal_progress_bar.SetColor(ARGB(255, 220, 220, 220));
			goal_progress_value.SetColor(ARGB(255, 206, 55, 87));
			GetLayoutRoot().SetColor(ARGB(255, 206, 55, 87));
			return true;
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		if (w == entry_button)
		{
			name_text.SetColor(ARGB(255, 206, 55, 87));
			goal_progress_bar.SetColor(ARGB(255, 206, 55, 87));
			goal_progress_value.SetColor(ARGB(255, 220, 220, 220));
			GetLayoutRoot().SetColor(ARGB(255, 30, 30, 30));
			return true;
		}

		return false;
	}
};

class NA_CommunityGoalsMenuEntryController: ExpansionViewController
{
	string Name;
	string Description;
	string ProgressVal;
};

class NA_CommunityGoalsContributorEntry: ExpansionScriptView
{
	protected ref NA_CommunityGoalsContributorEntryController m_CommunityGoalsContributorEntryController;
	protected ref ExpansionCommunityGoalPlayerData m_Contributor;
	protected int m_GoalID;
	protected int m_Points;

	void NA_CommunityGoalsContributorEntry(ExpansionCommunityGoalPlayerData contributor, int goalID)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		Class.CastTo(m_CommunityGoalsContributorEntryController, GetController());
		
		m_Contributor = contributor;
		m_GoalID = goalID;
		
		SetView();
	}

	void SetView()
	{
		m_CommunityGoalsContributorEntryController.Name = m_Contributor.GetName();
		m_CommunityGoalsContributorEntryController.NotifyPropertyChanged("Name");
		
		m_Points = m_Contributor.GetGoalPoints(m_GoalID);
		m_CommunityGoalsContributorEntryController.Points = m_Points.ToString();
		m_CommunityGoalsContributorEntryController.NotifyPropertyChanged("Points");
	}
	
	void SetRank(int rank)
	{
		m_CommunityGoalsContributorEntryController.Rank = rank.ToString() + ".";
		m_CommunityGoalsContributorEntryController.NotifyPropertyChanged("Rank");
	}
	
	int GetPoints()
	{
		return m_Points;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/NamalskAdventure/GUI/layouts/CommunityGoalsMenu/na_community_goals_contributor_entry.layout";
	}

	override typename GetControllerType()
	{
		return NA_CommunityGoalsContributorEntryController;
	}
};

class NA_CommunityGoalsContributorEntryController: ExpansionViewController
{
	string Rank;
	string Name;
	string Points;
};