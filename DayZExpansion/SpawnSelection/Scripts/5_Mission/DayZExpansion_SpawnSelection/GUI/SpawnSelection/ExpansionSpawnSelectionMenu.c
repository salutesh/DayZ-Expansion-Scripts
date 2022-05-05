/**
 * ExpansionSpawnSelectionMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSpawnSelectionMenu: ExpansionScriptViewMenu
{
	private ref array<ref ExpansionSpawSelectionMenuMapMarker> m_MapMarkers;
	private int m_SelectedSpawnPointIndex = -1;
	private int m_SelectedSpawnIndex = -1;
	
	private ref ExpansionRespawnHandlerModule m_RespawnModule;
	private MissionGameplay m_Mission;
	private ref ExpansionSpawnSelectionMenuController m_SpawnSelectionMenuController;
	private bool m_HasCooldown = false;
	private bool m_SelectedSpawnIsTerritory = false;
	private bool m_SelectedSpawnUseCooldown = false;
	private int m_NextListIndex = 0;
	
	private Widget MapSpacer;
	private MapWidget Map_Widget;
	private ButtonWidget Confirm;
	private TextWidget ConfirmText;
	private ButtonWidget Random;
	private TextWidget RandomText;
	private ImageWidget Background;
	
	void ExpansionSpawnSelectionMenu()
	{
		Print("ExpansionSpawnSelectionMenu - Start");
		
		m_MapMarkers = new array<ref ExpansionSpawSelectionMenuMapMarker>;
		
		CF_Modules<ExpansionRespawnHandlerModule>.Get(m_RespawnModule);
		Class.CastTo(m_Mission, MissionGameplay.Cast(GetGame().GetMission()));
		Class.CastTo(m_SpawnSelectionMenuController, GetController());
		
		Confirm.Show(false);
		Confirm.Enable(false);
		
		Print("ExpansionSpawnSelectionMenu - End");
	}
	
	void ~ExpansionSpawnSelectionMenu()
	{
		Clear();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/SpawnSelection/GUI/layouts/spawn_selection/expansion_spawn_selection.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionSpawnSelectionMenuController;
	}
	
	void FillList(array<ref ExpansionSpawnLocation> SpawnLocations, int type = 0)
	{
		if (!SpawnLocations)
			return;

		int primaryColor = ARGB(255,255,255,255);
		int hoverColor = ARGB(255,255,255,255);
		string markerIcon = "Map Marker";
		bool isTerritory = false;
		
		//! You can use this switch case for modding purposes
		switch (type)
		{
			case 0: //! Normal
				primaryColor = ARGB(255,226,65,66);
				hoverColor = ARGB(255,255,255,255);
			break;
			case 1: //! Territory
				primaryColor = ARGB(255, 0, 102, 204);
				hoverColor = ARGB(255,255,255,255);
				markerIcon = "Territory";
				isTerritory = true;
			break;
		}
				
		for (int i = 0; i < SpawnLocations.Count(); i++)
		{
			ExpansionSpawnLocation currenLocation = SpawnLocations[i];
			ExpansionSpawSelectionMenuLocationEntry location_entry = new ExpansionSpawSelectionMenuLocationEntry(m_NextListIndex, currenLocation, isTerritory);
			m_SpawnSelectionMenuController.SpawnLocationEntries.Insert(location_entry);
			
			//! Create map marker for territory
			vector pos;
			pos = currenLocation.Positions[0];
			
			ExpansionSpawSelectionMenuMapMarker marker = new ExpansionSpawSelectionMenuMapMarker(MapSpacer, Map_Widget, true);
			marker.SetIcon(markerIcon);
			marker.SetPosition(pos);
			marker.SetPrimaryColor(primaryColor);
			marker.SetHoverColour(hoverColor);
			marker.SetName(currenLocation.Name);
			marker.SetLocation(m_NextListIndex, currenLocation);
			marker.SetIsTerritory(isTerritory);
			
			marker.Show(); //! Need to show marker or it will never show up and color and icons dont get applied
			
			m_MapMarkers.Insert(marker);
			
			m_NextListIndex++;
		}
	}
	
	void Clear()
	{
		m_SpawnSelectionMenuController.SpawnLocationEntries.Clear();
		m_MapMarkers.Clear();
	}
	
	void SetSpawnPoint(int index, ExpansionSpawnLocation location, bool setmappos = true)
	{
		m_SelectedSpawnPointIndex = location.Positions.GetRandomIndex();
		m_SelectedSpawnIndex = index;
		m_SelectedSpawnIsTerritory = location.IsTerritory();
		m_SelectedSpawnUseCooldown = location.UseCooldown();
		m_SpawnSelectionMenuController.SelectedLocation = location.Name;
		m_SpawnSelectionMenuController.NotifyPropertyChanged("SelectedLocation");
		
		if (setmappos)
		{
			vector mapPos = location.Positions[0];
			Map_Widget.SetScale(0.2);
			Map_Widget.SetMapPos(Vector(mapPos[0] + 900.0, 0, mapPos[2] - 600.0));
		}
		
		Confirm.Show(true);
		Confirm.Enable(true);
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		GetGame().GetInput().ChangeGameFocus(1);
		SetFocus(GetLayoutRoot());
		PPEffects.SetBlurMenu(0.5);
		m_Mission.GetHud().ShowHud(false);
		m_Mission.GetHud().ShowQuickBar(false);
		m_SpawnSelectionMenuController.SelectedLocation = "NONE";
		m_SpawnSelectionMenuController.NotifyPropertyChanged("SelectedLocation");
		
		if (GetExpansionSettings().GetSpawn().BackgroundImagePath != string.Empty)
			Background.LoadImageFile(0, GetExpansionSettings().GetSpawn().BackgroundImagePath);
		else
			Background.LoadImageFile(0, "DayZExpansion/SpawnSelection/GUI/textures/wood_background.edds");
	}
	
	void ClearSpawnPoint()
	{
		m_SelectedSpawnPointIndex = -1;
		m_SelectedSpawnIndex = -1;
		m_SpawnSelectionMenuController.SelectedLocation = "NONE";
		m_SpawnSelectionMenuController.NotifyPropertyChanged("SelectedLocation");
	}
	
	override void OnHide()
	{
		super.OnHide();
		
		if (GetGame().GetMission())
		{
			GetGame().GetInput().ResetGameFocus();
			PPEffects.SetBlurMenu(0.0);
			m_Mission.GetHud().ShowHud(true);
			m_Mission.GetHud().ShowQuickBar(true);
			Clear();
		}
	}
	
	void Spawn()
	{
		if (m_SelectedSpawnPointIndex == -1 || m_SelectedSpawnIndex == -1)
			return;

		m_RespawnModule.SelectSpawn(m_SelectedSpawnIndex, m_SelectedSpawnPointIndex, m_SelectedSpawnIsTerritory, m_SelectedSpawnUseCooldown);
		Confirm.Show(false);
		Confirm.Enable(false);
		Random.Show(false);
		Random.Enable(false);
	}
	
	void SpawnRandom()
	{
		int count = m_SpawnSelectionMenuController.SpawnLocationEntries.Count();
		if (!count)
			return;

		TIntArray candidates();
		for (int i = 0; i < count; i++)
		{
			if (!m_SpawnSelectionMenuController.SpawnLocationEntries[i].HasCooldown())
				candidates.Insert(i);
		}

		if (!candidates.Count())
			return;

		int idx = candidates.GetRandomElement();
		ExpansionSpawnLocation loc = m_SpawnSelectionMenuController.SpawnLocationEntries[idx].GetLocation();

		if (!loc.Positions.Count())
			return;

		m_RespawnModule.SelectSpawn(idx, loc.Positions.GetRandomIndex(), loc.IsTerritory, loc.UseCooldown);
		Confirm.Show(false);
		Confirm.Enable(false);
		Random.Show(false);
		Random.Enable(false);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
			if (w == Confirm)
			{
				Confirm.SetColor(ARGB(255,220,220,220));
				ConfirmText.SetColor(ARGB(255,0,0,0));
			}
			else if (w == Random)
			{
				Random.SetColor(ARGB(255,220,220,220));
				RandomText.SetColor(ARGB(255,0,0,0));
			}
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
			if (w == Confirm)
			{
				Confirm.SetColor(ARGB(255,0,0,0));
				ConfirmText.SetColor(ARGB(255,220,220,220));
			}
			else if (w == Random)
			{
				Random.SetColor(ARGB(255,0,0,0));
				RandomText.SetColor(ARGB(255,220,220,220));
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override float GetUpdateTickRate()
	{
		return 0.01;
	}
	
	override void Update() 
	{
		for ( int i = 0; i < m_MapMarkers.Count(); i++ )
		{
			m_MapMarkers[i].Update(0.3);
		}
	}
	
	ButtonWidget GetConfirmButton()
	{
		return Confirm;
	}
	
	MapWidget GetMapWidget()
	{
		return Map_Widget;
	}
	
	void UpdateMarkerCooldownState(int index)
	{
		for (int i = 0; i < m_MapMarkers.Count(); i++)
		{
			ExpansionSpawSelectionMenuMapMarker marker = m_MapMarkers[i];
			if (marker.GetIndex() == index && !marker.IsDeathMarker())
			{
				marker.UpdateCooldown();
				continue;
			}
		}
	}
	
	void CreateDeathMarker(vector pos)
	{
		ExpansionSpawSelectionMenuMapMarker marker = new ExpansionSpawSelectionMenuMapMarker(MapSpacer, Map_Widget, true);
		marker.SetIcon("Skull 3");
		marker.SetPosition(pos);
		marker.SetPrimaryColor(ARGB(255,106,0,0));
		marker.SetHoverColour(ARGB(255,255,255,255));
		marker.SetName("You died here!");
		marker.SetIsDeathMarker(true);
		marker.Show();
		
		m_MapMarkers.Insert(marker);
	}
};

class ExpansionSpawnSelectionMenuController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionSpawSelectionMenuLocationEntry> SpawnLocationEntries = new ObservableCollection<ref ExpansionSpawSelectionMenuLocationEntry>(this);
	string SelectedLocation;
};

