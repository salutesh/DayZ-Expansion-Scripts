/**
 * ExpansionPlayerListMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPlayerListMenu: ExpansionScriptViewMenu
{
	private ref ExpansionPlayerListMenuController m_MenuController;
	
	void ExpansionPlayerListMenu()
	{
		Class.CastTo(m_MenuController, GetController());
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/PlayerList/GUI/layouts/expansion_player_list.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionPlayerListMenuController;
	}
	
	void PopulatePlayerList()
	{
		if (!m_MenuController)
			return;
		
		m_MenuController.PlayerList.Clear();

		foreach (SyncPlayer player: ClientData.m_PlayerList.m_PlayerList) 
		{
			if (!player)
				continue;
			
			ExpansionPlayerListEntry entry = new ExpansionPlayerListEntry(player.m_PlayerName, player.m_RUID, player.m_UID);
			m_MenuController.PlayerList.Insert(entry);
		}
						
		m_MenuController.PlayerCount = m_MenuController.PlayerList.Count();
		m_MenuController.NotifyPropertyChanged("PlayerCount");
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		GetUApi().GetInputByName("UAExpansionPlayerListToggle").ForceDisable(false);		
		GetUApi().UpdateControls();
		
		PopulatePlayerList();
	}
	
	override void LockControls(bool lockMovement = false)
	{
		super.LockControls(false);
	}
};