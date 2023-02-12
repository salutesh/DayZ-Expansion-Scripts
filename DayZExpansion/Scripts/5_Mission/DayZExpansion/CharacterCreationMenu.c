/**
 * CharacterCreationMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_MAINMENU_NEW_DISABLE
modded class CharacterCreationMenu
{
	DayZIntroSceneExpansion m_CustomScene;

	void CharacterCreationMenu()
	{
		MissionMainMenu mission = MissionMainMenu.Cast(GetGame().GetMission());
		m_CustomScene = mission.GetIntroSceneExpansion();
	}

	override PlayerBase GetPlayerObj()
	{
		if (m_Scene)
			return m_Scene.GetIntroCharacter().GetCharacterObj();

		if (m_CustomScene)
			return m_CustomScene.GetIntroCharacter().GetCharacterObj();

		return super.GetPlayerObj();
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/new_ui/character_creation/pc/character_creation.layout");

		m_CharacterRotationFrame			= layoutRoot.FindAnyWidget( "character_rotation_frame" );
		m_Apply								= layoutRoot.FindAnyWidget( "apply" );
		m_Save 								= layoutRoot.FindAnyWidget( "save" );
		m_RandomizeCharacter				= layoutRoot.FindAnyWidget( "randomize_character" );
		m_BackButton						= layoutRoot.FindAnyWidget( "back" );
		m_Version							= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		m_DetailsRoot 						= layoutRoot.FindAnyWidget( "menu_details_tooltip" );
		m_DetailsLabel						= TextWidget.Cast( m_DetailsRoot.FindAnyWidget( "menu_details_label" ) );
		m_DetailsText						= RichTextWidget.Cast( m_DetailsRoot.FindAnyWidget( "menu_details_tooltip_content" ) );
		m_CharacterHeaderText 				= TextWidget.Cast(layoutRoot.FindAnyWidget( "char_header_text" ));
		m_PlayedCharacterInfo 				= layoutRoot.FindAnyWidget( "played_char_info" );

		if (m_CustomScene && m_CustomScene.GetIntroCharacter())
		{
			m_OriginalCharacterID = m_CustomScene.GetIntroCharacter().GetCharacterID();
		}

		if (m_Scene && m_Scene.GetIntroCharacter())
		{
			m_OriginalCharacterID = m_Scene.GetIntroCharacter().GetCharacterID();
		}

		if (m_CustomScene)
		{
			m_NameSelector = new OptionSelectorEditbox(layoutRoot.FindAnyWidget("character_name_setting_option"), m_CustomScene.GetIntroCharacter().GetCharacterName(), null, false);
			m_GenderSelector	= new OptionSelectorMultistateCharacterMenu(layoutRoot.FindAnyWidget("character_gender_setting_option"), 0, null, false, m_CustomScene.GetIntroCharacter().GetCharGenderList());
		}

		if (m_Scene)
		{
			m_NameSelector = new OptionSelectorEditbox( layoutRoot.FindAnyWidget("character_name_setting_option"), m_Scene.GetIntroCharacter().GetCharacterName(), null, false );
			m_GenderSelector = new OptionSelectorMultistateCharacterMenu( layoutRoot.FindAnyWidget("character_gender_setting_option"), 0, null, false, m_Scene.GetIntroCharacter().GetCharGenderList() );
		}

		if (m_CustomScene )
		{
			if (m_CustomScene.GetIntroCharacter().IsCharacterFemale())
			{
				m_GenderSelector.SetValue("Female");
				m_SkinSelector	= new OptionSelectorMultistateCharacterMenu(layoutRoot.FindAnyWidget("character_head_setting_option"), 0, null, false, m_CustomScene.GetIntroCharacter().GetCharList(ECharGender.Female));
			}
			else
			{
				m_GenderSelector.SetValue("Male");
				m_SkinSelector	= new OptionSelectorMultistateCharacterMenu(layoutRoot.FindAnyWidget("character_head_setting_option"), 0, null, false, m_CustomScene.GetIntroCharacter().GetCharList(ECharGender.Male));
			}
		}

		if (m_Scene)
		{
			if (m_Scene.GetIntroCharacter().IsCharacterFemale() )
			{
				m_GenderSelector.SetValue("Female");
				m_SkinSelector	= new OptionSelectorMultistateCharacterMenu(layoutRoot.FindAnyWidget("character_head_setting_option"), 0, null, false, m_Scene.GetIntroCharacter().GetCharList(ECharGender.Female));
			}
			else
			{
				m_GenderSelector.SetValue("Male");
				m_SkinSelector = new OptionSelectorMultistateCharacterMenu(layoutRoot.FindAnyWidget("character_head_setting_option"), 0, null, false, m_Scene.GetIntroCharacter().GetCharList(ECharGender.Male));
			}
		}

		m_TopSelector = new OptionSelectorMultistateCharacterMenu(layoutRoot.FindAnyWidget("character_top_setting_option"), 0, null, false, DefaultCharacterCreationMethods.GetConfigAttachmentTypes(InventorySlots.BODY));
		m_BottomSelector = new OptionSelectorMultistateCharacterMenu(layoutRoot.FindAnyWidget("character_bottom_setting_option"), 0, null, false, DefaultCharacterCreationMethods.GetConfigAttachmentTypes(InventorySlots.LEGS));
		m_ShoesSelector = new OptionSelectorMultistateCharacterMenu(layoutRoot.FindAnyWidget("character_shoes_setting_option"), 0, null, false, DefaultCharacterCreationMethods.GetConfigAttachmentTypes(InventorySlots.FEET));

		PlayerBase scene_char = GetPlayerObj();
		if (scene_char)
		{
			Object obj = scene_char.GetInventory().FindAttachment(InventorySlots.BODY);
			if (obj)
				m_TopSelector.SetValue(obj.GetType(), false);

			obj = scene_char.GetInventory().FindAttachment(InventorySlots.LEGS);
			if (obj)
				m_BottomSelector.SetValue(obj.GetType(), false);

			obj = scene_char.GetInventory().FindAttachment(InventorySlots.FEET);
			if (obj)
				m_ShoesSelector.SetValue(obj.GetType(), false);

			m_SkinSelector.SetValue(scene_char.GetType());
		}

		m_GenderSelector.m_OptionChanged.Insert(GenderChanged);
		m_SkinSelector.m_OptionChanged.Insert(SkinChanged);
		m_TopSelector.m_OptionChanged.Insert(TopChanged);
		m_BottomSelector.m_OptionChanged.Insert(BottomChanged);
		m_ShoesSelector.m_OptionChanged.Insert(ShoesChanged);

		Refresh();
		SetCharacter();
		CheckNewOptions();

	#ifdef DAYZ_1_19
		GetGame().GetMission().GetOnInputPresetChanged().Insert(OnInputPresetChanged);
	#endif
		GetGame().GetMission().GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);

	#ifdef DAYZ_1_19
		OnInputDeviceChanged(GetGame().GetInput().GetCurrentInputDevice());
	#endif

		return layoutRoot;
	}

	override void Apply()
	{
		string name;
		if (m_CustomScene && !m_CustomScene.GetIntroCharacter().IsDefaultCharacter() )
		{
			name = m_NameSelector.GetValue();
			if (name == "")
				name = GameConstants.DEFAULT_CHARACTER_NAME;

			m_CustomScene.GetIntroCharacter().SaveCharName(name);
		}

		if (m_Scene && !m_Scene.GetIntroCharacter().IsDefaultCharacter())
		{
			name = m_NameSelector.GetValue();
			if (name == "")
				name = GameConstants.DEFAULT_CHARACTER_NAME;

			m_Scene.GetIntroCharacter().SaveCharName(name);
		}

		MainMenu menu_main = MainMenu.Cast(GetGame().GetUIManager().FindMenu(MENU_MAIN));
		if (menu_main)
			menu_main.OnChangeCharacter(false);

		GetGame().GetUIManager().Back();
	}

	override void Save()
	{
		string name = m_NameSelector.GetValue();
		if (name == "")
			name = GameConstants.DEFAULT_CHARACTER_NAME;

		if (m_CustomScene && m_CustomScene.GetIntroCharacter().IsDefaultCharacter())
		{
			m_CustomScene.GetIntroCharacter().SaveCharName(name);
			m_CustomScene.GetIntroCharacter().SaveDefaultCharacter();
			m_CustomScene.GetIntroCharacter().SetToDefaultCharacter();
			SetCharacterSaved(true);
		}

		if (m_Scene && m_Scene.GetIntroCharacter().IsDefaultCharacter())
		{
			m_Scene.GetIntroCharacter().SaveCharName(name);
			m_Scene.GetIntroCharacter().SaveDefaultCharacter();
			m_Scene.GetIntroCharacter().SetToDefaultCharacter();
			SetCharacterSaved(true);
		}
	}

	override void Back()
	{
		//bring back DefaultCharacter, if it exists (it should), or a previously played one.
		GetGame().GetMenuData().RequestGetDefaultCharacterData();

		if (m_CustomScene)
		{
			m_CustomScene.GetIntroCharacter().SetCharacterID(m_OriginalCharacterID);
			m_CustomScene.GetIntroCharacter().CreateNewCharacterById(m_CustomScene.GetIntroCharacter().GetCharacterID());
		}

		if (m_Scene)
		{
			m_Scene.GetIntroCharacter().SetCharacterID(m_OriginalCharacterID);
			m_Scene.GetIntroCharacter().CreateNewCharacterById(m_Scene.GetIntroCharacter().GetCharacterID());
		}

		GetGame().GetUIManager().Back();
	}

	override void SetCharacter()
	{
		if (m_CustomScene && m_CustomScene.GetIntroCharacter().IsDefaultCharacter())
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_CustomScene.GetIntroCharacter().GetCharacterObj());

		if (m_Scene && m_Scene.GetIntroCharacter().IsDefaultCharacter())
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_Scene.GetIntroCharacter().GetCharacterObj());
	}

	override void RandomizeCharacter()
	{
		if (m_CustomScene)
			m_CustomScene.GetIntroCharacter().SetToDefaultCharacter();

		if (m_Scene)
			m_Scene.GetIntroCharacter().SetToDefaultCharacter();

		// make random selection
		if (m_CustomScene)
			m_CustomScene.GetIntroCharacter().SetCharacterGender(Math.RandomInt(0, 2));

		if (m_Scene)
			m_Scene.GetIntroCharacter().SetCharacterGender( Math.RandomInt(0, 2) );

		if (m_CustomScene)
		{
			if (m_CustomScene.GetIntroCharacter().IsCharacterFemale() )
			{
				m_GenderSelector.SetValue("Female");
				m_SkinSelector.LoadNewValues( m_CustomScene.GetIntroCharacter().GetCharList(ECharGender.Female ), 0);
				m_SkinSelector.SetRandomValue();
			}
			else
			{
				m_GenderSelector.SetValue("Male");
				m_SkinSelector.LoadNewValues(m_CustomScene.GetIntroCharacter().GetCharList(ECharGender.Male ), 0);
				m_SkinSelector.SetRandomValue();
			}
		}

		if (m_Scene)
		{
			if (m_Scene.GetIntroCharacter().IsCharacterFemale() )
			{
				m_GenderSelector.SetValue("Female");
				m_SkinSelector.LoadNewValues(m_Scene.GetIntroCharacter().GetCharList(ECharGender.Female), 0);
				m_SkinSelector.SetRandomValue();
			}
			else
			{
				m_GenderSelector.SetValue("Male");
				m_SkinSelector.LoadNewValues(m_Scene.GetIntroCharacter().GetCharList(ECharGender.Male), 0);
				m_SkinSelector.SetRandomValue();
			}
		}

		GetGame().GetMenuDefaultCharacterData().GenerateRandomEquip();
		m_TopSelector.SetValue(GetGame().GetMenuDefaultCharacterData().GetAttachmentMap().Get(InventorySlots.BODY),false);
		m_BottomSelector.SetValue(GetGame().GetMenuDefaultCharacterData().GetAttachmentMap().Get(InventorySlots.LEGS),false);
		m_ShoesSelector.SetValue(GetGame().GetMenuDefaultCharacterData().GetAttachmentMap().Get(InventorySlots.FEET),false);

		Refresh();
		SetCharacter();
		CheckNewOptions();
	}

	//Selector Events
	override void GenderChanged()
	{
		ECharGender gender = ECharGender.Male;
		if (m_GenderSelector.GetStringValue() == "Female")
			gender = ECharGender.Female;

		if (m_CustomScene)
		{
			m_CustomScene.GetIntroCharacter().SetCharacterGender(gender);
			m_SkinSelector.LoadNewValues(m_CustomScene.GetIntroCharacter().GetCharList(gender) , 0);
		}

		if (m_Scene)
		{
			m_Scene.GetIntroCharacter().SetCharacterGender(gender);
			m_SkinSelector.LoadNewValues(m_Scene.GetIntroCharacter().GetCharList(gender) , 0);
		}

		m_SkinSelector.SetRandomValue();
		SetCharacterSaved(false);
	}

	override void SkinChanged()
	{
		if (m_CustomScene)
			m_CustomScene.GetIntroCharacter().CreateNewCharacterByName(m_SkinSelector.GetStringValue(), false);

		if (m_Scene)
			m_Scene.GetIntroCharacter().CreateNewCharacterByName(m_SkinSelector.GetStringValue(), false);

		SetCharacterSaved(false);
	}

	override void TopChanged()
	{
		GetGame().GetMenuDefaultCharacterData().SetDefaultAttachment(InventorySlots.BODY,m_TopSelector.GetStringValue());
		if (m_CustomScene)
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_CustomScene.GetIntroCharacter().GetCharacterObj());

		if (m_Scene)
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_Scene.GetIntroCharacter().GetCharacterObj());

		SetCharacterSaved(false);
	}

	override void BottomChanged()
	{
		GetGame().GetMenuDefaultCharacterData().SetDefaultAttachment(InventorySlots.LEGS,m_BottomSelector.GetStringValue());
		if (m_CustomScene)
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_CustomScene.GetIntroCharacter().GetCharacterObj());

		if (m_Scene)
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_Scene.GetIntroCharacter().GetCharacterObj());

		SetCharacterSaved(false);
	}

	override void ShoesChanged()
	{
		GetGame().GetMenuDefaultCharacterData().SetDefaultAttachment(InventorySlots.FEET,m_ShoesSelector.GetStringValue());

		if (m_CustomScene)
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_CustomScene.GetIntroCharacter().GetCharacterObj());

		if (m_Scene)
			GetGame().GetMenuDefaultCharacterData().EquipDefaultCharacter(m_Scene.GetIntroCharacter().GetCharacterObj());

		SetCharacterSaved(false);
	}

	override void CheckNewOptions()
	{
		bool show_widgets;
		if (m_CustomScene)
			show_widgets = m_CustomScene.GetIntroCharacter().IsDefaultCharacter();

		if (m_Scene)
			show_widgets = m_Scene.GetIntroCharacter().IsDefaultCharacter();

		bool was_visible = layoutRoot.FindAnyWidget("character_gender_button").IsVisible();
		layoutRoot.FindAnyWidget("character_gender_button").Show(show_widgets);
		layoutRoot.FindAnyWidget("character_head_button").Show(show_widgets);
		layoutRoot.FindAnyWidget("character_top_button").Show(show_widgets);
		layoutRoot.FindAnyWidget("character_bottom_button").Show(show_widgets);
		layoutRoot.FindAnyWidget("character_shoes_button").Show(show_widgets);

		if (!was_visible && show_widgets)
			m_GenderSelector.Focus();

		if (!show_widgets)
			SetFocus(m_RandomizeCharacter);
	}

	override void Refresh()
	{
		string name;
		if (m_CustomScene)
			name = m_CustomScene.GetIntroCharacter().GetCharacterName();

		if (m_Scene)
			name = m_Scene.GetIntroCharacter().GetCharacterName();

		if (name == "")
			name = GameConstants.DEFAULT_CHARACTER_NAME;

		m_NameSelector.SetValue(name);

		string version;
		GetGame().GetVersion( version );
		version = "#main_menu_version" + " " + version;
		if (m_CustomScene)
		{
			m_Apply.Show(!m_CustomScene.GetIntroCharacter().IsDefaultCharacter());
			m_Save.Show(m_CustomScene.GetIntroCharacter().IsDefaultCharacter());
		}

		if (m_Scene)
		{
			m_Apply.Show(!m_Scene.GetIntroCharacter().IsDefaultCharacter());
			m_Save.Show(m_Scene.GetIntroCharacter().IsDefaultCharacter());
		}

		if (m_CustomScene)
		{
			m_PlayedCharacterInfo.Show(!m_CustomScene.GetIntroCharacter().IsDefaultCharacter());
			if (m_CustomScene.GetIntroCharacter().IsDefaultCharacter())
			{
				m_CharacterHeaderText.SetText("#character_menu_header");
			}
			else
			{
				m_CharacterHeaderText.SetText("#server_browser_prev_play_filter");
			}
		}

		if (m_Scene)
		{
			m_PlayedCharacterInfo.Show(!m_Scene.GetIntroCharacter().IsDefaultCharacter());

			if (m_Scene.GetIntroCharacter().IsDefaultCharacter())
			{
				m_CharacterHeaderText.SetText("#character_menu_header");
			}
			else
			{
				m_CharacterHeaderText.SetText("#server_browser_prev_play_filter");
			}
		}

		m_Version.SetText( version );
	}

	override void Update(float timeslice)
	{
		if (GetGame().GetInput().LocalPress("UAUIBack",false))
			Back();

		if (GetGame().GetInput().LocalPress("UAUICtrlX",false))
			RandomizeCharacter();
	}
};
#endif