/**
 * ExpansionBookMenuTabTerritoryMemberEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODBASEBUILDING
class ExpansionBookMenuTabTerritoryMemberEntry: ExpansionScriptView
{
	ref ExpansionTerritoryModule m_TerritoryModule;
	ref ExpansionBookMenuTabTerritory m_TerritoryTab;
	ref ExpansionTerritoryMember m_Member;
	ref ExpansionTerritory m_Territroy;
	ref ExpansionBookMenuTabTerritoryMemberEntryController m_EntryController;
	
	private TextWidget member_entry_label;
	private ImageWidget member_entry_icon;
	private ButtonWidget member_entry_button;
	
	void ExpansionBookMenuTabTerritoryMemberEntry(ExpansionBookMenuTabTerritory tab, ExpansionTerritoryMember member, ExpansionTerritory territory)
	{
		m_TerritoryTab = tab;
		m_Member = member;
		m_Territroy = territory;
		
		if (!m_EntryController)
			m_EntryController = ExpansionBookMenuTabTerritoryMemberEntryController.Cast(GetController());
		
		if (!m_TerritoryModule)
			m_TerritoryModule = ExpansionTerritoryModule.Cast(CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
		
		SetEntry(m_Member);
	}
	
	void SetEntry(ExpansionTerritoryMember member)
	{
		m_EntryController.MemberName = member.GetName();
		m_EntryController.NotifyPropertyChanged("MemberName");
		
		if (PlayerBase.Expansion_IsOnline(m_Member.GetID()))
		{
			member_entry_icon.SetColor(ARGB(255, 22, 160, 133));
		} 
		else 
		{
			member_entry_icon.SetColor(ARGB(255, 192, 57, 43));
		}
		
		m_EntryController.Background = GetRandomElementBackground();
		m_EntryController.NotifyPropertyChanged("Background");
	}
	
	string GetRandomElementBackground()
	{
		TStringArray backgrounds = new TStringArray;
		
		backgrounds.Insert("set:book_assets image:menu_element_1");
		backgrounds.Insert("set:book_assets image:menu_element_2");
		backgrounds.Insert("set:book_assets image:menu_element_3");
		backgrounds.Insert("set:book_assets image:menu_element_4");
		backgrounds.Insert("set:book_assets image:menu_element_5");
		backgrounds.Insert("set:book_assets image:menu_element_6");
		
		return backgrounds.GetRandomElement();
	}
	
	void OnEditButtonClick()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());		
		ExpansionTerritory player_territory = m_TerritoryModule.GetTerritory(player.GetTerritoryIDInside());
		ExpansionTerritoryMember player_member = player_territory.GetMember(player.GetIdentity().GetId());
		
		if (!player_member)
			return;
		
		if (player_member.GetRank() == ExpansionTerritoryRank.MEMBER)
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "You have no rights to edit any territory members.", "Questionmark", ARGB(255, 255, 255, 255), 5).Create();
			return;
		}
		
		if (player_member.GetRank() == ExpansionTerritoryRank.MODERATOR || player_member.GetRank() == ExpansionTerritoryRank.ADMIN)
		{
			if (m_TerritoryTab && m_Member)
			{
				m_TerritoryTab.Hide();
				m_TerritoryTab.GetMemberEditTab().SetMember(m_Member);
				m_TerritoryTab.GetMemberEditTab().SetTerritory(m_Territroy);
				m_TerritoryTab.GetMemberEditTab().Show();
			}
		}
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/territory/expansion_book_tab_territory_member_entry.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabTerritoryMemberEntryController;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == member_entry_button)
		{
			member_entry_label.SetColor(ARGB(255, 220, 220, 220));
			member_entry_icon.SetColor(ARGB(255, 220, 220, 220));
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == member_entry_button)
		{
			member_entry_label.SetColor(ARGB(255, 0, 0, 0));
			
			if (PlayerBase.Expansion_IsOnline(m_Member.GetID()))
			{
				member_entry_icon.SetColor(ARGB(255, 22, 160, 133));
			} 
			else 
			{
				member_entry_icon.SetColor(ARGB(255, 192, 57, 43));
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
};

class ExpansionBookMenuTabTerritoryMemberEntryController: ExpansionViewController
{
	string MemberName;
	string Background;
};
#endif
