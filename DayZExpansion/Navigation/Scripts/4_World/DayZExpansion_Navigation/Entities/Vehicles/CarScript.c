/*
 * CarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**
 * @class CarScript
 * @brief		
 */
/*modded class CarScript
{
	protected ExpansionMarkerModule m_MarkerModule;
	protected string m_ServerMarker;
	protected static int m_ServerMarkerIndex = 0;

	void CarScript()
	{
		if (IsMissionHost())
		{
			m_MarkerModule = ExpansionMarkerModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarkerModule));
			m_ServerMarker = "";
		}
	}

	void ~CarScript()
	{
		//RemoveServerMarker();
	}

	void CreateServerMarker()
	{
		if (IsMissionHost())
		{
			if (m_ServerMarker != "")
				return;

			m_ServerMarkerIndex++;
			m_ServerMarker = "[DBG:" + m_ServerMarkerIndex + "]" + GetDisplayName();

			if (IsCar())
			{
				m_MarkerModule.CreateServerMarker(m_ServerMarker, "Car", GetPosition(), ARGB(255, 255, 0, 0), true);
			} else if (IsHelicopter())
			{
				m_MarkerModule.CreateServerMarker(m_ServerMarker, "Helicopter", GetPosition(), ARGB(255, 0, 255, 0), true);
			} else if (IsBoat())
			{
				m_MarkerModule.CreateServerMarker(m_ServerMarker, "Boat", GetPosition(), ARGB(255, 0, 0, 255), true);
			}
		}
	}

	void RemoveServerMarker()
	{
		if (IsMissionHost())
		{
			if (m_ServerMarker != "")
			{
				m_MarkerModule.RemoveServerMarker(m_ServerMarker);
				m_ServerMarker = "";
			}
		}
	}
};*/
