/**
 * LoginScreenBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef NAMALSK_ISLAND
class CustomLoadingScreenBackground
{
	string ImagePath;
};

class CustomLoadingScreenData
{
	bool ShowLogo = true;
	string LogoPath = "DayZExpansion/NamalskAdventure/GUI/logo.edds";
	bool RandomizeBackgounds = true;
	int LoadingBarColor = ARGB(255, 63, 212, 252);
	bool UseCustomHints = false;
	string HintIconPath = "set:expansion_inventory image:anomaly";

	ref array<ref CustomLoadingScreenBackground> m_ExBackgrounds;

	void CustomLoadingScreenData()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_ExBackgrounds = new array<ref CustomLoadingScreenBackground>;
		JsonFileLoader<array<ref CustomLoadingScreenBackground>>.JsonLoadFile("DayZExpansion/NamalskAdventure/Scripts/Data/LoadingImages.json", m_ExBackgrounds);
	}
};

modded class UiHintPanel
{
	protected const string 	m_ExDataPath = "DayZExpansion/NamalskAdventure/Scripts/Data/Hints.json"; //! Custom hints json path
	protected ref CustomLoadingScreenData m_ExCustomLoadingScreenData;

	override protected void LoadContentList()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!m_ExCustomLoadingScreenData)
			m_ExCustomLoadingScreenData = new CustomLoadingScreenData();

		if (m_ExCustomLoadingScreenData && m_ExCustomLoadingScreenData.UseCustomHints)
		{
			JsonFileLoader<array<ref HintPage>>.JsonLoadFile(m_ExDataPath, m_ContentList);
		}
		else
		{
			JsonFileLoader<array<ref HintPage>>.JsonLoadFile(m_DataPath, m_ContentList);
		}
	}
};

modded class LoginScreenBase
{
	protected ImageWidget m_ExHintIcon;
	protected int m_ExBackgroundIndex = 0;

	protected const float LOADING_SCREEN_CHANGE_TIME = 3.0;
	protected float m_ExLoadingTime;

	protected ImageWidget m_ExBackground;

	protected ref CustomLoadingScreenData m_ExCustomLoadingScreenData;
	protected ref array<int> m_ShownBackgrounds;

	void LoginScreenBase()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_ExCustomLoadingScreenData = new CustomLoadingScreenData();

		if (!m_ExCustomLoadingScreenData)
			return;

		if (m_ExCustomLoadingScreenData.RandomizeBackgounds)
			m_ShownBackgrounds = new array<int>;
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);

		m_ExLoadingTime += timeslice;
		if (m_ExLoadingTime >= LOADING_SCREEN_CHANGE_TIME)
		{
			if (!m_ExCustomLoadingScreenData.RandomizeBackgounds)
			{
				SetBackgroundImage(m_ExBackgroundIndex);
				m_ExBackgroundIndex++;
				if (m_ExBackgroundIndex > (m_ExCustomLoadingScreenData.m_ExBackgrounds.Count() - 1))
					m_ExBackgroundIndex = 0;
			}
			else
			{
				if (m_ShownBackgrounds.Count() < m_ExCustomLoadingScreenData.m_ExBackgrounds.Count())
				{
					m_ExBackgroundIndex = GetRandomBackroundIndex();
					SetBackgroundImage(m_ExBackgroundIndex);
					m_ShownBackgrounds.Insert(m_ExBackgroundIndex);
				}
				else
				{
					m_ShownBackgrounds.Clear();
				}

			}

			m_ExLoadingTime = 0;
		}
	}

	protected int GetRandomBackroundIndex()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		int index = Math.RandomIntInclusive(0, (m_ExCustomLoadingScreenData.m_ExBackgrounds.Count() - 1));
		int searchCount;
		while ((m_ShownBackgrounds.Find(index) > -1) && (searchCount < m_ShownBackgrounds.Count()))
		{
			index = Math.RandomIntInclusive(0, (m_ExCustomLoadingScreenData.m_ExBackgrounds.Count() - 1));
		}

		return index;
	}

	protected void SetBackgroundImage(int index)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		CustomLoadingScreenBackground background = m_ExCustomLoadingScreenData.m_ExBackgrounds.Get(index);
		if (background && background.ImagePath != string.Empty)
		{
			Print(ToString() + "::SetBackgroundImage - Background: " + background.ImagePath);
			m_ExBackground.LoadImageFile(0, background.ImagePath, true);
			m_ExBackground.SetImage(0);
		}
	}
};

modded class LoginQueueBase
{
	protected Widget m_ExSeperatorLine;
	protected ImageWidget m_ExHintIcon;
	
	override Widget Init()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		layoutRoot = super.Init();

		m_ExSeperatorLine = layoutRoot.FindAnyWidget("SeparatorPanel");
		m_ExSeperatorLine.SetColor(ARGB(255, 63, 212, 252));
		m_ExBackground = ImageWidget.Cast(layoutRoot.FindAnyWidget("Background"));
		m_btnLeave.SetColor(ARGB(255, 63, 212, 252));
		
		m_ExHintIcon = ImageWidget.Cast(layoutRoot.FindAnyWidget("hintIcon"));
		m_ExHintIcon.LoadImageFile(0, m_ExCustomLoadingScreenData.HintIconPath);
		
		SetBackgroundImage(m_ExBackgroundIndex);
		m_ExBackgroundIndex++;

		return layoutRoot;
	}
};

modded class LoginTimeBase
{
	protected Widget m_ExSeperatorLine;
	protected ImageWidget m_ExHintIcon;
	
	override Widget Init()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		layoutRoot = super.Init();
		
		m_ExSeperatorLine = layoutRoot.FindAnyWidget("SeparatorPanel");
		m_ExSeperatorLine.SetColor(ARGB(255, 63, 212, 252));
		m_ExBackground = ImageWidget.Cast(layoutRoot.FindAnyWidget("Background"));
		m_btnLeave.SetColor(ARGB(255, 63, 212, 252));
		
		m_ExHintIcon = ImageWidget.Cast(layoutRoot.FindAnyWidget("hintIcon"));
		m_ExHintIcon.LoadImageFile(0, m_ExCustomLoadingScreenData.HintIconPath);
		
		SetBackgroundImage(m_ExBackgroundIndex);
		m_ExBackgroundIndex++;

		return layoutRoot;
	}
};

modded class LoadingScreen
{
	protected const string LOADING_SCREEN_LAYOUTS = "DayZExpansion/NamalskAdventure/GUI/layouts/";
	
	protected Widget m_ExPanelWidget;
	protected ImageWidget m_VanillaHintIcon;
	protected ImageWidget m_ExHintIcon;
	protected int m_ExBackgroundIndex = 0;

	protected const float LOADING_SCREEN_CHANGE_TIME = 2.5;
	protected float m_ExLoadingTime;
	protected float m_ExLoadingTimeStamp;

	protected ref CustomLoadingScreenData m_ExCustomLoadingScreenData;
	protected ref array<int> m_ShownBackgrounds;

	void LoadingScreen(DayZGame game)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_ExCustomLoadingScreenData = new CustomLoadingScreenData();
		if (!m_ExCustomLoadingScreenData)
			return;

		if (m_ExCustomLoadingScreenData.RandomizeBackgounds)
			m_ShownBackgrounds = new array<int>;

		m_ExPanelWidget = Widget.Cast(m_WidgetRoot.FindAnyWidget("PanelWidget1"));

		//! Hide vanilla hint icon
		float posHintIconX, posHintIconY;
		m_VanillaHintIcon = ImageWidget.Cast(m_WidgetRoot.FindAnyWidget("hintIcon"));
		m_VanillaHintIcon.GetScreenPos(posHintIconX, posHintIconY);
		m_VanillaHintIcon.Show(false);
		m_VanillaHintIcon.Unlink();

		//! Create custom hint icon
		m_ExHintIcon = ImageWidget.Cast(game.GetLoadingWorkspace().CreateWidgets(LOADING_SCREEN_LAYOUTS + "hint_icon.layout", m_ExPanelWidget));
		m_ExHintIcon.SetScreenPos(posHintIconX, posHintIconY, true);
		m_ExHintIcon.LoadImageFile(0, m_ExCustomLoadingScreenData.HintIconPath);

		//! Remove vanilla mask texture
		m_ImageWidgetBackground.LoadMaskTexture("");

		//! Replace and set logo
		if (m_ExCustomLoadingScreenData.ShowLogo && m_ExCustomLoadingScreenData.LogoPath != string.Empty)
		{
			float posLogoX, posLogoY;
			m_ImageLogoCorner.GetScreenPos(posLogoX, posLogoY);
			m_ImageLogoCorner.Show(false);
			m_ImageLogoCorner.Unlink();

			m_ImageLogoCorner = ImageWidget.Cast(game.GetLoadingWorkspace().CreateWidgets(LOADING_SCREEN_LAYOUTS + "logo.layout", m_ExPanelWidget));
			m_ImageLogoCorner.SetScreenPos(posLogoX, posLogoY, true);
			m_ImageLogoCorner.LoadImageFile(0, m_ExCustomLoadingScreenData.LogoPath);
			m_ImageLogoMid.Show(false);
		}
		//! Hide vanilla logo
		else if (!m_ExCustomLoadingScreenData.ShowLogo)
		{
			Print(ToString() + "::LoadingScreen - Hide logo!");
			m_ImageLogoCorner.Show(false);
			m_ImageLogoMid.Show(false);
		}

		if (m_ExCustomLoadingScreenData.LoadingBarColor != 0)
		{
			float posLoadingBarX, posLoadingBarY;
			m_ProgressLoading.GetScreenPos(posLoadingBarX, posLoadingBarY);
			m_ProgressLoading.Show(false);
			m_ProgressLoading.Unlink();

			m_ProgressLoading = ProgressBarWidget.Cast(game.GetLoadingWorkspace().CreateWidgets(LOADING_SCREEN_LAYOUTS + "loading_bar.layout", m_ExPanelWidget));
			m_ProgressLoading.SetScreenPos(posLoadingBarX, posLoadingBarY, true);
			m_ProgressLoading.SetColor(m_ExCustomLoadingScreenData.LoadingBarColor);
			ProgressAsync.SetProgressData(m_ProgressLoading);

			Print(ToString() + "::LoadingScreen - New progress bar: " + m_ProgressLoading.ToString());
		}
	}
	
	override void Show()
	{
		Widget lIcon = m_ImageBackground;
		Widget pText = m_ProgressLoading;
		m_ProgressText.SetText("");
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
				m_ImageLogoMid.Show(false);
				m_ImageLogoCorner.Show(false);				
				m_ImageWidgetBackground.Show(true);		
				m_TextWidgetStatus.Show(true);
			}
			else
			{
				m_ImageLogoMid.Show(false);
				m_ImageLogoCorner.Show(true);
				m_ImageWidgetBackground.Show(true);
				m_TextWidgetStatus.Show(false);
			}
			
			m_WidgetRoot.Show(true);
			m_TextWidgetTitle.SetText("");
			m_TextWidgetStatus.SetText("");
		}
		
		ProgressAsync.SetProgressData(pText);
		ProgressAsync.SetUserData(lIcon);
	}

	//! Method called when loading progress has finished.
	override void Dec()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.Dec();
	}

	override void EndLoading()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.EndLoading();
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		m_ExLoadingTime += timeslice;
		if (m_ExLoadingTime >= LOADING_SCREEN_CHANGE_TIME)
		{
			SetBackgroundImage(m_ExBackgroundIndex);
			m_ExBackgroundIndex++;
			if (m_ExBackgroundIndex > (m_ExCustomLoadingScreenData.m_ExBackgrounds.Count() - 1))
				m_ExBackgroundIndex = 0;

			m_ExLoadingTime = 0;
		}
	}

	override void ShowEx(DayZGame game)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.ShowEx(game);

		float loadingTime = m_ExLoadingTime;
		float tickTime = game.GetTickTime();

		if (m_ExLoadingTimeStamp < 0)
		{
			m_ExLoadingTime = 0;
		}
		else
		{
			m_ExLoadingTime += tickTime - m_ExLoadingTimeStamp;
		}

		m_ExLoadingTimeStamp = tickTime;

		if (m_ExLoadingTime >= LOADING_SCREEN_CHANGE_TIME)
		{
			SetBackgroundImage(m_ExBackgroundIndex);
			m_ExBackgroundIndex++;
			if (m_ExBackgroundIndex > (m_ExCustomLoadingScreenData.m_ExBackgrounds.Count() - 1))
				m_ExBackgroundIndex = 0;

			m_ExLoadingTime = 0;
		}
	}

	protected void SetBackgroundImage(int index)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		CustomLoadingScreenBackground background = m_ExCustomLoadingScreenData.m_ExBackgrounds.Get(index);
		if (background && background.ImagePath != string.Empty)
		{
			ExDebugPrint("::SetBackgroundImage - Background: " + background.ImagePath);
			m_ImageWidgetBackground.LoadImageFile(0, background.ImagePath, true);
			m_ImageWidgetBackground.SetImage(0);
		}
	}
	
	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};
#endif
