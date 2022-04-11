/**
 * ExpansionPlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	protected int m_CountMap;
	protected int m_CountGPS;
	protected int m_CountPen;
	protected int m_CountCompass;

	//Only server side
	protected int m_QuickMarkerColor;
	
	// ------------------------------------------------------------
	// PlayerBase Constructor
	// ------------------------------------------------------------
	void PlayerBase()
	{
		m_CountMap = 0;
		m_CountGPS = 0;
		m_CountPen = 0;
		m_CountCompass = 0;
		
		SetRandomQuickMarkerColor();
	}

	// ------------------------------------------------------------
	// PlayerBase Destructor
	// ------------------------------------------------------------
	void ~PlayerBase() {};
	
	// ------------------------------------------------------------
	// PlayerBase DeferredClientInit
	// ------------------------------------------------------------
	override void DeferredClientInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "DeferredClientInit");
#endif

		super.DeferredClientInit();
		
		//! Only run this on this client's player, not other playerbase entities (it will be handled in their client if they are players)
		if (GetGame() && IsMissionClient() && GetGame().GetPlayer() == this && GetModuleManager())
		{
			ExpansionMarkerModule module;
			if (Class.CastTo(module, GetModuleManager().GetModule(ExpansionMarkerModule)))
				module.Refresh();
		}
	}

	// ------------------------------------------------------------
	// Expansion SetRandomQuickMarkerColor
	// ------------------------------------------------------------
	void SetRandomQuickMarkerColor()
	{
		if (IsMissionHost())
		{
			array<int> colors = new array<int>;
			
			int geen_sea = ARGB(255, 22, 160, 133);
			colors.Insert(geen_sea);
			
			int nephritis = ARGB(255, 39, 174, 96);
			colors.Insert(nephritis);
			
			int belize_hole = ARGB(255, 41, 128, 185);
			colors.Insert(belize_hole);
			
			int wisteria = ARGB(255, 142, 68, 173);
			colors.Insert(wisteria);
			
			int orange = ARGB(255, 243, 156, 18);
			colors.Insert(orange);
			
			int pumpkin = ARGB(255, 211, 84, 0);
			colors.Insert(pumpkin);
			
			int pomegranate = ARGB(255, 192, 57, 43);
			colors.Insert(pomegranate);
			
			int greenland_green = ARGB(255, 34, 166, 179);
			colors.Insert(greenland_green);
			
			int steel_pink = ARGB(255, 190, 46, 221);
			colors.Insert(steel_pink);
	
			m_QuickMarkerColor = colors.GetRandomElement();
		}
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetQuickMarkerColor
	// ------------------------------------------------------------
	int GetQuickMarkerColor()
	{
		return m_QuickMarkerColor;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemMap
	// ------------------------------------------------------------
	void SetHasItemMap(bool state)
	{
		if (state)
		{
			m_CountMap += 1;
		}
		else if (m_CountMap > 0)
		{
			m_CountMap -= 1;
		}
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemMap
	// ------------------------------------------------------------
	bool HasItemMap()
	{
		return m_CountMap > 0;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemGPS
	// ------------------------------------------------------------
	void SetHasItemGPS(bool state)
	{
		if (state)
		{
			m_CountGPS += 1;
		} 
		else if (m_CountGPS > 0)
		{
			m_CountGPS -= 1;
		}
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemGPS
	// ------------------------------------------------------------
	bool HasItemGPS()
	{
		return m_CountGPS > 0;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemPen
	// ------------------------------------------------------------
	void SetHasItemPen(bool state)
	{
		if (state)
		{
			m_CountPen += 1;
		} 
		else if (m_CountPen > 0)
		{
			m_CountPen -= 1;
		}
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemPen
	// ------------------------------------------------------------
	bool HasItemPen()
	{
		return m_CountPen > 0;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemCompass
	// ------------------------------------------------------------
	void SetHasItemCompass(bool state)
	{
		if (state)
		{
			m_CountCompass += 1;
		} 
		else if (m_CountCompass > 0)
		{
			m_CountCompass -= 1;
		}
		
		Print("m_CountCompass: " + m_CountCompass);
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemCompass
	// ------------------------------------------------------------
	bool HasItemCompass()
	{
		return m_CountCompass > 0;
	}
	
	// ------------------------------------------------------------
	// PlayerBase EEKilled
	// ------------------------------------------------------------	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		
		if (GetExpansionSettings().GetMap().CreateDeathMarker)
		{
			ExpansionMarkerModule module = ExpansionMarkerModule.Cast(GetModuleManager().GetModule(ExpansionMarkerModule));
			if (module && GetIdentity() != NULL)
				module.CreateDeathMarkerServer(GetPosition(), GetIdentity());
		}
	}
};
