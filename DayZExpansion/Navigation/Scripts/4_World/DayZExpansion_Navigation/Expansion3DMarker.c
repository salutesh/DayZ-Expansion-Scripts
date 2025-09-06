/**
 * Expansion3DMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion3DMarker: ScriptedWidgetEventHandler
{
	private Widget m_LayoutRoot;
	private Widget m_Frame;
	private TextWidget m_Text_Distance;
	private TextWidget m_Text_Name;
	private ImageWidget m_Image_Icon;

	private ExpansionMarkerData m_MarkerData;

	private int m_Transparency;
	private int m_TransparencyMin;
	private int m_TransparencyMax;
	private float m_TransparencyLerp;

	private ExpansionMarkerModule m_MarkerModule;
	#ifdef EXPANSIONMODGROUPS
	private ExpansionPartyModule m_PartyModule;
	#endif
	
	private string m_LayoutPath;
	
	private bool m_ShowQuickName = true;
	private bool m_ShowQuickDistance = true;

	private bool m_ShowMemberName = true;
	private bool m_ShowMemberDistance = true;
	
	private bool m_ShowServerName = true;
	private bool m_ShowServerDistance = true;

	private bool m_ShowPersonalDistance = true;
	
	private bool m_Show3DPartyMemberIcon = true;

	private int m_MaxDistance3DGlobalMarkers;
	private int m_MaxDistance3DClientMarkers;
	private int m_MaxDistance3DPartyMarkers;
	private int m_MaxDistance3DPlayerMarkers;

	private bool m_ShowMarkerTextColor;
	
	private int m_FadeDistance;
	private int m_AdditionalMaxDistance;

	private PlayerBase m_Player;
	
	void Expansion3DMarker(ExpansionMarkerData data = null)
	{
		GetSettings();
		
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets(m_LayoutPath);
		OnWidgetScriptInit(m_LayoutRoot);

		if (data)
		{
			SetMarkerData(data);
		}

		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
		#ifdef EXPANSIONMODGROUPS
		CF_Modules<ExpansionPartyModule>.Get(m_PartyModule);
		#endif
		
		GetExpansionClientSettings().SI_UpdateSetting.Insert(OnSettingChanged);

		m_Player = PlayerBase.Cast(GetGame().GetPlayer());
	}

	void ~Expansion3DMarker()
	{
		GetExpansionClientSettings().SI_UpdateSetting.Remove(OnSettingChanged);
		
		if ( m_LayoutRoot )
		{
			m_LayoutRoot.Show(false);
			m_LayoutRoot.Unlink();
		}
	}

	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}

	void OnWidgetScriptInit(Widget w)
	{
		m_LayoutRoot = w;

		Class.CastTo(m_Image_Icon, m_LayoutRoot.FindAnyWidget("MarkerImage"));
		Class.CastTo(m_Text_Name, m_LayoutRoot.FindAnyWidget("MarkerText"));
		Class.CastTo(m_Text_Distance, m_LayoutRoot.FindAnyWidget("MarkerDistance"));
		
		m_LayoutRoot.SetHandler(this);
	}
	
	void SetParent(Widget parent)
	{
		if (m_LayoutRoot)
		{
			parent.AddChild(m_LayoutRoot);
		}
	}

	/**
	 * /return	boolean		False to remove the marker
	 */
	bool Update(float timeslice)
	{
		if (!m_LayoutRoot || !m_MarkerData || !m_Player)
		{
			return false;
		}

		ExpansionMapMarkerType markerType = m_MarkerData.GetType();

		#ifdef EXPANSIONMODGROUPS
		switch (markerType)
		{
			case ExpansionMapMarkerType.PARTY:
			case ExpansionMapMarkerType.PLAYER:
				if (!m_PartyModule.HasParty())
				{
					return false;
				}
				break;
		}
		#endif

		if (markerType != ExpansionMapMarkerType.PARTY_QUICK)
		{
			if (!m_MarkerData.Is3D() || !m_MarkerData.IsWorldVisible())
			{
				return false;
			}

			if (!m_MarkerModule.IsWorldVisible(markerType))
			{
				return false;
			}
		}

		Hud inGameHUD = GetGame().GetMission().GetHud();
		if (inGameHUD && !inGameHUD.Expansion_CanShowHUDElements())
		{
			m_LayoutRoot.Show(false);
			return true;
		}

		vector position = m_MarkerData.GetAdjustedPosition();
		vector screen_position = GetGame().GetScreenPosRelative(position);
		if (screen_position[0] >= 1 || screen_position[0] <= 0 || screen_position[1] >= 1 || screen_position[1] <= 0 || screen_position[2] <= 0)
		{
			m_LayoutRoot.Show(false);
			return true;
		}

		int color = m_MarkerData.GetColor();

		m_TransparencyMax = (color >> 24) & 0xFF;

		float dist = vector.Distance(screen_position, Vector(0.5, 0.5, screen_position[2]));
		float minTransparencyDist = 0.05;
		if (CheckPlayerZoom())
		{
			minTransparencyDist = 0.1;
		}
		else if (m_Player.IsInIronsights() || m_Player.IsInOptics())
		{
			minTransparencyDist = 0.15;
		}

		m_Transparency = ExpansionMath.LinearConversion(0.0125, minTransparencyDist, dist, m_TransparencyMax, m_TransparencyMin);
		
		vector cameraPos = GetGame().GetCurrentCameraPosition();
		vector markerDir =  vector.Direction(cameraPos, position);
		float distance = markerDir.Length();
		float transparencyOverride;
		
		switch (markerType)
		{
			case ExpansionMapMarkerType.SERVER:
				m_Text_Name.Show(m_ShowServerName);
				m_Text_Distance.Show(m_ShowServerDistance);
				if (Is3DMarkerFarAway(distance, m_MaxDistance3DGlobalMarkers, transparencyOverride))
				{
					return true;
				}
				break;

			case ExpansionMapMarkerType.PERSONAL:
				m_Text_Distance.Show(m_ShowPersonalDistance);
				if (Is3DMarkerFarAway(distance, m_MaxDistance3DClientMarkers, transparencyOverride))
				{
					return true;
				}
				break;

			#ifdef EXPANSIONMODGROUPS
			case ExpansionMapMarkerType.PARTY_QUICK:
				m_Text_Name.Show(m_ShowQuickName);
				m_Text_Distance.Show(m_ShowQuickDistance);
				if (Is3DMarkerFarAway(distance, m_MaxDistance3DPartyMarkers, transparencyOverride))
				{
					return true;
				}
				break;

			case ExpansionMapMarkerType.PARTY:
				if (Is3DMarkerFarAway(distance, m_MaxDistance3DPartyMarkers, transparencyOverride))
				{
					return true;
				}
				break;

			case ExpansionMapMarkerType.PLAYER:
				m_Text_Name.Show(m_ShowMemberName);
				m_Text_Distance.Show(m_ShowMemberDistance);
				m_Image_Icon.Show(m_Show3DPartyMemberIcon);
				if (Is3DMarkerFarAway(distance, m_MaxDistance3DPlayerMarkers, transparencyOverride))
				{
					return true;
				}
				break;
			#endif
		}

		vector cameraDir = GetGame().GetCurrentCameraDirection();
		vector markerDirNorm = markerDir.Normalized();

		//! dotprod tolerance 10 degrees = 0.984808 = Math.Cos(10 * Math.DEG2RAD)
		float dot = vector.Dot(markerDirNorm, cameraDir);
		if (dot > 0.984808)
			CheckLOSBlocked(cameraPos, markerDirNorm, distance, transparencyOverride);

		m_TransparencyLerp = Math.Lerp(m_TransparencyLerp, Math.Min(m_Transparency, transparencyOverride), timeslice * 6);

		int markerColorA = 0;
		int markerColorR = 0;
		int markerColorG = 0;
		int markerColorB = 0;
		ExpansionStatic.IntToARGB(color, markerColorA, markerColorR, markerColorG, markerColorB);

		int colorText;
		if (m_ShowMarkerTextColor)
		{
			colorText = ARGB(m_TransparencyLerp, markerColorR, markerColorG, markerColorB);
		}
		else
		{
			colorText = ARGB(m_TransparencyLerp, 255, 255, 255);
		}

		m_Text_Name.SetColor(colorText);
		
		//! Set distance
		m_Text_Distance.SetText(Math.Ceil(distance).ToString() + "m");
		m_Text_Distance.SetColor(colorText);
		
		m_LayoutRoot.Show(true);
		m_LayoutRoot.SetPos(screen_position[0], screen_position[1]);

		m_Image_Icon.SetColor(ARGB(m_TransparencyLerp, markerColorR, markerColorG, markerColorB));

		return true;
	}

	bool CheckPlayerZoom()
	{
		switch (m_Player.GetEyeZoomLevel())
		{
			case ECameraZoomType.SHALLOW:
				m_FadeDistance = 1750;
				return true;
			case ECameraZoomType.NORMAL:
				m_FadeDistance = 3500;
				return true;
			default:
				m_FadeDistance = 50;
				break;
		}

		return false;
	}

	bool Is3DMarkerFarAway(float distance, float maxDist, out float transparency)
	{
		if (distance > maxDist)
		{
			int fadedist = 50;
			float value = maxDist + m_FadeDistance - distance;
			transparency = ExpansionMath.LinearConversion(0, m_FadeDistance, value, 0, m_TransparencyMax);
			if (transparency == 0.0)
			{
				m_LayoutRoot.Show(false);
				return true;
			}
		}
		else
		{
			transparency = m_TransparencyMax;
		}

		return false;
	}

	/**
	 * @brief check if a player or creature is blocking LOS to marker
	 */
	bool CheckLOSBlocked(vector rayStart, vector rayDir, float markerDist, out float transparencyOverride)
	{
		vector contactPos;
		vector contactDir;
		int contactComponent;

		//! Default values so the odds of the marker fading 
		//! due to a player by mistakes are lower
		float radius = 0.05;
		float scanDist = 550;
		float decaySpeed = 0.15;
		
		if (m_Player.IsInIronsights() || m_Player.IsInOptics())
		{
			scanDist = 1100;
			decaySpeed = 0.35;
		}

		vector rayEnd = rayStart + rayDir * scanDist;
		set<Object> results = new set<Object>;
	
		if (DayZPhysics.RaycastRV(rayStart, rayEnd, contactPos, contactDir, contactComponent, results, null, m_Player, false, false, ObjIntersectGeom, radius))
		{
			foreach (Object result: results)
			{
				EntityAI entity;
				if (Class.CastTo(entity, result))
				{
					result = entity.GetHierarchyRoot();
				}

				if (result.IsMan() || result.IsDayZCreature())
				{
					//! If marker is on target, don't override transparency
					if (Math.AbsFloat(markerDist - vector.Distance(rayStart, contactPos)) < 1.5)
						return false;

					vector screen_position = GetGame().GetScreenPosRelative( contactPos );
					float dist = vector.Distance( screen_position, Vector( 0.5, 0.5, screen_position[2] ) );
					transparencyOverride = ExpansionMath.LinearConversion( 0.0125, decaySpeed, dist, 0, m_TransparencyMin );
					return true;
				}
			}
		}

		return false;
	}

	void SetMarkerData(ExpansionMarkerData data)
	{
		m_MarkerData = data;

		Refresh();
	}

	void Refresh()
	{
		RefreshAlphaMinColor();
		m_Text_Name.SetColor(ARGB(m_TransparencyMax, 255, 255, 255));
		m_Text_Distance.SetColor(ARGB(m_TransparencyMax, 255, 255, 255));

		string icon = m_MarkerData.GetIcon();
		if (icon != string.Empty)
		{
			m_Image_Icon.LoadImageFile(0, icon);
			m_Image_Icon.SetImage(0);
			m_Image_Icon.Show(true);
		}
		else
		{
			m_Image_Icon.Show(false);
		}

		m_Text_Name.SetText(m_MarkerData.GetName());
	}

	ExpansionMarkerData GetMarkerData()
	{
		return m_MarkerData;
	}
	
	void RefreshAlphaMinColor()
	{
		m_TransparencyMin = GetExpansionClientSettings().AlphaColorLookAtMinimum;
		m_Transparency = Math.Clamp(m_Transparency, m_TransparencyMin, m_TransparencyMax);
	}

	void OnSettingChanged()
	{
		if (m_LayoutRoot)
		{	
			m_LayoutRoot.Unlink();
		}

		GetSettings();
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets(m_LayoutPath);
		OnWidgetScriptInit(m_LayoutRoot);

		Refresh();
	}

	void GetSettings()
	{
		if (GetExpansionClientSettings())
		{
			ExpansionClientUIMarkerSize markersize = GetExpansionClientSettings().MarkerSize;
			switch (markersize)
			{
				case ExpansionClientUIMarkerSize.VERYSMALL:
					m_LayoutPath = "DayZExpansion/Navigation/GUI/layouts/expansion_dynamic_marker_16.layout";
					break;
				case ExpansionClientUIMarkerSize.SMALL:
					m_LayoutPath = "DayZExpansion/Navigation/GUI/layouts/expansion_dynamic_marker_24.layout";
					break;
				case ExpansionClientUIMarkerSize.MEDIUM:
					m_LayoutPath = "DayZExpansion/Navigation/GUI/layouts/expansion_dynamic_marker_32.layout";
					break;
				case ExpansionClientUIMarkerSize.LARGE:
					m_LayoutPath = "DayZExpansion/Navigation/GUI/layouts/expansion_dynamic_marker_40.layout";
					break;
			}
		}
		
		if (GetExpansionSettings())
		{
			m_ShowServerName = GetExpansionSettings().GetMap().ShowNameOnServerMarkers;
			m_ShowServerDistance = GetExpansionSettings().GetMap().ShowDistanceOnServerMarkers;
			m_ShowPersonalDistance = GetExpansionSettings().GetMap().ShowDistanceOnPersonalMarkers;
			m_Show3DPartyMemberIcon = GetExpansionClientSettings().Show3DPartyMemberIcon;
			m_Show3DPartyMemberIcon = GetExpansionClientSettings().Show3DPartyMemberIcon;
			m_MaxDistance3DGlobalMarkers = GetExpansionClientSettings().MaxDistance3DGlobalMarkers;
			m_MaxDistance3DClientMarkers = GetExpansionClientSettings().MaxDistance3DClientMarkers;
			m_ShowMarkerTextColor = GetExpansionClientSettings().ShowMarkerTextColor;
			#ifdef EXPANSIONMODGROUPS
			m_ShowQuickName = GetExpansionSettings().GetParty().ShowNameOnQuickMarkers;
			if (m_ShowQuickName)
			{
				m_ShowQuickName = GetExpansionClientSettings().ShowNameQuickMarkers;
			}

			m_ShowQuickDistance = GetExpansionSettings().GetParty().ShowDistanceUnderQuickMarkers;
			if (m_ShowQuickDistance)
			{
				m_ShowQuickDistance = GetExpansionClientSettings().ShowDistanceQuickMarkers;
			}
			
			m_ShowMemberName = GetExpansionSettings().GetParty().ShowNameOnPartyMembersMarkers;
			if (GetExpansionSettings().GetParty().ShowNameOnPartyMembersMarkers)
			{
				m_ShowMemberName = GetExpansionClientSettings().ShowMemberNameMarker;
			}

			m_ShowMemberDistance = GetExpansionSettings().GetParty().ShowDistanceUnderPartyMembersMarkers;
			if (m_ShowMemberDistance)
			{
				m_ShowMemberDistance = GetExpansionClientSettings().ShowMemberDistanceMarker;
			}

			m_MaxDistance3DPartyMarkers = GetExpansionClientSettings().MaxDistance3DPartyMarkers;
			m_MaxDistance3DPlayerMarkers = GetExpansionClientSettings().MaxDistance3DPlayerMarkers;
			#endif
		}
	}
}