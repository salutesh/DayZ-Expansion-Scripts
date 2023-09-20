/**
 * ExpansionBookMenuTabFactions.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODBOOK
#ifdef EXPANSIONMODAI
class ExpansionBookMenuTabFactions: ExpansionBookMenuTabBase
{
	ref ExpansionBookMenuTabFactionsController m_FactionTabController;

	void ExpansionBookMenuTabFactions(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		m_FactionTabController = ExpansionBookMenuTabFactionsController.Cast(GetController());
	}
	
	void SetView()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
			return;
	
		if (m_FactionTabController.FactionEntries.Count() > 0)
			m_FactionTabController.FactionEntries.Clear();
			
		foreach (int id, typename factionType: eAIRegisterFaction.s_FactionTypes)
		{
			string displayName;
			int factionRep = player.m_Expansion_HardlineData.GetReputationByFactionID(id);
			if (factionRep > 0)
			{
				eAIFaction faction = eAIFaction.Cast(factionType.Spawn());
				if (faction)
					displayName = faction.GetDisplayName();
			
				faction = null;
				
				ExpansionBookMenuTabFactionEntry factionEntry = new ExpansionBookMenuTabFactionEntry(displayName, factionRep);
				m_FactionTabController.FactionEntries.Insert(factionEntry);
			}
		}
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Hardline/GUI/layouts/tabs/factions/expansion_book_tab_factions.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionBookMenuTabFactionsController;
	}

	override string GetTabIconName()
	{
		return "Territory";
	}

	override string GetTabName()
	{
		return "Factions";
	}

	override int GetTabColor()
	{
		return ARGB(255,0,0,0);
	}

	override void OnShow()
	{
		super.OnShow();
		
		SetView();
	}

	override void OnHide()
	{
		super.OnHide();
		
		if (m_FactionTabController.FactionEntries.Count() > 0)
			m_FactionTabController.FactionEntries.Clear();
	}
	
	override bool CanShow()
	{
		return true;
	}
	
	override bool IsParentTab()
	{
		return true;
	}
};

class ExpansionBookMenuTabFactionsController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionBookMenuTabFactionEntry> FactionEntries = new ObservableCollection<ref ExpansionBookMenuTabFactionEntry>(this);
};

class ExpansionBookMenuTabFactionEntry: ExpansionScriptView
{
	ref ExpansionBookMenuTabFactionEntryController m_FactionEntryController;
	
	string m_FactionName;
	int m_FactionRep;
	
	ProgressBarWidget faction_progress;
	
	void ExpansionBookMenuTabFactionEntry(string name, int rep)
	{
		m_FactionName = name;
		m_FactionRep = rep;
		
		m_FactionEntryController = ExpansionBookMenuTabFactionEntryController.Cast(GetController());
		
		SetEntry();
	}
	
	protected void SetEntry()
	{
		m_FactionEntryController.FactionName = m_FactionName;
		m_FactionEntryController.FactionReputation = m_FactionRep;
		m_FactionEntryController.NotifyPropertiesChanged({"FactionName", "FactionReputation"});
		
		int repPercent = (m_FactionRep * 100) / GetExpansionSettings().GetHardline().MaxReputation;
		faction_progress.SetCurrent(repPercent);
		
		int color = ARGB(150, 220, 0, 0);
		if (repPercent < 100)
		{
			if (repPercent < 50)
			{
				color = ARGB(150, 220, 220, 220);
			}
			else
			{
				color = ARGB(150, 220, 220, 0);
			}
		}
		
		faction_progress.SetColor(color);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Hardline/GUI/layouts/tabs/factions/expansion_book_tab_faction_entry.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabFactionEntryController;
	}
};

class ExpansionBookMenuTabFactionEntryController: ExpansionViewController
{
	string FactionName;
	int FactionReputation;
};
#endif
#endif