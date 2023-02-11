/**
 * ExpansionMapMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMenu extends ExpansionUIScriptedMenu
{
	protected PlayerBase m_PlayerB;
	protected ref MapWidget m_MapWidget;
	protected int COLOR_EXPANSION_MARKER_PLAYER_POSITION = ARGB(255, 255, 180, 24);

	protected ref array<ref ExpansionMapWidgetBase> m_Markers;
	protected ref map<string, ExpansionMapMarker> m_PersonalMarkers;
	protected ref map<string, ExpansionMapMarker> m_ServerMarkers;
#ifdef EXPANSIONMODGROUPS
	protected ref map<string, ExpansionMapMarker> m_PartyMarkers;
	protected ref map<string, ExpansionMapMarker> m_PlayerMarkers;
#endif

	protected ref set<ExpansionMapMarker> m_DeletingMarkers;
	protected ExpansionMapMarker m_SelectedMarker;
	protected ExpansionMarkerModule m_MarkerModule;

#ifdef EXPANSIONMODGROUPS
	protected ExpansionPartyModule m_PartyModule;
#endif

	protected bool m_GpsWasOpen = false;
	protected ref ExpansionMapMarkerList m_MarkerList;

	protected float m_OpenMapTime = 0;
	protected float m_Time;
	protected bool m_IsEditingMarker = false;
	protected bool m_IsShown;

	protected bool m_DoUpdateMarkers;
	protected int m_MaxMarkerUpdatesPerFrame = 3;  //! Max markers updated per frame for each marker type

	protected ref array<string> m_PartyMarkersCheckArr;
	protected int m_PartyMarkersUpdateIndex;
	protected bool m_PartyMarkersUpdated;

	protected ref array<string> m_PersonalMarkersCheckArr;
	protected int m_PersonalMarkersUpdateIndex;
	protected bool m_PersonalMarkersUpdated;

	protected ref array<string> m_PlayerMarkersCheckArr;
	protected int m_PlayerMarkersUpdateIndex;
	protected bool m_PlayerMarkersUpdated;

	protected ref array<string> m_ServerMarkersCheckArr;
	protected int m_ServerMarkersUpdateIndex;
	protected bool m_ServerMarkersUpdated;

	protected const string COORD_FORMAT = "%1.%2%3";
	protected const int SCALE_RULER_LINE_WIDTH = 8;
	protected const int SCALE_RULER_NUM_SEGMENTS 	= 10;

	protected bool m_WasChanged;
	protected bool m_HasCompass;
	protected bool m_HasGPS;
	protected bool m_HasExpansionGPS;
	protected bool m_IsOpenning;

	protected float m_ToolScaleCellSizeCanvasWidth;
	protected ref IngameHud	m_Hud ;
	protected ref SizeToChild	 m_LegendResizer;
	protected Widget m_GPSMarker;
	protected ImageWidget m_GPSMarkerArrow;
	protected Widget m_UpperLegendContainer;
	protected ImageWidget m_ToolsCompassBase;
	protected ImageWidget m_ToolsCompassArrow;
	protected TextWidget m_ToolsCompassAzimuth;
	protected TextWidget m_ToolsScaleContourText;
	protected TextWidget m_ToolsGPSElevationText;
	protected TextWidget m_ToolsGPSXText;
	protected TextWidget m_ToolsGPSYText;
	protected TextWidget m_ToolsScaleCellSizeText;
	protected CanvasWidget m_ToolsScaleCellSizeCanvas;
	protected ItemMap m_Map;
#ifndef DAYZ_1_18
	//! 1.19
	protected ref MapNavigationBehaviour m_MapNavigationBehaviour;
#endif

	// ------------------------------------------------------------
	// Expansion ExpansionMapMenu Constructor
	// ------------------------------------------------------------
	void ExpansionMapMenu()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::ExpansionMapMenu - Start");
		#endif

		m_Markers = new array<ref ExpansionMapWidgetBase>();

		m_PersonalMarkers = new map<string, ExpansionMapMarker>();
		m_ServerMarkers = new map<string, ExpansionMapMarker>();

	#ifdef EXPANSIONMODGROUPS
		m_PartyMarkers = new map<string, ExpansionMapMarker>();
		m_PlayerMarkers = new map<string, ExpansionMapMarker>();
	#endif

		m_DeletingMarkers = new set<ExpansionMapMarker>();
		Class.CastTo(m_PlayerB, GetGame().GetPlayer());
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);

	#ifdef EXPANSIONMODGROUPS
		CF_Modules<ExpansionPartyModule>.Get(m_PartyModule);
	#endif

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::ExpansionMapMenu - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMapMenu Destructor
	// ------------------------------------------------------------
	void ~ExpansionMapMenu()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::~ExpansionMapMenu - Start");
		#endif

		delete m_Markers;
		delete m_PersonalMarkers;
		delete m_ServerMarkers;
	#ifdef EXPANSIONMODGROUPS
		delete m_PartyMarkers;
		delete m_PlayerMarkers;
	#endif
		delete m_DeletingMarkers;

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::~ExpansionMapMenu - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Override Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Init - Start");
		#endif

	#ifdef EXPANSIONMODGROUPS
		m_PartyMarkersUpdated = false;
		m_PlayerMarkersUpdated = false;
	#endif
		m_PersonalMarkersUpdated = false;
		m_ServerMarkersUpdated = false;
		m_DoUpdateMarkers = true;

		if (layoutRoot)
		{
			SetMapLegend(layoutRoot);
			return layoutRoot;
		}

		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Navigation/GUI/layouts/expansion_map.layout");
		Class.CastTo(m_MapWidget, layoutRoot.FindAnyWidget( "Map" ));
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());

		if (GetExpansionSettings().GetMap().ShowPlayerPosition == 1 || GetExpansionSettings().GetMap().ShowPlayerPosition == 2)
		{
			ExpansionMapMarkerPlayerArrow player_Marker = new ExpansionMapMarkerPlayerArrow(layoutRoot, m_MapWidget);
			if (Class.CastTo(player, g_Game.GetPlayer()))
			{
				if (GetExpansionClientSettings().StreamerMode)
				{
					player_Marker.SetName(new StringLocaliser("STR_EXPANSION_MAP_MARKER_PLAYERMARKER_STREAMERMODE"));
				}
				else
				{
					StringLocaliser text = new StringLocaliser("STR_EXPANSION_MAP_MARKER_PLAYERMARKER", player.GetIdentityName());
					player_Marker.SetName(text.Format());
				}

				player_Marker.SetPosition(player.GetPosition());
			}

			if (GetExpansionSettings().GetMap().ShowPlayerPosition == 2)
			{
				if (PlayerBase.Cast(GetGame().GetPlayer() ).HasItemCompass())
				{
					player_Marker.Show();
				}
				else
				{
					player_Marker.Hide();
				}
			}
			else
			{
				player_Marker.Show();
			}

			m_Markers.Insert(player_Marker);
		}

		Widget marker_list = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Navigation/GUI/layouts/expansion_map_markerlist.layout", layoutRoot);
		marker_list.GetScript(m_MarkerList);

		m_MarkerList.Init(this);
		SetMapPosition();
		SetMapLegend(layoutRoot);

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::Init - End and return layoutRoot: " + layoutRoot.ToString());
		#endif

		return layoutRoot;
	}

	// ------------------------------------------------------------
	// Expansion SetMapLegend
	// ------------------------------------------------------------
	protected void SetMapLegend(Widget root)
	{
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());

		m_IsOpenning = true;
		Widget mapToolsContainer = root.FindAnyWidget("Map_Tools_Container");
		mapToolsContainer.GetScript(m_LegendResizer);

		m_GPSMarker = root.FindAnyWidget("GPSMarkerCircle");
		m_GPSMarkerArrow = ImageWidget.Cast(root.FindAnyWidget("GPSMarkerArrow"));
		m_UpperLegendContainer = root.FindAnyWidget("Tools_Extra");
		root.Update();

		m_ToolsCompassBase = ImageWidget.Cast(root.FindAnyWidget("Tools_Compass_Base"));
		m_ToolsCompassBase.SetColor(ARGB(255, 220, 220, 220));

		m_ToolsCompassArrow = ImageWidget.Cast(root.FindAnyWidget("Tools_Compass_Arrow"));
		m_ToolsCompassArrow.SetColor(ARGB(255, 220, 220, 220));

		m_ToolsCompassAzimuth = TextWidget.Cast(root.FindAnyWidget("Tools_Compass_Azimuth"));
		m_ToolsGPSXText = TextWidget.Cast(root.FindAnyWidget("Tools_GPS_X_Value"));
		m_ToolsGPSYText = TextWidget.Cast(root.FindAnyWidget("Tools_GPS_Y_Value"));
		m_ToolsGPSElevationText = TextWidget.Cast(root.FindAnyWidget("Tools_GPS_Elevation_Value"));
		m_ToolsScaleContourText = TextWidget.Cast(root.FindAnyWidget("Tools_Scale_Contour_Value"));
		m_ToolsScaleCellSizeText = TextWidget.Cast(root.FindAnyWidget("Tools_Scale_CellSize_Value"));
		m_ToolsScaleCellSizeCanvas = CanvasWidget.Cast(root.FindAnyWidget("Tools_Scale_CellSize_Canvas"));

		float canvasHeight = 0;
		m_ToolsScaleCellSizeCanvas.GetSize(m_ToolScaleCellSizeCanvasWidth, canvasHeight);

		if (m_MapWidget)
		{
			float scale;
			vector mapPosition;
			if (player && !player.GetLastMapInfo(scale, mapPosition))
			{
				vector tempPosition = GetGame().ConfigGetVector(string.Format("CfgWorlds %1 centerPosition", GetGame().GetWorldName()));
				scale = 0.33;
				mapPosition = Vector(tempPosition[0], tempPosition[1], tempPosition[2]);
			}

			m_MapWidget.SetScale(scale);
			m_MapWidget.SetMapPos(mapPosition);

		#ifndef DAYZ_1_18
			//! 1.19
			m_HasCompass 	= false;
			m_HasGPS = false;
			m_HasExpansionGPS = false;

			SetGPSMarkerVisibility(false);
			SetGPSDirectionVisibility(false);
			SetCompassUIVisibility(false);
			SetUpperLegendVisibility(true);

			if (player)
			{
				m_MapNavigationBehaviour = player.GetMapNavigationBehaviour();
				if (m_MapNavigationBehaviour)
				{
					m_HasGPS = (m_MapNavigationBehaviour.GetNavigationType() & EMapNavigationType.GPS|EMapNavigationType.ALL == 0);
					m_HasCompass = (m_MapNavigationBehaviour.GetNavigationType() & EMapNavigationType.COMPASS|EMapNavigationType.ALL == 0);
					m_HasExpansionGPS = player.HasItemGPS();

					//! gameplay cfg json overrides handling
					m_HasGPS = CfgGameplayHandler.GetMapIgnoreNavItemsOwnership() || m_HasGPS;
					m_HasCompass = CfgGameplayHandler.GetMapIgnoreNavItemsOwnership() || m_HasCompass;

					if (m_HasGPS || m_HasExpansionGPS)
					{
						SetUpperLegendVisibility(true);
						if (m_HasGPS && CfgGameplayHandler.GetMapDisplayPlayerPosition())
						{
							SetGPSMarkerVisibility(true);
							m_MapWidget.SetMapPos(m_MapNavigationBehaviour.GetPositionReal());
						}
					}

					if (m_HasCompass || m_HasExpansionGPS)
					{
						SetCompassUIVisibility(true);
						SetUpperLegendVisibility(true);
						if (m_HasGPS && CfgGameplayHandler.GetMapDisplayPlayerPosition())
						{
							SetGPSDirectionVisibility(true);
						}
					}

					//! override the CfgGameplayHandler.GetMapIgnoreNavItemsOwnership()
					if ((!m_HasGPS && !m_HasCompass && !m_HasExpansionGPS) || !CfgGameplayHandler.GetMapDisplayNavigationInfo())
					{
						SetUpperLegendVisibility(false);
					}
				}
			}
		#endif
		}
	}

	// ------------------------------------------------------------
	// Expansion Override Refresh
	// ------------------------------------------------------------
	override void Refresh()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::Refresh - Start");
		#endif

		super.Refresh();

		m_DoUpdateMarkers = true;

		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::Refresh - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdateMarkers
	// ------------------------------------------------------------
	void UpdateMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMarkers - Start");
		#endif

		if (!m_PersonalMarkersUpdated)
			UpdatePersonalMarkers();
		if (!m_ServerMarkersUpdated)
			UpdateServerMarkers();
	#ifdef EXPANSIONMODGROUPS
		if (!m_PartyMarkersUpdated)
			UpdatePartyMarkers();
		if (!m_PlayerMarkersUpdated)
			UpdatePlayerMarkers();
	#endif

		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMarkers - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdatePersonalMarkers
	// ------------------------------------------------------------
	private void UpdatePersonalMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePersonalMarkers - Start");
		#endif

		int removeIndex = 0;
		int count = m_MarkerModule.GetData().PersonalCount();
		int index = m_PersonalMarkersUpdateIndex;
		int end = Math.Min(index + m_MaxMarkerUpdatesPerFrame, count);
		ExpansionMapMarker marker = NULL;
		string uid = "";

		for (int i = 0; i < m_DeletingMarkers.Count(); ++i)
		{
			marker = m_DeletingMarkers[i];
			if (marker && marker.GetMarkerData() != NULL)
			{
				if (marker.GetMarkerData().GetType() == ExpansionMapMarkerType.PERSONAL)
				{
					int idx2 = m_Markers.Find(marker);
					if (idx2 != -1)
						m_Markers.Remove(idx2);

					delete marker;
					m_DeletingMarkers.Remove(i);
					i--;
				}
			}
		}

		if (index == 0)
			m_PersonalMarkersCheckArr = m_PersonalMarkers.GetKeyArray();

		for (; index < end; ++index)
		{
			ExpansionMarkerData markerData = m_MarkerModule.GetData().PersonalGet(index);
			uid = markerData.GetUID();
			removeIndex = m_PersonalMarkersCheckArr.Find(uid);
			if (removeIndex != -1)
				m_PersonalMarkersCheckArr.Remove(removeIndex);

			marker = m_PersonalMarkers.Get(uid);
			if (!marker)
			{
				marker = new ExpansionMapMarker(layoutRoot, m_MapWidget, false);
				marker.Init();
				m_PersonalMarkers.Insert(uid, marker);
				m_Markers.Insert(marker);
			}

			ExpansionMapMarkerListEntry listEntry = m_MarkerList.GetPersonalEntry(marker);

			marker.SetMarkerData(markerData);
			marker.SetMapMenu(this);

			if (!listEntry)
				m_MarkerList.AddPersonalEntry(marker);
			else if (listEntry.GetName() != markerData.GetName())
				m_MarkerList.UpdatePersonalEntry(listEntry);
			else
				listEntry.Update();

			m_PersonalMarkersUpdateIndex++;
		}

		if (index == count)
		{
			m_PersonalMarkersUpdateIndex = 0;
			for (index = 0; index < m_PersonalMarkersCheckArr.Count(); ++index)
			{
				marker = m_PersonalMarkers.Get(m_PersonalMarkersCheckArr[index]);
				if (marker && !marker.GetMarkerData())
				{
					removeIndex = m_Markers.Find(marker);
					if (removeIndex != -1)
						m_Markers.Remove(removeIndex);

					m_PersonalMarkers.Remove(m_PersonalMarkersCheckArr[index]);
					m_MarkerList.RemovePersonalEntry(marker);
					delete marker;
				}
			}

			m_PersonalMarkersUpdated = true;
		}

		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePersonalMarkers - End");
		#endif
	}

#ifdef EXPANSIONMODGROUPS
	// ------------------------------------------------------------
	// Expansion UpdatePartyMarkers
	// ------------------------------------------------------------
	private void UpdatePartyMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePartyMarkers - Start");
		#endif

		int removeIndex = 0;
		int index = m_PartyMarkersUpdateIndex;
		ExpansionMapMarker marker = NULL;
		string uid = "";

		if (!m_PartyModule || !m_PartyModule.HasParty())
		{
			foreach (string uuid, ExpansionMapMarker mmarker : m_PartyMarkers)
			{
				if (mmarker)
				{
					removeIndex = m_Markers.Find(mmarker);
					if (removeIndex != -1)
						m_Markers.Remove(removeIndex);

					m_PartyMarkers.Set(uuid, NULL);
					m_MarkerList.RemovePartyEntry(mmarker);
					delete mmarker;
				}
			}

			m_PartyMarkers.Clear();
			m_PartyMarkersUpdated = true;

			#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
			EXLogPrint("ExpansionMapMenu::UpdatePartyMarkers - No Party - End");
			#endif

			return;
		}

		array<ref ExpansionMarkerData> markers = m_PartyModule.GetParty().GetAllMarkers();

		if (index == 0)
			m_PartyMarkersCheckArr = m_PartyMarkers.GetKeyArray();

		for (int i = 0; i < m_DeletingMarkers.Count(); ++i)
		{
			marker = m_DeletingMarkers[i];
			if (marker && marker.GetMarkerData() != NULL)
			{
				if (marker.GetMarkerData().GetType() == ExpansionMapMarkerType.PARTY)
				{
					int idx2 = m_Markers.Find(marker);
					if (idx2 != -1)
						m_Markers.Remove(idx2);

					delete marker;
					m_DeletingMarkers.Remove(i);
					i--;
				}
			}
		}

		int count = markers.Count();
		int end = Math.Min(index + m_MaxMarkerUpdatesPerFrame, count);

		for (; index < end; ++index)
		{
			uid = markers[index].GetUID();
			removeIndex = m_PartyMarkersCheckArr.Find(uid);
			if (removeIndex != -1)
				m_PartyMarkersCheckArr.Remove(removeIndex);

			marker = m_PartyMarkers.Get(uid);
			if (!marker)
			{
				marker = new ExpansionMapMarker(layoutRoot, m_MapWidget, false);
				marker.Init();
				m_PartyMarkers.Insert(uid, marker);
				m_Markers.Insert(marker);
			}

			ExpansionMapMarkerListEntry listEntry = m_MarkerList.GetPartyEntry(marker);
			marker.SetMarkerData(markers[index]);
			marker.SetMapMenu(this);

			if (!listEntry)
				m_MarkerList.AddPartyEntry(marker);
			else if (listEntry.GetName() != markers[index].GetName())
				m_MarkerList.UpdatePartyEntry(listEntry);
			else
				listEntry.Update();

			m_PartyMarkersUpdateIndex++;
		}

		if (index == count)
		{
			m_PartyMarkersUpdateIndex = 0;
			for (index = 0; index < m_PartyMarkersCheckArr.Count(); ++index)
			{
				marker = m_PartyMarkers.Get(m_PartyMarkersCheckArr[index]);
				if (marker && !marker.GetMarkerData())
				{
					removeIndex = m_Markers.Find(marker);
					if (removeIndex != -1)
						m_Markers.Remove(removeIndex);

					m_PartyMarkers.Remove(m_PartyMarkersCheckArr[index]);
					m_MarkerList.RemovePartyEntry(marker);
					delete marker;
				}
			}

			m_PartyMarkersUpdated = true;
		}

		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePartyMarkers - End");
		#endif
	}
#endif

	// ------------------------------------------------------------
	// Expansion UpdateServerMarkers
	// ------------------------------------------------------------
	private void UpdateServerMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateServerMarkers - Start");
		#endif

		if (!m_MarkerModule)
		{
			m_ServerMarkersUpdated = true;

			#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
			EXLogPrint("ExpansionMapMenu::UpdateServerMarkers - No Marker Module - End");
			#endif

			return;
		}

		int removeIndex = 0;
		int count = m_MarkerModule.GetData().ServerCount();
		int index = m_ServerMarkersUpdateIndex;
		int end = Math.Min(index + m_MaxMarkerUpdatesPerFrame, count);
		ExpansionMapMarker marker = NULL;
		string uid = "";

		if (index == 0)
			m_ServerMarkersCheckArr = m_ServerMarkers.GetKeyArray();

		for (; index < end; ++index)
		{
			uid = m_MarkerModule.GetData().ServerGet(index).GetUID();
			removeIndex = m_ServerMarkersCheckArr.Find(uid);
			if (removeIndex != -1)
				m_ServerMarkersCheckArr.Remove(removeIndex);

			marker = m_ServerMarkers.Get(uid);
			if (!marker)
			{
				marker = new ExpansionMapMarkerServer(layoutRoot, m_MapWidget, false);
				marker.Init();
				m_ServerMarkers.Insert(uid, marker);
				m_Markers.Insert(marker);
			}

			ExpansionMapMarkerListEntry listEntry = m_MarkerList.GetServerEntry(marker);
			marker.SetMarkerData(m_MarkerModule.GetData().ServerGet(index));
			marker.SetMapMenu(this);

			if (!listEntry)
				m_MarkerList.AddServerEntry(marker);
			else
				listEntry.Update();

			m_ServerMarkersUpdateIndex++;
		}

		if (index == count)
		{
			m_ServerMarkersUpdateIndex = 0;
			for (index = 0; index < m_ServerMarkersCheckArr.Count(); ++index)
			{
				marker = m_ServerMarkers.Get(m_ServerMarkersCheckArr[index]);
				if (marker && !marker.GetMarkerData())
				{
					removeIndex = m_Markers.Find(marker);
					if (removeIndex != -1)
						m_Markers.Remove(removeIndex);

					m_ServerMarkers.Remove(m_ServerMarkersCheckArr[index]);
					m_MarkerList.RemoveServerEntry(marker);
					delete marker;
				}
			}

			m_ServerMarkersUpdated = true;
		}

		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateServerMarkers - End");
		#endif
	}

#ifdef EXPANSIONMODGROUPS
	// ------------------------------------------------------------
	// Expansion UpdatePlayerMarkers
	// ------------------------------------------------------------
	private void UpdatePlayerMarkers()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePlayerMarkers - Start");
		#endif

		int removeIndex = 0;
		int index = m_PlayerMarkersUpdateIndex;
		ExpansionMapMarker marker = NULL;
		string uid = "";

		if (!m_PartyModule || !m_PartyModule.HasParty())
		{
			foreach (string uuid, ExpansionMapMarker mmarker : m_PlayerMarkers)
			{
				if (mmarker)
				{
					removeIndex = m_Markers.Find(mmarker);
					if (removeIndex != -1)
						m_Markers.Remove(removeIndex);

					m_PlayerMarkers.Set(uuid, NULL);
					m_MarkerList.RemoveMemberEntry(mmarker);
					delete mmarker;
				}
			}

			m_PlayerMarkers.Clear();
			m_PlayerMarkersUpdated = true;

			#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
			EXLogPrint("ExpansionMapMenu::UpdatePlayerMarkers - No Party - End");
			#endif

			return;
		}

		PlayerBase localPlayer = PlayerBase.Cast(GetGame().GetPlayer());
		string localUid = "";
		if (localPlayer)
			localUid = localPlayer.GetIdentityUID();

		array<ref ExpansionPartyPlayerData> players = m_PartyModule.GetParty().GetPlayers();

		if (index == 0)
			m_PlayerMarkersCheckArr = m_PlayerMarkers.GetKeyArray();

		int count = players.Count();
		int end = Math.Min( index + m_MaxMarkerUpdatesPerFrame, count );

		for (; index < end; ++index)
		{
			uid = players[index].UID;
			removeIndex = m_PlayerMarkersCheckArr.Find(uid);
			if (removeIndex != -1)
				m_PlayerMarkersCheckArr.Remove(removeIndex);

			if (uid == localUid)
				continue;

			marker = ExpansionMapMarkerPlayer.Cast(m_PlayerMarkers.Get(uid));
			if (!marker)
			{
				marker = new ExpansionMapMarkerPlayer(layoutRoot, m_MapWidget, false);
				marker.Init();
				m_PlayerMarkers.Insert(uid, marker);
				m_Markers.Insert(marker);
			}

			ExpansionMapMarkerListEntry listEntry = m_MarkerList.GetMemberEntry(marker);
			marker.SetMarkerData(players[index].Marker);
			marker.SetMapMenu(this);

			if (!listEntry)
				m_MarkerList.AddMemberEntry(marker);
			else if (listEntry.GetName() != players[index].Marker.GetName())
				m_MarkerList.UpdateMemberEntry(listEntry);
			else
				listEntry.Update();

			m_PlayerMarkersUpdateIndex++;
		}

		if (index == count)
		{
			m_PlayerMarkersUpdateIndex = 0;

			for (index = 0; index < m_PlayerMarkersCheckArr.Count(); ++index)
			{
				marker = ExpansionMapMarkerPlayer.Cast(m_PlayerMarkers.Get(m_PlayerMarkersCheckArr[index]));
				if (marker && !marker.GetMarkerData())
				{
					removeIndex = m_Markers.Find(marker);
					if (removeIndex != -1)
						m_Markers.Remove(removeIndex);

					m_PlayerMarkers.Remove(m_PlayerMarkersCheckArr[index]);
					m_MarkerList.RemoveMemberEntry(marker);
					delete marker;
				}
			}

			m_PlayerMarkersUpdated = true;
		}

		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdatePlayerMarkers - End");
		#endif
	}
#endif

	// ------------------------------------------------------------
	// Expansion CreateNewMarker
	// ------------------------------------------------------------
	void CreateNewMarker(int x, int y)
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CreateNewMarker - Start");
		#endif

		ExpansionMapMarker marker = new ExpansionMapMarker(layoutRoot, m_MapWidget, false);
		marker.Init();
		marker.SetIcon(ExpansionIcons.Get("Map Marker"));
		marker.SetCreation(true);
		marker.SetPosition(x, y);
		marker.SetMapMenu(this);
		marker.ShowEditPanel();
		m_SelectedMarker = marker;
		m_Markers.Insert(marker);

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CreateNewMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion GetSelectedMarker
	// ------------------------------------------------------------
	ExpansionMapMarker GetSelectedMarker()
	{
		return m_SelectedMarker;
	}

	bool HasSelectedMarker()
	{
		return m_SelectedMarker != NULL;
	}

	// ------------------------------------------------------------
	// Expansion SetSelectedMarker
	// ------------------------------------------------------------
	bool SetSelectedMarker(ExpansionMarkerData data)
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::SetSelectedMarker - Start");
		#endif

		if (m_SelectedMarker != NULL)
			CancelCurrentSelectedMarker();

		for (int i = 0; i < m_Markers.Count(); ++i)
		{
			ExpansionMapMarker marker;
			if (Class.CastTo(marker, m_Markers[i]) && marker.GetMarkerData() == data)
			{
				m_SelectedMarker = marker;

				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMenu::SetSelectedMarker - return true for m_SelectedMarker: " + m_SelectedMarker.ToString());
				#endif

				return true;
			}
		}

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::SetSelectedMarker - return false");
		#endif

		return false;
	}

	// ------------------------------------------------------------
	// Expansion CompleteCreationNewMarker
	// ------------------------------------------------------------
	void CompleteCreationNewMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CompleteCreationNewMarker - Start");
		#endif

		if (m_SelectedMarker == NULL)
			return;

		if (!m_MarkerModule.CreateMarker(m_SelectedMarker.GetMarkerData()))
			return;

		m_DeletingMarkers.Insert(m_SelectedMarker);
		PlayDrawSound();

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CompleteCreationNewMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdateSelectedMarker
	// ------------------------------------------------------------
	void UpdateSelectedMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateSelectedMarker - Start");
		#endif

		if (m_SelectedMarker == NULL)
			return;

		if (!m_MarkerModule.UpdateMarker(m_SelectedMarker.GetMarkerData()))
			return;

		m_SelectedMarker = NULL;
		PlayDrawSound();

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateSelectedMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion CancelCurrentSelectedMarker
	// ------------------------------------------------------------
	/**
	 * Cancels what ever is currently happening to the selected marker
	 * If the selected marker is the creation of a new marker, then it deletes it
	 * Otherwise it chucks out the new data
	 */
	void CancelCurrentSelectedMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CancelCurrentSelectedMarker - Start");
		#endif

		if (m_SelectedMarker == NULL)
			return;

		m_SelectedMarker.CloseEditPanel();

		if (m_SelectedMarker.GetMarkerData())
		{
			string uid = m_SelectedMarker.GetMarkerData().GetUID();
		#ifdef EXPANSIONMODGROUPS
			if (m_PersonalMarkers.Get(uid) == m_SelectedMarker || m_PartyMarkers.Get(uid) == m_SelectedMarker)
			{
				m_SelectedMarker = NULL;
				return;
			}
		#else
			if (m_PersonalMarkers.Get(uid) == m_SelectedMarker)
			{
				m_SelectedMarker = NULL;
				return;
			}
		#endif
		}

		int index = m_Markers.Find(m_SelectedMarker);
		if (index != -1)
			m_Markers.Remove(index);

		m_SelectedMarker = NULL;

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::CancelCurrentSelectedMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion DeleteMarker
	// ------------------------------------------------------------
	void DeleteMarker(ExpansionMapMarker marker)
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeleteMarker - Start");
		#endif

		if (marker == NULL)
			return;

		if (marker.GetMarkerData() != NULL)
		{
			string uid = marker.GetMarkerData().GetUID();
			if (m_PersonalMarkers.Get(uid) == marker)
			{
				m_MarkerModule.GetData().PersonalRemoveByUID(uid);
				m_DoUpdateMarkers = true;
				return;
			}

		#ifdef EXPANSIONMODGROUPS
			if (m_PartyMarkers.Get(uid) == marker)
			{
				m_MarkerModule.GetData().PartyRemove(uid);
				return;
			}
		#endif
			return;
		}

		m_DeletingMarkers.Insert(marker);
		m_DoUpdateMarkers = true;

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeleteMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion DeleteSelectedMarker
	// ------------------------------------------------------------
	void DeleteSelectedMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeleteSelectedMarker - Start");
		#endif

		DeleteMarker(m_SelectedMarker);

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeleteSelectedMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion DeletePreviewMarker
	// ------------------------------------------------------------
	void DeletePreviewMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeletePreviewMarker - Start");
		#endif

		DeleteMarker(m_SelectedMarker);
		for (int i = 0; i < m_DeletingMarkers.Count(); ++i)
		{
			ExpansionMapMarker marker = m_DeletingMarkers[i];
			if (marker == NULL)
				continue;

			if (marker.GetMarkerData() != NULL)
			{
				if (marker.GetMarkerData().GetType() == ExpansionMapMarkerType.PERSONAL)
					continue;

				if (marker.GetMarkerData().GetType() == ExpansionMapMarkerType.PARTY)
					continue;
			}

			int index = m_Markers.Find(marker);
			if (index != -1)
				m_Markers.Remove(index);

			m_DeletingMarkers.Remove(i);

			i--;
		}

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::DeletePreviewMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion GetMapMenuRoot
	// ------------------------------------------------------------
	Widget GetMapMenuRoot()
	{
		return layoutRoot;
	}

	// ------------------------------------------------------------
	// Expansion GetCurrentMapPos
	// ------------------------------------------------------------
	protected vector GetCurrentMapPos()
	{
		vector cameraPos;
		vector mapPos;

		cameraPos = GetGame().GetCurrentCameraPosition();
		mapPos = m_MapWidget.MapToScreen(cameraPos);

		return mapPos;
	}

	// ------------------------------------------------------------
	// Expansion GetCurrentMapDir
	// ------------------------------------------------------------
	protected int GetCurrentMapDir()
	{
		vector cameraOri;
		float yaw;
		float normalizedAngle;
		int dir;

		cameraOri = GetGame().GetCurrentCameraDirection().VectorToAngles();
		yaw = cameraOri[0];
		normalizedAngle = Math.NormalizeAngle(yaw);
		dir = Math.Round(normalizedAngle);

		return dir;
	}

	// ------------------------------------------------------------
	// Expansion GetMap
	// ------------------------------------------------------------
	MapWidget GetMap()
	{
		return m_MapWidget;
	}

	// ------------------------------------------------------------
	// Expansion GetMarkerList
	// ------------------------------------------------------------
	ExpansionMapMarkerList GetMarkerList()
	{
		return m_MarkerList;
	}

	// ------------------------------------------------------------
	// Expansion RemoveMarker
	// Events when remove marker with delete key
	// ------------------------------------------------------------
	protected void RemoveMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::RemoveMarker - Start");
		#endif

		for (int i = 0; i < m_Markers.Count(); ++i)
		{
			ExpansionMapMarker marker;
			if (Class.CastTo(marker, m_Markers[i]) && marker.IsMouseHovering() && marker.CanEdit())
			{
				DeleteMarker(marker);

				return;
			}
		}

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::RemoveMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SetMapPosition
	// ------------------------------------------------------------
	protected void SetMapPosition()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::SetMapPosition - Start");
		#endif

		float scale;
		vector map_pos;
		vector player_pos;
		vector camera_pos;

		if (!m_PlayerB)
			m_PlayerB = PlayerBase.Cast( GetGame().GetPlayer() );

		if (m_PlayerB && !m_PlayerB.GetLastMapInfo(scale, map_pos))
		{
			//! Lower number zooms in / Higher number zooms out
			scale = 0.10; // Float between 0-1 ?!
			player_pos = m_PlayerB.GetPosition();
			map_pos = Vector(0,0,0);

			//! Only do this if the server want to show player pos (if not, we don't want to open the map on player pos)
			if (GetExpansionSettings().GetMap().ShowPlayerPosition)
			{
				camera_pos = GetGame().GetCurrentCameraPosition();
				map_pos = camera_pos;
			}
		}

		m_MapWidget.SetScale(scale);
		m_MapWidget.SetMapPos(map_pos);

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::SetMapPosition - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdateMapPosition
	// ------------------------------------------------------------
	protected void UpdateMapPosition()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMapPosition - Start");
		#endif

		float scale = m_MapWidget.GetScale();
		vector map_pos = m_MapWidget.GetMapPos();
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

		if (player)
			player.SetLastMapInfo(scale, map_pos);

		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMapPosition - End");
		#endif
	}

	// ------------------------------------------------------------
	// Override OnDoubleClick
	// ------------------------------------------------------------
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button == MouseState.LEFT)
		{
			if (w == m_MapWidget)
			{
				if (HasSelectedMarker())
				{
					CancelCurrentSelectedMarker();
					return true;
				}
				else if (GetExpansionSettings().GetMap().CanCreateMarker)
				{
					int mouse_x;
					int mouse_y;
					PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

					if (GetExpansionSettings().GetMap().NeedPenItemForCreateMarker && player.HasItemPen())
					{
						#ifdef EXPANSION_MAP_MENU_DEBUG
						EXLogPrint("ExpansionMapMenu::OnDoubleClick - player has pen: " + player.HasItemPen().ToString());
						#endif

						GetGame().GetMousePos(mouse_x, mouse_y);
						CreateNewMarker(mouse_x, mouse_y);
					}
					else if (GetExpansionSettings().GetMap().NeedGPSItemForCreateMarker && player.HasItemGPS())
					{
						#ifdef EXPANSION_MAP_MENU_DEBUG
						EXLogPrint("ExpansionMapMenu::OnDoubleClick - player has gps: " + player.HasItemGPS().ToString());
						#endif

						GetGame().GetMousePos(mouse_x, mouse_y);
						CreateNewMarker(mouse_x, mouse_y);
					}
					else if (!GetExpansionSettings().GetMap().NeedGPSItemForCreateMarker && !GetExpansionSettings().GetMap().NeedPenItemForCreateMarker)
					{
						GetGame().GetMousePos(mouse_x, mouse_y);
						CreateNewMarker(mouse_x, mouse_y);
					}

					return true;
				}
			}
		}

		return super.OnDoubleClick(w, x, y, button);
	}

	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		return super.OnClick(w, x, y, button);
	}

	// ------------------------------------------------------------
	// Expansion Override OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::OnShow - Start");
		#endif

		m_IsShown = true;

		super.OnShow();

		m_OpenMapTime = 0;

		SetFocus(layoutRoot);
		GetGame().GetMission().AddActiveInputExcludes({"map"});

		PPEffects.SetBlurMenu(0.5);

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::OnShow - End");
		#endif
	}

	// ------------------------------------------------------------
	// Override OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::OnHide - Start");
		#endif

		if (!m_IsShown)
			return;

		m_IsShown = false;

		super.OnHide();

		m_MarkerList.HideTooltips();
		m_MarkerModule.SaveLocalServerMarkers();

		if (GetGame().GetMission())
			GetGame().GetMission().RemoveActiveInputExcludes({"map"});

		PPEffects.SetBlurMenu(0.0);

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMenu::OnHide - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Override Update
	// ------------------------------------------------------------
	override void Update(float timeslice)
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::Update - Start");
		#endif

		m_OpenMapTime += timeslice;
		m_Time += timeslice;

		UpdateMapLegend();

		if (GetGame().GetInput().LocalPress("UAUIBack", false))
		{
			CloseMapMenu();
			return;
		}

		if (GetGame().GetInput().LocalPress("UAExpansionMapToggle", false ) && m_OpenMapTime > 0.10 && !IsEditingMarker())
		{
			CloseMapMenu();
			return;
		}

		if (GetGame().GetInput().LocalPress( "UAExpansionMapDeleteMarker", false))
			RemoveMarker();

		UpdateMapPosition();

		if (layoutRoot.IsVisible())
		{
			for (int i = 0; i < m_Markers.Count(); ++i)
			{
				if (m_Markers[i])
				{
					m_Markers[i].Update(timeslice);
				}
			}
		}

		if (m_DoUpdateMarkers)
		{
			UpdateMarkers();

		#ifdef EXPANSIONMODGROUPS
			if (m_PartyMarkersUpdated && m_PersonalMarkersUpdated && m_PlayerMarkersUpdated && m_ServerMarkersUpdated)
		#else
			if (m_PersonalMarkersUpdated && m_ServerMarkersUpdated)
		#endif
			{
			#ifdef EXPANSIONMODGROUPS
				m_PartyMarkersUpdated = false;
				m_PlayerMarkersUpdated = false;
			#endif
				m_PersonalMarkersUpdated = false;
				m_ServerMarkersUpdated = false;
				m_DoUpdateMarkers = false;
			}
		}

		int mouse_x, mouse_y;
		GetGame().GetMousePos(mouse_x, mouse_y);
		vector expMapPos = m_MapWidget.ScreenToMap(Vector(mouse_x, mouse_y, 0));
		m_MarkerList.UpdatePosition(Math.Round(expMapPos[0]), Math.Round(expMapPos[2]));

		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::Update - End");
		#endif
	}

	protected void UpdateMapLegend()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMapLegend - Start");
		#endif

		m_ToolsScaleCellSizeCanvas.Clear();
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());

		if (m_MapWidget)
		{
		#ifndef DAYZ_1_18
			//! 1.19
			if (player)
			{
				m_ToolsScaleContourText.SetText(string.Format("%1 m", m_MapWidget.GetContourInterval()));
				RenderScaleRuler();
				float rulerMaxDistance;
				string rulerUnits;
				ProcessDistanceAndUnits(m_MapWidget.GetCellSize(m_ToolScaleCellSizeCanvasWidth), rulerMaxDistance, rulerUnits);
				m_ToolsScaleCellSizeText.SetText(string.Format("%1%2", rulerMaxDistance, rulerUnits));

				if (m_MapNavigationBehaviour)
				{
					vector mapPos = m_MapWidget.MapToScreen(m_MapNavigationBehaviour.GetPositionReal());
					float scale = 1 - m_MapWidget.GetScale();

					if (m_HasCompass || m_HasExpansionGPS)
					{
						vector rot = player.GetYawPitchRoll();
						float angle = Math.Round(rot[0]);
						if (angle < 0)
							angle = 360 + angle;

						m_GPSMarkerArrow.SetRotation(0, 0, angle);
						m_ToolsCompassArrow.SetRotation(0, 0, angle);
						m_ToolsCompassAzimuth.SetText(string.Format("%1°", angle));
					}

					array<int> coords = MapNavigationBehaviour.OrderedPositionNumbersFromGridCoords(player);
					if (m_HasGPS || m_HasExpansionGPS || CfgGameplayHandler.GetMapIgnoreNavItemsOwnership())
					{
						m_GPSMarker.SetSize(scale * 100, scale * 100);
						m_GPSMarkerArrow.SetSize(scale * 100, scale * 100);
						float sizeX, sizeY;
						m_GPSMarker.GetSize(sizeX, sizeY);
						sizeX = Math.Round(sizeX);
						sizeY = Math.Round(sizeY);
						m_GPSMarker.SetPos(mapPos[0] - sizeX/2, mapPos[1] - sizeY/2);
						m_GPSMarkerArrow.SetPos(mapPos[0] - sizeX/2, mapPos[1] - sizeY/2);

						if (coords.Count() == m_MapNavigationBehaviour.DISPLAY_GRID_POS_MAX_CHARS_COUNT * 2 && coords[0] >= 0)
						{
							m_ToolsGPSXText.SetText(string.Format(COORD_FORMAT, coords[0], coords[1], coords[2]));
							m_ToolsGPSYText.SetText(string.Format(COORD_FORMAT, coords[3], coords[4], coords[5]));
						}
						else
						{
							m_ToolsGPSXText.SetText("-.--");
							m_ToolsGPSYText.SetText("-.--");
						}

						m_ToolsGPSElevationText.SetText(string.Format("%1m", Math.Round(player.GetPosition()[1])));
					}
					else
					{
						m_ToolsGPSXText.SetText("-.--");
						m_ToolsGPSYText.SetText("-.--");
						m_ToolsGPSElevationText.SetText("----m");
					}
				}
			}
		#endif

			m_IsOpenning = false;
		}

		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMenu::UpdateMapLegend - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion PlayDrawSound
	// ------------------------------------------------------------
	void PlayDrawSound()
	{
		if (!GetGame().IsServer() || !GetGame().IsMultiplayer()) // client side
		{
			SEffectManager.PlaySound("Expansion_Draws_SoundSet", m_PlayerB.GetPosition());
		}
	}

	// ------------------------------------------------------------
	// Expansion SetIsEditingMarker
	// ------------------------------------------------------------
	void SetIsEditingMarker(bool state)
	{
		m_IsEditingMarker = state;
	}

	// ------------------------------------------------------------
	// Expansion IsEditingMarker
	// ------------------------------------------------------------
	bool IsEditingMarker()
	{
		return m_IsEditingMarker;
	}

	// ------------------------------------------------------------
	// Expansion IsEditingMarker
	// ------------------------------------------------------------
	override void InitMapItem(EntityAI item)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "InitMapItem");
#endif

		super.InitMapItem(item);
		m_Map = ItemMap.Cast(item);
	}

	// ------------------------------------------------------------
	// Expansion CloseMapMenu
	// ------------------------------------------------------------
	void CloseMapMenu(bool destroy = false)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "CloseMapMenu");
#endif

		if (m_Map)
		{
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			if (player)
			{
				player.SetMapClosingSyncSet(false); //map is closing, server needs to be notified - once
			}
		}

		if (destroy)
			Close();
		else
			GetGame().GetUIManager().HideScriptedMenu( this );  //! Don't close, we do not want to have to redraw all the markers next time we open it
	}

#ifndef DAYZ_1_18
	//! 1.19
	// ------------------------------------------------------------
	// 1.19 Vanilla addition
	// ------------------------------------------------------------
	protected void SetCompassUIVisibility(bool pState)
	{
		if (m_ToolsCompassArrow)
		{
			m_ToolsCompassArrow.Show(pState);
		}

		if (m_ToolsCompassAzimuth)
		{
			m_ToolsCompassAzimuth.Show(pState);
		}
	}

	// ------------------------------------------------------------
	// 1.19 Vanilla addition
	// ------------------------------------------------------------
	protected void SetGPSMarkerVisibility(bool pState)
	{
		if (m_GPSMarker)
		{
			m_GPSMarker.Show(pState);
		}
	}

	// ------------------------------------------------------------
	// 1.19 Vanilla addition
	// ------------------------------------------------------------
	protected void SetGPSDirectionVisibility(bool pState)
	{
		if (m_GPSMarkerArrow)
		{
			m_GPSMarkerArrow.Show(pState);
		}
	}

	// ------------------------------------------------------------
	// 1.19 Vanilla addition
	// ------------------------------------------------------------
	protected void SetUpperLegendVisibility(bool pState)
	{
		if (m_UpperLegendContainer)
		{
			m_UpperLegendContainer.Show(pState);
		}

		if (m_LegendResizer)
		{
			m_LegendResizer.ResizeParentToChild();
		}
	}

	// ------------------------------------------------------------
	// 1.19 Vanilla addition
	// ------------------------------------------------------------
	protected void RenderScaleRuler()
	{
		float sizeYShift = 8;
		float segmentLength = m_ToolScaleCellSizeCanvasWidth / SCALE_RULER_NUM_SEGMENTS;
		int lineColor = FadeColors.LIGHT_GREY;

		for (int i = 1; i <= SCALE_RULER_NUM_SEGMENTS; i++)
		{
			lineColor = FadeColors.LIGHT_GREY;
			if (i % 2 == 0)
			{
				lineColor = FadeColors.DARK_RED;
			}

			if (i == 1) //! 1st segment
			{
				m_ToolsScaleCellSizeCanvas.DrawLine(0, sizeYShift, segmentLength, sizeYShift, SCALE_RULER_LINE_WIDTH, lineColor);
			}
			else if (i == SCALE_RULER_NUM_SEGMENTS) //! last segment
			{
				m_ToolsScaleCellSizeCanvas.DrawLine(segmentLength * (SCALE_RULER_NUM_SEGMENTS - 1), sizeYShift, segmentLength * SCALE_RULER_NUM_SEGMENTS, sizeYShift, SCALE_RULER_LINE_WIDTH, lineColor);
			}
			else
			{
				m_ToolsScaleCellSizeCanvas.DrawLine(segmentLength * (i - 1), sizeYShift, segmentLength * i, sizeYShift, SCALE_RULER_LINE_WIDTH, lineColor);
			}
		}
	}

	// ------------------------------------------------------------
	// 1.19 Vanilla addition
	// ------------------------------------------------------------
	protected void ProcessDistanceAndUnits(float num, out float dist, out string units)
	{
		if (num >= 901)
		{
			num *= 0.001;
			num = Math.Round(num * 10) * 0.1;
			dist = num;
			units = "km";
		}
		else if (num >= 100 && num <= 900)
		{
			num = Math.Ceil(num * 0.1) * 10;
			dist = num;
			units = "m";
		}
		else if (num >= 1)
		{
			num = Math.Floor(num);
			dist = num;
			units = "m";
		}
		else
		{
			num = Math.Ceil(num * 10);
			dist = num;
			units = "cm";
		}
	}
#endif
};