modded class DebugMonitor
{
	protected TextWidget m_Expansion_VersionLabel;
	protected TextWidget m_Expansion_FPSLabel;
	protected TextWidget m_Expansion_HealthLabel;
	protected TextWidget m_Expansion_BloodLabel;
	protected TextWidget m_Expansion_DmgSourceLabel;
	protected TextWidget m_Expansion_MapTileLabel;

	void DebugMonitor()
	{
		m_Expansion_VersionLabel = TextWidget.Cast(m_WidgetRoot.FindAnyWidget("VersionLabel"));
		m_Expansion_FPSLabel = TextWidget.Cast(m_WidgetRoot.FindAnyWidget("FPSLabel"));
		m_Expansion_HealthLabel = TextWidget.Cast(m_WidgetRoot.FindAnyWidget("HealthLabel"));
		m_Expansion_BloodLabel = TextWidget.Cast(m_WidgetRoot.FindAnyWidget("BloodLabel"));
		m_Expansion_DmgSourceLabel = TextWidget.Cast(m_WidgetRoot.FindAnyWidget("DmgSourceLabel"));
		m_Expansion_MapTileLabel = TextWidget.Cast(m_WidgetRoot.FindAnyWidget("MapTileLabel"));
	}

	override void SetFramerate(float current, float min, float max, float avg)
	{
		if (ExpansionWorld.s_DebugMonitor_ShowServerStats)
			m_Expansion_FPSLabel.SetText("SERVER FPS:");
		else
			m_Expansion_FPSLabel.SetText("FPS:");

		super.SetFramerate(current, min, max, avg);
	}

	override string CalculateMapTile(vector pos)
	{
		if (ExpansionWorld.s_DebugMonitor_ShowServerStats)
		{
			m_Expansion_MapTileLabel.SetText("FTO LEVEL:");
			return eAIBase.s_eAI_FTO.ToString();
		}

		m_Expansion_MapTileLabel.SetText("MAP TILE:");
		return super.CalculateMapTile(pos);
	}

	void Expansion_SetVersion()
	{
		m_Expansion_VersionLabel.SetText("EXECUTABLE:");
		string version;
		g_Game.GetVersion(version);
		m_VersionValue.SetText(" " + version);
	}

	void Expansion_SetAICount()
	{
		m_Expansion_VersionLabel.SetText("EAI COUNT:");
		m_VersionValue.SetText(" " + ExpansionWorld.s_AI_Alive_Count.ToString() + " | PATROLS: " + eAIDynamicPatrol.s_PatrolCount);
	}

	void Expansion_Update()
	{
		float currFPS;
		float minFPS, maxFPS, avgFPS;

		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());

		if (ExpansionWorld.s_DebugMonitor_ShowServerStats)
		{
			m_Expansion_HealthLabel.SetText("EAI UPD:");
			m_Expansion_BloodLabel.SetText("EAI UPD DT:");
			m_Expansion_DmgSourceLabel.SetText("EAI UPD COST:");

			string aiUpd_Avg = ExpansionStatic.FormatFloat(ExpansionWorld.s_AIUpdate_Avg, 1);
			m_HealthValue.SetText(" " + ExpansionWorld.s_AIUpdate_Current.ToString() + " | Avg " + aiUpd_Avg);

			string aiUpdDT_Cur = ExpansionStatic.FormatFloat(ExpansionWorld.s_AIUpdateDT_Current, 2);
			string aiUpdDT_Avg = ExpansionStatic.FormatFloat(ExpansionWorld.s_AIUpdateDT_Avg, 2);
			m_BloodValue.SetText(" " + aiUpdDT_Cur + " ms | Avg " + aiUpdDT_Avg + " ms");

			string aiUpdCost_Cur = ExpansionStatic.FormatFloat(ExpansionWorld.s_AIUpdateCost_Current, 4);
			string aiUpdCost_Avg = ExpansionStatic.FormatFloat(ExpansionWorld.s_AIUpdateCost_Avg, 4);
			m_DmgSourceValue.SetText(" " + aiUpdCost_Cur + " ms | Avg " + aiUpdCost_Avg + " ms");

			if (player)
				SetPosition(MiscGameplayFunctions.TruncateVec(player.GetPosition(),1));

			Expansion_SetAICount();

			currFPS = ExpansionWorld.s_ServerFPS_Current;
			minFPS = ExpansionWorld.s_ServerFPS_Min;
			maxFPS = ExpansionWorld.s_ServerFPS_Max;
			avgFPS = ExpansionWorld.s_ServerFPS_Avg;
		}
		else
		{
			//! Vanilla, see MissionGameplay::UpdateDebugMonitor

			m_Expansion_HealthLabel.SetText("HEALTH:");
			m_Expansion_BloodLabel.SetText("BLOOD:");
			m_Expansion_DmgSourceLabel.SetText("LAST DAMAGE:");

			if (player)
			{
				DebugMonitorValues values = player.GetDebugMonitorValues();
				if (values)
				{
					SetHealth(values.GetHealth());
					SetBlood(values.GetBlood());
					SetLastDamage(values.GetLastDamage());
					SetPosition(MiscGameplayFunctions.TruncateVec(player.GetPosition(),1));
				}
			}

			Expansion_SetVersion();

			currFPS = g_Game.GetAvgFPS(10); // Not using last, but average of last x to prevent jitter
			g_Game.GetFPSStats(minFPS, maxFPS, avgFPS);
		}

		SetFramerate(currFPS, minFPS, maxFPS, avgFPS);
	}

	override protected void SetFramerateText(TextWidget widget, float value)
	{
		if (ExpansionWorld.s_DebugMonitor_ShowServerStats)
		{
			World world = g_Game.GetWorld();

			if (value > world.GetServerFpsWarningThreshold())
				widget.SetColor(m_FPSTextDefaultColor);
			else if (value > world.GetServerFpsCriticalThreshold())
				widget.SetColor(0xffffcc00);
			else
				widget.SetColor(COLOR_RED);
		
			widget.SetTextFormat("%1", Math.Round(value));
		}
		else
		{
			super.SetFramerateText(widget, value);
		}
	}
}
