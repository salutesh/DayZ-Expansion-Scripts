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
	private bool m_IsTerritory = false;
	private bool m_IsHighlighted = false;
	
	private ButtonWidget spawn_entry;
	private Widget background;
	private TextWidget name;
	private ImageWidget icon;
	private ImageWidget icon_locked;
	private ImageWidget cooldown_icon;
	private TextWidget cooldown;
	
	void ExpansionSpawSelectionMenuLocationEntry(int index, ExpansionSpawnLocation location, bool isTerritory)
	{
		m_Index = index;
		m_Location = location;
		m_IsTerritory = isTerritory;
		
		Class.CastTo(m_SpawnSelectionEntryController, GetController());
		CF_Modules<ExpansionRespawnHandlerModule>.Get(m_RespawnModule);
		
		SetEntry();
	}

	ExpansionSpawnLocation GetLocation()
	{
		return m_Location;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/SpawnSelection/GUI/layouts/spawn_selection/expansion_spawn_selection_entry.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionSpawSelectionMenuLocationEntryController;
	}

	private void SetEntry()
	{
		SetDisplayName(m_Location.Name);
		if (!m_IsTerritory)
			SetIcon(ExpansionIcons.GetPath("Marker"));
		else
			SetIcon(ExpansionIcons.GetPath("Territory"));
	}
		
	private void UpdateCooldown()
	{
		if (m_RespawnModule)
		{
			int respawnCooldown = GetExpansionSettings().GetSpawn().GetCooldown(m_IsTerritory);
			map<int, ref ExpansionRespawnDelayTimer> playerCooldowns = m_RespawnModule.m_PlayerRespawnDelays.GetElement(0);
			if (playerCooldowns)
			{
				ExpansionRespawnDelayTimer timer = playerCooldowns[m_Index];
				if (!timer)
					return;

				if (timer.HasCooldown())
				{
					int cooldownTime = timer.GetTimeDiff();
					int cooldown = respawnCooldown;
					if (GetExpansionSettings().GetSpawn().PunishMultispawn)
					{
						cooldown += timer.GetPunishment();
					}
					if (cooldownTime < cooldown)
					{
						m_HasCooldown = true;
					}
					if (m_HasCooldown)
					{
						SetDisplayTime(cooldown - cooldownTime);
						if (!m_IsLocked)
							SetLocked();
					}
				}
				else
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
		if (!m_IsHighlighted)
			background.SetColor(ARGB(255,0,0,0));
		else
			background.SetColor(ARGB(255,220,220,220));
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
				{
					menu.Spawn();
					return true;
				}
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
		m_IsHighlighted = true;
		background.SetColor(ARGB(255,220,220,220));
		name.SetColor(ARGB(255,0,0,0));
		icon.SetColor(ARGB(255,0,0,0));
	}
	
	void SetNormal()
	{
		m_IsHighlighted = false;
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
	
	void SetIcon(string icon)
	{
		m_SpawnSelectionEntryController.EntryIcon = icon;
		m_SpawnSelectionEntryController.NotifyPropertyChanged("EntryIcon");
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
	string EntryIcon;
	string LocationName;
	string Cooldown;
};