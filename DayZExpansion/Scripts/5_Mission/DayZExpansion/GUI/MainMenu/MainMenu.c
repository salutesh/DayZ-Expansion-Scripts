/**
 * MainMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MainMenu
{
	override Widget Init()
	{
		layoutRoot = super.Init();

		if (GetExpansionSettings().GetGeneral().UseExpansionMainMenuLogo)
		{
			ImageWidget dayZLogo = ImageWidget.Cast(layoutRoot.FindAnyWidget("dayz_logo"));
			dayZLogo.LoadImageFile(0, "set:expansion_iconset image:logo_expansion_white");
			dayZLogo.SetFlags(dayZLogo.GetFlags() | WidgetFlags.STRETCH);
		}

		if (GetExpansionSettings().GetGeneral().UseExpansionMainMenuIcons)
		{
			ImageWidget tutorialButtonImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("tutorial_button_image"));
			tutorialButtonImage.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\hud\\questionmark_64x64.edds");
			tutorialButtonImage.SetFlags(tutorialButtonImage.GetFlags() | WidgetFlags.STRETCH);

			ImageWidget creditsButtonImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("message_button_image"));
			creditsButtonImage.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\hud\\menu_64x64.edds");
			creditsButtonImage.SetFlags(creditsButtonImage.GetFlags() | WidgetFlags.STRETCH);

			ImageWidget settingsButtonImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("settings_button_image"));
			settingsButtonImage.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\hud\\toothwheels_64x64.edds");
			settingsButtonImage.SetFlags(settingsButtonImage.GetFlags() | WidgetFlags.STRETCH);

			ImageWidget exitButtonImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("exit_button_image"));
			exitButtonImage.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\hud\\switch_64x64.edds");
			exitButtonImage.SetFlags(exitButtonImage.GetFlags() | WidgetFlags.STRETCH);
		}

		return layoutRoot;
	}

	override void Refresh()
	{
		super.Refresh();

		GetDayZGame().Expansion_SetGameVersionText(m_Version);
	}

	override void OnChangeCharacter(bool create_character = true)
	{
		super.OnChangeCharacter(create_character);

		if (m_ScenePC && m_ScenePC.Expansion_IsCustomScene())
			m_ScenePC.Expansion_SetupCharacter();
	}
};
