/**
 * ExpansionPlayerListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPlayerListEntry: ExpansionScriptView
{	
	private ImageWidget player_icon;
	private TextWidget player_name;
	private string m_Name;
	private string m_PlayerID;
	private string m_PlayerPlainID;
	private ref ExpansionTooltipPlayerListEntry m_Tooltip;
	private ref ExpansionPlayerListEntryController m_EntryController;
	
	void ExpansionPlayerListEntry(string name, string playerID, string plainID)
	{
		Class.CastTo(m_EntryController, GetController());
		SetName(name);
		SetID(playerID, plainID);
	}
	
	void ~ExpansionPlayerListEntry()
	{
		if (m_Tooltip)
			m_Tooltip.Destroy();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/PlayerList/GUI/layouts/expansion_player_list_entry.layout";
	}
	
	override typename GetControllerType()
	{
		return ExpansionPlayerListEntryController;
	}
	
	void SetName(string name)
	{
		if (!m_EntryController)
			return;

		m_EntryController.PlayerName = name;
		m_EntryController.NotifyPropertyChanged("PlayerName");
		
		m_Name = name;
	}
	
	void SetID(string id, string plainID)
	{		
		m_PlayerID = id;
		m_PlayerPlainID = plainID;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == GetLayoutRoot())
		{
			player_name.SetColor(ARGB(255,1,1,1));
			player_icon.SetColor(ARGB(255,1,1,1));
			
			if (GetExpansionSettings().GetPlayerList().EnableTooltip)
			{
				if (!m_Tooltip)
					m_Tooltip = new ExpansionTooltipPlayerListEntry(m_Name, m_PlayerID, m_PlayerPlainID);
				else
					m_Tooltip.Show();
			}
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == GetLayoutRoot())
		{
			player_name.SetColor(ARGB(255,220,220,220));
			player_icon.SetColor(ARGB(255,220,220,220));
			
			if (GetExpansionSettings().GetPlayerList().EnableTooltip)
			{
				if (m_Tooltip)
					m_Tooltip.Hide();
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
};