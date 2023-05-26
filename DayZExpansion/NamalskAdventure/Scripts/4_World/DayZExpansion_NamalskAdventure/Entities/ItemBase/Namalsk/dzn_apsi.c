/**
 * dzn_apsi.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/*#ifdef NAMALSK_SURVIVAL
class ExpansionAnomalyHUDMarker: ScriptView
{
	protected ref ExpansionAnomalyHUDMarkerController m_HudMarkerController;
	protected ref ExpansionAnomalyHUD m_AnomalyHud;
	protected string m_AnomalyType;
	protected vector m_AnomalyPos;
	
	protected ref Timer m_UpdateTimer;

	void ExpansionAnomalyHUDMarker(ExpansionAnomalyHUD hud, string anomalyType, vector anomalyPos)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_HudMarkerController = ExpansionAnomalyHUDMarkerController.Cast(GetController());
		
		m_AnomalyHud = hud;
		m_AnomalyType = anomalyType;
		m_AnomalyPos = anomalyPos;
		
		CreateUpdateTimer();
		
		hud.GetMarkerPanelWidget().AddChild(GetLayoutRoot(), true);
	}
	
	void ~ExpansionAnomalyHUDMarker()
	{
		DestroyUpdateTimer();
	}
	
	override typename GetControllerType()
	{
		return ExpansionAnomalyHUDMarkerController;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/NamalskAdventure/GUI/layouts/expansion_anomaly_hud_marker.layout";
	}
	
	void SetWidgetPositionFromWorldPosition()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	   	float x, y;
		m_AnomalyHud.GetMarkerPanelWidget().GetParent().GetPos(x, y);
		vector parentPos = Vector(x, y, 0);
		float width, height;
		m_AnomalyHud.GetMarkerPanelWidget().GetSize(width, height);
		vector parentSize = Vector(width, height, 0);
		
		// Get player's camera direction and position
		vector camDir = GetGame().GetCurrentCameraDirection();
		vector playerPos = GetGame().GetPlayer().GetPosition();
		
		// Calculate the vector from player's position to anomaly position
		vector toAnomaly = m_AnomalyPos - playerPos;
		
		// Rotate the vector to align with the player's camera direction
		vector alignedToAnomaly = vector.Dot(toAnomaly, camDir) * camDir;
		
		// Get the screen position of the aligned vector
		//vector screenPos = GetGame().GetScreenPos(alignedToAnomaly);
		vector screenPos = GetGame().GetScreenPosRelative(alignedToAnomaly);
		
		int screen_width, screen_height;
		GetGame().GetScreenSize(screen_width, screen_height);
		vector localPos = screenPos - parentPos;
		localPos[0] = localPos[0] / screen_width * parentSize[0];
		localPos[1] = localPos[1] / screen_height * parentSize[1];
		
		GetLayoutRoot().SetPos(localPos[0], localPos[1], true);
		
		ExDebugPrint("::SetWidgetPositionFromWorldPosition - Player position:" + playerPos.ToString());
		ExDebugPrint("::SetWidgetPositionFromWorldPosition - Player camera dir:" + camDir.ToString());
		ExDebugPrint("::SetWidgetPositionFromWorldPosition - Aligned anormaly position:" + alignedToAnomaly.ToString());
		ExDebugPrint("::SetWidgetPositionFromWorldPosition - Parent position:" + parentPos.ToString());
		ExDebugPrint("::SetWidgetPositionFromWorldPosition - Parent widget size Width:" + width + " | Height: " + height);
		ExDebugPrint("::SetWidgetPositionFromWorldPosition - Anomaly world to screen position:" + screenPos.ToString());
		ExDebugPrint("::SetWidgetPositionFromWorldPosition - Screen size Width:" + screen_width + " | Height: " + screen_height);
		ExDebugPrint("::SetWidgetPositionFromWorldPosition - Local position X:" + localPos + " | Y: " + localPos);
	}
	
	protected float GetUpdateTickRate()
	{
		return 0.1;
	}
	
	protected void CreateUpdateTimer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!m_UpdateTimer && GetUpdateTickRate() != -1)
		{
			m_UpdateTimer = new Timer(CALL_CATEGORY_GUI);
			m_UpdateTimer.Run(GetUpdateTickRate(), this, "SetWidgetPositionFromWorldPosition", NULL, true);
		}
	}
	
	protected void DestroyUpdateTimer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (m_UpdateTimer)
		{
			m_UpdateTimer.Stop();
			m_UpdateTimer = null;
		}
	}
	
	protected void ExDebugPrint(string text)
	{
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	}
};
class ExpansionAnomalyHUDMarkerController: ViewController {};
class ExpansionAnomalyHUD: ScriptView
{
	protected ref ExpansionAnomalyHUDController m_HudController;
	protected Widget MarkerPanel;
	protected ImageWidget RadarCircle;
	protected vector m_ClosestAnomalyPos;
	protected ref array<ref ExpansionAnomalyHUDMarker> m_Markers;

	void ExpansionAnomalyHUD()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_HudController = ExpansionAnomalyHUDController.Cast(GetController());
		
		m_Markers = new array<ref ExpansionAnomalyHUDMarker>;
	}
	
	override typename GetControllerType()
	{
		return ExpansionAnomalyHUDController;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/NamalskAdventure/GUI/layouts/expansion_anomaly_hud.layout";
	}
	
	void Destroy()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!g_Game)
			return;

		delete this;
	}

	void UpdateRadar()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		ref map<string, vector> anomalies = new map<string, vector>;
		vector camDir = GetGame().GetCurrentCameraDirection();
		//! Update rotation of the radar circle image widget
		RadarCircle.SetRotation(0, 0, Math.Round(Math.NormalizeAngle(camDir.VectorToAngles()[0])), true);
		
		const float detectionRange = 50.0; //! 50 meters
		const vector playerCamPos = GetGame().GetCurrentCameraPosition(); // Current players camera position
		//! Find closest anomaly to player
		vector closestPosition;
		
		auto node = Expansion_Anomaly_Base.s_Expansion_AllAnomalies.m_Head;
		while (node)
		{
		    Expansion_Anomaly_Base anomaly = node.m_Value;
		    ExDebugPrint("::UpdateRadar - Anomaly: " + anomaly.ToString() + " | Position: " + anomaly.GetPosition());

			anomalies.Insert(anomaly.GetType(), anomaly.GetPosition());
		    node = node.m_Next;
		}
		
		if (!m_Markers)
			m_Markers = new array<ref ExpansionAnomalyHUDMarker>;
		
		if (m_Markers.Count() != anomalies.Count())
		{
			m_Markers.Clear();
		}
		else
		{
			return;
		}
		
		foreach (string anomalyType, vector anomalyPos: anomalies)
		{
			ExpansionAnomalyHUDMarker marker = new ExpansionAnomalyHUDMarker(this, anomalyType, anomalyPos);
			m_Markers.Insert(marker);
		}
	}
	
	Widget GetMarkerPanelWidget()
	{
		return MarkerPanel;
	}

	protected void ExDebugPrint(string text)
	{
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	}
};
class ExpansionAnomalyHUDController: ViewController {};

modded class dzn_apsi
{
	protected ref ExpansionAnomalyHUD m_AnomalyHUD;
	protected vector m_LastClosestAnomalyPos = vector.Zero;
	protected float m_UpdateQueueTimer;
	protected float m_LastUpdateTimer;
	protected const float UPDATE_DELAY = 5000.0;

	override void InitHud()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.InitHud();

		if (GetGame().IsClient())
		{
			if (m_AnomalyHUD)
			{
				m_AnomalyHUD.Destroy();
			}
			
			m_AnomalyHUD = new ExpansionAnomalyHUD();
		}
	}
	
	override void UpdateFrame()
	{
		super.UpdateFrame();
		
		if (!m_AnomalyHUD) 
		{
		    return;
		}
		
		m_UpdateQueueTimer += GetGame().GetTickTime();
		if (m_UpdateQueueTimer >= m_LastUpdateTimer + UPDATE_DELAY)
		{
			m_AnomalyHUD.UpdateRadar();
			m_LastUpdateTimer = m_UpdateQueueTimer;
		}
	}
	
	protected void ExDebugPrint(string text)
	{
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	}
};
#endif*/
