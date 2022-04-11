/**
 * ExpansionSpawSelectionMenuLocationEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSpawSelectionMenuLocationEntry: ExpansionScriptView
{
	private int m_Index;
	private ref ExpansionSpawnLocation m_Location;
	private ref ExpansionSpawSelectionMenuLocationEntryController m_SpawnSelectionEntryController;
	private ExpansionRespawnHandlerModule m_RespawnModule;
	private bool m_HasCooldown = false;
	private bool m_IsLocked = false;
	
	private ButtonWidget spawn_entry;
	private Widget background;
	private TextWidget name;
	private ImageWidget icon;
	private ImageWidget icon_locked;
	private ImageWidget cooldown_icon;
	private TextWidget cooldown;
	
	void ExpansionSpawSelectionMenuLocationEntry(int index, ExpansionSpawnLocation location)
	{
		m_Index = index;
		m_Location = location;
	
		Class.CastTo(m_SpawnSelectionEntryController, GetController());
		Class.CastTo(m_RespawnModule, GetModuleManager().GetModule(ExpansionRespawnHandlerModule));
		
		SetDisplayName(m_Location.Name);
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/GUI/layouts/spawn_selection/expansion_spawn_selection_entry.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionSpawSelectionMenuLocationEntryController;
	}
	
	private void UpdateCooldown()
	{
		if (m_RespawnModule)
		{
			foreach (ExpansionRespawnDelayTimer timer: m_RespawnModule.m_PlayerRespawnDelays)
			{
				if (timer.Index == m_Index && timer.HasCooldown() && m_Location.IsTerritory() == timer.IsTerritory())
				{
					int cooldownTime = timer.GetTimeDiff();
					if (GetExpansionSettings().GetSpawn().PunishMultispawn)
					{
						if (cooldownTime < (GetExpansionSettings().GetSpawn().RespawnCooldown + timer.GetPunishment()))
						{
							m_HasCooldown = true;
							SetDisplayTime((GetExpansionSettings().GetSpawn().RespawnCooldown + timer.GetPunishment()) - cooldownTime);
							if (!m_IsLocked)
								SetLocked();
						}
					}
					else
					{
						if (cooldownTime < GetExpansionSettings().GetSpawn().RespawnCooldown)
						{
							m_HasCooldown = true;
							SetDisplayTime(GetExpansionSettings().GetSpawn().RespawnCooldown - cooldownTime);
							if (!m_IsLocked)
								SetLocked();
						}
					}
				}
				else if (timer.Index == m_Index && !timer.HasCooldown() && m_Location.IsTerritory() == timer.IsTerritory())
				{
					OnCooldownEnd();
				}
				
				ExpansionSpawnSelectionMenu spawnSelectionMenu = ExpansionSpawnSelectionMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
				if (spawnSelectionMenu)
					spawnSelectionMenu.UpdateMarkerCooldownState(m_Index);
			}
		}
	}
	
	void OnCooldownEnd()
	{
		m_HasCooldown = false;
		if (m_IsLocked)
			SetUnlocked();
	}
	
	void SetLocked()
	{
		m_IsLocked = true;
		icon.Show(false);
		icon_locked.Show(true);
		cooldown_icon.Show(true);
		cooldown.Show(true);
		background.SetColor(ARGB(255,106,0,0));
	}
	
	void SetUnlocked()
	{
		icon.Show(true);
		icon_locked.Show(false);
		cooldown_icon.Show(false);
		cooldown.Show(false);
		background.SetColor(ARGB(255,0,0,0));
	}
	
	void OnEntryButtonClick()
	{
		ExpansionSpawnSelectionMenu spawnSelectionMenu = ExpansionSpawnSelectionMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
		if (!spawnSelectionMenu)
			return;
		
		if (m_HasCooldown)
		{
			vector mapPos = m_Location.Positions[0];
			spawnSelectionMenu.GetMapWidget().SetScale(0.2);
			spawnSelectionMenu.GetMapWidget().SetMapPos(Vector(mapPos[0] + 900.0, 0, mapPos[2] - 600.0));
			spawnSelectionMenu.GetConfirmButton().Show(false);
			spawnSelectionMenu.GetConfirmButton().Enable(false);
			spawnSelectionMenu.ClearSpawnPoint();
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_SPAWNSELECTION_POINT_LOCKED"), new StringLocaliser("STR_EXPANSION_SPAWNSELECTION_POINT_LOCKED_DESC"), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR).Create();
			return;
		}
		
		spawnSelectionMenu.SetSpawnPoint(m_Index, m_Location, true);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		switch (w)
		{
			case spawn_entry:
			{
				ExpansionSpawnSelectionMenu menu = ExpansionSpawnSelectionMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
				if (menu && !m_HasCooldown)
					menu.Spawn();
				break;
			}
		}
		
		return super.OnDoubleClick(w, x, y, button);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case spawn_entry:
			SetHighlight();
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case spawn_entry:
			SetNormal();
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	void SetHighlight()
	{
		background.SetColor(ARGB(255,220,220,220));
		name.SetColor(ARGB(255,0,0,0));
		icon.SetColor(ARGB(255,0,0,0));
	}
	
	void SetNormal()
	{
		if (!m_HasCooldown)
			background.SetColor(ARGB(255,0,0,0));
		else
			background.SetColor(ARGB(255,106,0,0));
		
		name.SetColor(ARGB(255,220,220,220));
		icon.SetColor(ARGB(255,220,220,220));
	}
	
	void SetDisplayName(string locationName)
	{
		m_SpawnSelectionEntryController.LocationName = locationName;
		m_SpawnSelectionEntryController.NotifyPropertyChanged("LocationName");
	}
	
	void SetDisplayTime(int cooldownTime)
	{
		m_SpawnSelectionEntryController.Cooldown = ExpansionStatic.FormatTimestamp(cooldownTime, false);
		m_SpawnSelectionEntryController.NotifyPropertyChanged("Cooldown");
	}
	
	bool HasCooldown()
	{
		return m_HasCooldown;
	}
	
	override float GetUpdateTickRate()
	{
		return 1.0;
	}
	
	override void Update()
	{
		if (GetExpansionSettings().GetSpawn().EnableRespawnCooldowns)
			UpdateCooldown();
	}
};

class ExpansionSpawSelectionMenuLocationEntryController: ExpansionViewController 
{
	string LocationName;
	string Cooldown;
};