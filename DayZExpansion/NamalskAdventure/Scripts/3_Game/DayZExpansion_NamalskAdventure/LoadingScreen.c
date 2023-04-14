/**
 * LoadingScreen.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSION_NAMALSK_ADVENTURE_LOADING_SCREEN
class ExpansionNamalskAdventureLoadingScreenBackground
{
	string Path;
};

class ExpansionNamalskAdventureLoadingScreenBackgrounds
{
	static ref array<ref ExpansionNamalskAdventureLoadingScreenBackground> s_NA_Backgrounds = new array<ref ExpansionNamalskAdventureLoadingScreenBackground>;

	static array<ref ExpansionNamalskAdventureLoadingScreenBackground> Get()
	{
		if (!s_NA_Backgrounds.Count())
			JsonFileLoader<array<ref ExpansionNamalskAdventureLoadingScreenBackground>>.JsonLoadFile("DayZExpansion/NamalskAdventure/Scripts/Data/LoadingImages.json", s_NA_Backgrounds);

		return s_NA_Backgrounds;
	}
};

modded class LoadingScreen
{
	protected const float UPDATE_TICK_TIME = 5.0;
	
	protected autoptr array<ref ExpansionNamalskAdventureLoadingScreenBackground> m_NABackgrounds;
	protected float m_UpdateQueueTimer;

    protected int m_CurrentImageIndex; // current index of the image being displayed
    protected float m_SlideshowImageDuration; // duration of each image in slideshow in seconds
    protected float m_SlideshowImageTimer; // timer for changing images in slideshow
	
	static float s_Expansion_LoadingTime = -1;
	static float s_Expansion_LoadingTimeStamp = -1;

    void LoadingScreen(DayZGame game)
    {
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_NABackgrounds = ExpansionNamalskAdventureLoadingScreenBackgrounds.Get();
		
		DebugTrace("::LoadingScreen - Got " + m_NABackgrounds.Count() + " loading screen images!");
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		int imageCount;
		foreach (ExpansionNamalskAdventureLoadingScreenBackground background: m_NABackgrounds)
		{
			imageCount++;
			DebugTrace("::LoadingScreen - [" + imageCount + "] Path: " + background.Path);	
		}
	#endif
		
        m_CurrentImageIndex = 0;
        m_SlideshowImageDuration = 5.0; // 5 seconds for each image
        m_SlideshowImageTimer = 0;
		
		if (m_WidgetRoot)
			m_WidgetRoot.Unlink();

		m_DayZGame = game;
		m_WidgetRoot = game.GetLoadingWorkspace().CreateWidgets("DayZExpansion/NamalskAdventure/GUI/layouts/loading.layout");
		
		Class.CastTo(m_ImageLogoMid, m_WidgetRoot.FindAnyWidget("ImageLogoMid"));
		Class.CastTo(m_ImageLogoCorner, m_WidgetRoot.FindAnyWidget("ImageLogoCorner"));
		Class.CastTo(m_TextWidgetTitle, m_WidgetRoot.FindAnyWidget("TextWidget"));
		Class.CastTo(m_TextWidgetStatus, m_WidgetRoot.FindAnyWidget("StatusText"));
		Class.CastTo(m_ImageWidgetBackground, m_WidgetRoot.FindAnyWidget("ImageBackground"));
		Class.CastTo(m_ImageLoadingIcon, m_WidgetRoot.FindAnyWidget("ImageLoadingIcon"));
		Class.CastTo(m_ModdedWarning, m_WidgetRoot.FindAnyWidget("ModdedWarning"));

		m_ImageBackground = ImageWidget.Cast(m_WidgetRoot.FindAnyWidget("ImageBackground"));
		m_ProgressLoading = ProgressBarWidget.Cast(m_WidgetRoot.FindAnyWidget("LoadingBar"));

		//! Show/Hide modded game warning
		m_ModdedWarning.Show(false);

		//! Show logo image
		m_ImageLogoMid.Show(false);
		m_ImageLogoCorner.Show(false);

		//! Show background image
		m_ImageWidgetBackground.Show(true);
		m_Counter = 0;

		//! Lighten up your desktop
		game.GetBacklit().LoadingAnim();

		//! Set loadingbar progress
		if (m_ProgressLoading)
			ProgressAsync.SetProgressData(m_ProgressLoading);

		if (m_ImageBackground)
			ProgressAsync.SetUserData(m_ImageBackground);
    }
	
	void UpdateNALoadingBackground()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		float loadingTime = s_Expansion_LoadingTime;
		float tickTime = m_DayZGame.GetTickTime();

		if (s_Expansion_LoadingTimeStamp < 0)
		{
			s_Expansion_LoadingTime = 0;
		}
		else
		{
			s_Expansion_LoadingTime += tickTime - s_Expansion_LoadingTimeStamp;
		}

		s_Expansion_LoadingTimeStamp = tickTime;

		//! Show each loading message and screen at least five seconds
		if (loadingTime > -1 && s_Expansion_LoadingTime < 5)
			return;

		m_CurrentImageIndex = (m_CurrentImageIndex + 1) % m_NABackgrounds.Count();
		for (int i = 0; i < m_NABackgrounds.Count(); i++)
		{
			if (i == m_CurrentImageIndex)
			{
				m_ImageBackground.LoadImageFile(0, m_NABackgrounds[m_CurrentImageIndex].Path);
			}
		}
	}

	override void OnUpdate(float timeslice)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.OnUpdate(timeslice);

		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			if (IsLoading())
				UpdateNALoadingBackground();
			
			m_UpdateQueueTimer = 0;
		}
	}
	
	override void Show()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Widget lIcon = m_ImageBackground;
		Widget pText = m_ProgressLoading;
		m_ProgressLoading.SetCurrent(0.0);
		m_ImageBackground.SetMaskProgress(0.0);
		
		if (!m_WidgetRoot.IsVisible())
		{
			if (m_DayZGame.GetUIManager().IsDialogVisible())
			{
				m_DayZGame.GetUIManager().HideDialog();
			}
			
			if (m_DayZGame.GetMissionState() == DayZGame.MISSION_STATE_MAINMENU)
			{			
				m_ImageWidgetBackground.Show(true);		
				m_TextWidgetStatus.Show(true);
			}
			else
			{
				m_ImageWidgetBackground.Show(true);
				m_TextWidgetStatus.Show(false);
			}
			
			m_WidgetRoot.Show(true);
			m_TextWidgetTitle.SetText("");
			m_TextWidgetStatus.SetText("");
		}
		
		ProgressAsync.SetProgressData(pText);
		ProgressAsync.SetUserData(lIcon);
		
		UpdateNALoadingBackground();
	}
	
	override void Hide(bool force)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (force)
		{
			while (m_Counter > 0)
			{
				Dec();
			}
		}

		if (m_Counter <= 0)
		{
			m_WidgetRoot.Show(false);
			ProgressAsync.SetUserData(null);
			ProgressAsync.SetProgressData(null);
		}
	}
	
	protected void DebugTrace(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};

modded class LoginQueueBase
{
	override Widget Init()
	{
		layoutRoot 		= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/NamalskAdventure/GUI/layouts/queue_position.layout");
		m_txtPosition	= TextWidget.Cast(layoutRoot.FindAnyWidget("txtPosition"));
		m_txtNote 		= TextWidget.Cast(layoutRoot.FindAnyWidget("txtNote"));
		m_btnLeave 		= ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnLeave"));		

		return layoutRoot;
	}
};

modded class LoginTimeBase
{
	override Widget Init()
	{
		layoutRoot 			= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/NamalskAdventure/GUI/layouts/login_time.layout");
		m_txtDescription 	= TextWidget.Cast(layoutRoot.FindAnyWidget("txtDescription"));
		m_txtLabel 			= TextWidget.Cast(layoutRoot.FindAnyWidget("txtLabel"));
		m_btnLeave 			= ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnLeave"));
		
		return layoutRoot;
	}
};
#endif