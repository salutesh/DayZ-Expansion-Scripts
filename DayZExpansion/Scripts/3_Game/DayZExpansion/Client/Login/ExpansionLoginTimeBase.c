/**
 * ExpansionLoginTimeBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class LoginTimeBase
{
	protected ImageWidget m_ImageBackground;
	protected autoptr array<ref ExpansionLoadingScreenBackground> m_Backgrounds;

	static float s_Expansion_LoadingTime = -1;
	static float s_Expansion_LoadingTimeStamp = -1;
	bool m_Expansion_Init;
	string m_Expansion_CurrentBackground;
	
	void LoginTimeBase()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "LoginTimeBase");
#endif
		
		m_Backgrounds = ExpansionLoadingScreenBackgrounds.Get();
	}
	
	override Widget Init()
	{	
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "Init");
#endif
		EXPrint(ToString() + "::Init");
		super.Init();

		m_ImageBackground = ImageWidget.Cast(layoutRoot.FindAnyWidget("Background"));

		if (!m_Expansion_Init)
		{
			//! For some reason, LoginQueueStatic::Init gets called twice (not by us, by base game apparently?)
			m_Expansion_Init = true;
			UpdateLoadingBackground(true);
		}
		else if (m_Expansion_CurrentBackground)
		{
			m_ImageBackground.LoadImageFile(0, m_Expansion_CurrentBackground);
		}

		return layoutRoot;
	}

	void UpdateLoadingBackground(bool force = false)
	{
		float loadingTime = s_Expansion_LoadingTime;
		float tickTime = GetGame().GetTickTime();

		if (s_Expansion_LoadingTimeStamp < 0)
		{
			s_Expansion_LoadingTime = 0;
		}
		else
		{
			s_Expansion_LoadingTime += tickTime - s_Expansion_LoadingTimeStamp;
		}

		s_Expansion_LoadingTimeStamp = tickTime;

		//! Show each loading screen at least five seconds
		if (!force && s_Expansion_LoadingTime > -1 && s_Expansion_LoadingTime < 5)
			return;

		//EXPrint(ToString() + "::UpdateLoadingBackground - " + force + " - " + s_Expansion_LoadingTime);
		s_Expansion_LoadingTime = 0;
		
		string world_name = "default";

		//! Game object does not exist yet during parts of the loading process
		if (GetGame())
			world_name = ExpansionStatic.GetCanonicalWorldName();
		
		ExpansionLoadingScreenBackground backgrounds = m_Backgrounds[0];

		for (int i = 0; i < m_Backgrounds.Count(); ++i)
		{
			if (world_name == m_Backgrounds[i].MapName)
			{
				backgrounds = m_Backgrounds[i];
				break;
			}
		}
		
		if (backgrounds)
		{
			m_Expansion_CurrentBackground = backgrounds.GetRandomPath();
			m_ImageBackground.LoadImageFile(0, m_Expansion_CurrentBackground);
		}
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);

		if (m_Expansion_Init && IsVisible())
			UpdateLoadingBackground();
	}
};
