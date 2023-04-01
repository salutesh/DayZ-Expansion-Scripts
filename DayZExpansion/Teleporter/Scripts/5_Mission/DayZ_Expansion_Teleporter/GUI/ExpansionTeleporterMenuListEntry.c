/**
 * ExpansionTeleporterMenuListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTeleporterMenuListEntry: ExpansionScriptView
{
	protected ref ExpansionTeleporterMenuListEntryController m_TeleporterMenuListEntryController;
	protected ref ExpansionTeleportPosition m_TeleportPos;
	protected ref ExpansionTeleporterMenu m_TeleportMenu;
	
	protected bool m_HasCooldown;
	
	protected TextWidget Text;
	protected Widget Background;
	protected ButtonWidget Button;

	void ExpansionTeleporterMenuListEntry(ExpansionTeleportPosition teleportPos, ExpansionTeleporterMenu menu)
	{
		m_TeleportPos = teleportPos;
		m_TeleportMenu = menu;
		
		Class.CastTo(m_TeleporterMenuListEntryController, GetController());
		
		SetEntry();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Teleporter/GUI/layouts/expansion_teleporter_menu_list_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionTeleporterMenuListEntryController;
	}
	
	void SetEntry()
	{
		if (!m_TeleportPos)
			return;
		
		string displayName = m_TeleportPos.GetDisplayName();
	#ifdef EXPANSIONMODAI
		string factionName = m_TeleportPos.GetFaction();
		if (factionName != string.Empty)
			displayName = "[" + factionName + "] " + displayName;
	#endif
		
		if (displayName != string.Empty)
		{
			m_TeleporterMenuListEntryController.EntryText = displayName;
			m_TeleporterMenuListEntryController.NotifyPropertyChanged("EntryText");
		}
	}
	
	void OnEntryClick()
	{
		if (!m_TeleportPos)
			return;
		
		if (!m_HasCooldown)
		{
			m_TeleportMenu.SetTeleportPosition(m_TeleportPos);
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
			case Button:
			{
				Text.SetColor(ARGB(255, 0, 0, 0));
				Background.SetColor(ARGB(200, 220, 220, 220));
				return true;
			}
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
			case Button:
			{
				Text.SetColor(ARGB(255, 220, 220, 220));
				Background.SetColor(ARGB(200, 0, 0, 0));
				return true;
			}
		}

		return false;
	}
};
class ExpansionTeleporterMenuListEntryController: ExpansionViewController 
{
	string EntryIcon;
	string EntryText;
};