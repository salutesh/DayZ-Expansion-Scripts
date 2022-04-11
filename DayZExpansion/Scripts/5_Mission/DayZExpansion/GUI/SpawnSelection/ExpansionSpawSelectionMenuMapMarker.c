/**
 * ExpansionSpawSelectionMenuMapMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSpawSelectionMenuMapMarker : ExpansionMapWidgetBase
{
	private int m_Index;
	private ref ExpansionSpawnLocation m_Location;
	private ExpansionRespawnHandlerModule m_RespawnModule;
	private bool m_HasCooldown = false;
	private bool m_IsTerritory = false;
	private bool m_IsLocked = false;
	private bool m_IsDeathMarker = false;
		
	void ExpansionSpawSelectionMenuMapMarker(Widget parent, MapWidget mapWidget, bool autoInit = true)
	{
		Class.CastTo(m_RespawnModule, GetModuleManager().GetModule(ExpansionRespawnHandlerModule));
	}
	
	void SetIsTerritory(bool state)
	{
		m_IsTerritory = state;
	}
	
	void UpdateCooldown()
	{
		if (m_RespawnModule)
		{
			foreach (ExpansionRespawnDelayTimer timer: m_RespawnModule.m_PlayerRespawnDelays)
			{
				if (timer.Index == m_Index && timer.HasCooldown() && timer.IsTerritory() == m_IsTerritory)
				{
					int cooldownTime = timer.GetTimeDiff();
					if (GetExpansionSettings().GetSpawn().PunishMultispawn)
					{
						if (cooldownTime < (GetExpansionSettings().GetSpawn().RespawnCooldown + timer.GetPunishment()))
						{
							m_HasCooldown = true;
							if (!m_IsLocked)
								SetLocked();
						}
					}
					else
					{
						if (cooldownTime < GetExpansionSettings().GetSpawn().RespawnCooldown)
						{
							m_HasCooldown = true;
							if (!m_IsLocked)
								SetLocked();
						}
					}
				}
				else if (timer.Index == m_Index && !timer.HasCooldown() && timer.IsTerritory() == m_IsTerritory)
				{
					OnCooldownEnd();
				}
			}
		}
	}
	
	void OnCooldownEnd()
	{
		m_HasCooldown = false;
		SetUnlocked();
	}
	
	void SetLocked()
	{
		m_IsLocked = true;
		m_Icon.LoadImageFile(0, "set:dayz_gui image:icon_locked_sb");
		SetPrimaryColor(ARGB(255,106,0,0));
	}
	
	void SetUnlocked()
	{
		m_IsLocked = false;
		SetIcon(m_CurrentExpansionIcon);
		if (!m_IsTerritory)
			SetPrimaryColor(ARGB(255,226,65,66));
		else
			SetPrimaryColor(ARGB(255, 0, 102, 204));
	}
	
	void SetLocation(int index, ExpansionSpawnLocation location)
	{
		m_Index = index;
		m_Location = location;
	}

	override void Update(float pDt)
	{
		vector mapPos;
		if (!m_IsDeathMarker)
		{
			mapPos = GetMapWidget().MapToScreen(m_Location.Positions[0]);	
		}
		else
		{
			mapPos = GetMapWidget().MapToScreen(m_WorldPosition);	
		}
			
		float x, y;
		GetLayoutRoot().GetParent().GetScreenPos(x, y);
		GetLayoutRoot().SetPos(mapPos[0] - x, mapPos[1] - y, true);
	}
	
	override void Show()
	{
		super.Show();
		
		if (!m_IsDeathMarker)
			UpdateCooldown();
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (w != NULL && IsEditButton(w) && button == MouseState.LEFT)
		{
			ExpansionSpawnSelectionMenu menu = ExpansionSpawnSelectionMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
			if (menu && !m_HasCooldown)
				menu.Spawn();
		}
		
		return super.OnDoubleClick(w, x, y, button);
	}
	
	override void OnMarkerClick()
	{
		if (m_IsDeathMarker)
			return;
		
		ExpansionSpawnSelectionMenu spawnSelectionMenu = ExpansionSpawnSelectionMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
		if (!spawnSelectionMenu)
			return;
		
		if (m_HasCooldown)
		{
			spawnSelectionMenu.GetConfirmButton().Show(false);
			spawnSelectionMenu.GetConfirmButton().Enable(false);
			spawnSelectionMenu.ClearSpawnPoint();
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_SPAWNSELECTION_POINT_LOCKED"), new StringLocaliser("STR_EXPANSION_SPAWNSELECTION_POINT_LOCKED_DESC"), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR).Create();
			return;
		}
		
		spawnSelectionMenu.SetSpawnPoint(m_Index, m_Location, false);
	}
	
	int GetIndex()
	{
		return m_Index;
	}
	
	override bool CanDrag()
	{
		return false;
	}
	
	void SetIsDeathMarker(bool state)
	{
		m_IsDeathMarker = state;
	}
	
	bool IsDeathMarker()
	{
		return m_IsDeathMarker;
	}
};
