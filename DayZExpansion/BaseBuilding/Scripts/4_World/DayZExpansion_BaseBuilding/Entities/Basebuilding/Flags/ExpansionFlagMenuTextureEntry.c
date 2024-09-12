/**
 * ExpansionFlagMenuTextureEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionFlagMenuTextureEntry
 * @brief		
 **/

class ExpansionFlagMenuTextureEntry: ScriptedWidgetEventHandler
{
	protected GridSpacerWidget m_Root;
	protected ButtonWidget m_Button;
	protected ImageWidget m_Icon;
	protected TextWidget m_Name;
	protected autoptr ExpansionFlagTexture m_Texture;
	protected ExpansionFlagMenu m_FlagMenu;
	string m_NameText;

	void ExpansionFlagMenuTextureEntry(Widget parent, ExpansionFlagTexture texture)
	{
		m_Root = GridSpacerWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/BaseBuilding/GUI/layouts/flag/expansion_flag_menu_texture_entry.layout", parent ) );
		m_Button = ButtonWidget.Cast( m_Root.FindAnyWidget("texture_entry_button") );
		m_Icon = ImageWidget.Cast( m_Root.FindAnyWidget("texture_icon") );
		m_Name = TextWidget.Cast( m_Root.FindAnyWidget("texture_name") );
		
		m_Texture = texture;
		
		m_Button.SetHandler(this);

		SetEntry();
	}

	void ~ExpansionFlagMenuTextureEntry()
	{
		if (g_Game && m_Root)
			m_Root.Unlink();
	}

	void SetEntry()
	{
		m_Icon.LoadImageFile( 0, GetTextureData().TexturePath );
		m_Icon.Show( true );

		m_NameText = GetTextureData().Name;
		m_Name.SetText( m_NameText );
	}

	ExpansionFlagTexture GetTextureData()
	{
		return m_Texture;
	}
	
	string GetName()
	{
		return m_NameText;
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.TERRITORY, this, "OnMouseButtonUp");
#endif

		if (w == m_Button)
		{
			if (!m_FlagMenu)
				m_FlagMenu = ExpansionFlagMenu.Cast( GetGame().GetUIManager().GetMenu() );
			
			m_FlagMenu.SetCurrentTexture( GetTextureData() );
			return true;
		}
		return false;
	}
	
	void Show()
	{
		m_Root.Show(true);
	}
	
	void Hide()
	{
		m_Root.Show(false);
	}
	
	bool IsVisible()
	{
		return m_Root.IsVisible();
	}
};
