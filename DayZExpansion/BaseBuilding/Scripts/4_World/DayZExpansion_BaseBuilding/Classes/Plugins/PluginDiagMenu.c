#ifdef DIAG_DEVELOPER
modded class PluginDiagMenu
{
	static bool s_Expansion_BBSettingsChangeInProgress;

	protected int m_Expansion_BaseBuilding_RootMenuID;
	static int EXPANSION_REMEMBER_CODELOCK_PIN;
	static int EXPANSION_REMEMBER_CODELOCK_PIN_RPC = "EXPANSION_REMEMBER_CODELOCK_PIN_RPC".Hash();
	
	override protected void RegisterModdedDiagsIDs()
	{
		super.RegisterModdedDiagsIDs();

		m_Expansion_BaseBuilding_RootMenuID = GetModdedDiagID();
		EXPANSION_REMEMBER_CODELOCK_PIN = GetModdedDiagID();
	}

	override protected void RegisterModdedDiags()
	{
		super.RegisterModdedDiags();

		DiagMenu.RegisterMenu(m_Expansion_BaseBuilding_RootMenuID, "EXP BASEBUILDING", GetModdedRootMenu());
		DiagMenu.RegisterBool(EXPANSION_REMEMBER_CODELOCK_PIN, "", "Remember CodeLock PIN", m_Expansion_BaseBuilding_RootMenuID);

	#ifndef SERVER
		ExpansionSettings.SI_BaseBuilding.Insert(Expansion_OnBBSettingsChanged);
	#endif
	}

	override void OnRPC(PlayerBase player, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(player, rpc_type, ctx);

		switch (rpc_type)
		{
			case EXPANSION_REMEMBER_CODELOCK_PIN_RPC:
				auto param = new Param1<bool>(false);
				if (ctx.Read(param))
				{
					EXError.Info(this, "GetExpansionSettings().GetBaseBuilding().RememberCode=" + param.param1.ToString());
					GetExpansionSettings().GetBaseBuilding().RememberCode = param.param1;
					Expansion_UpdateBBSettings();
				}
				break;
		}
	}

	static void Expansion_UpdateBBSettings()
	{
		s_Expansion_BBSettingsChangeInProgress = true;
		ExpansionSettings.SI_BaseBuilding.Invoke();
		s_Expansion_BBSettingsChangeInProgress = false;
	}

	void Expansion_OnBBSettingsChanged()
	{
		if (s_Expansion_BBSettingsChangeInProgress)
			return;

		s_Expansion_BBSettingsChangeInProgress = true;
		auto settings = GetExpansionSettings().GetBaseBuilding();
		DiagMenu.SetValue(EXPANSION_REMEMBER_CODELOCK_PIN, settings.RememberCode);  //! Setting the value fires the callback
		s_Expansion_BBSettingsChangeInProgress = false;
	}
}


modded class PluginDiagMenuClient
{
	override protected void BindCallbacks()
	{
		super.BindCallbacks();

		DiagMenu.BindCallback(EXPANSION_REMEMBER_CODELOCK_PIN, Expansion_SetRememberCode);
	}

	static void Expansion_SetRememberCode(bool enabled, int id)
	{
		if (s_Expansion_BBSettingsChangeInProgress)
			return;

		EXError.Info(null, "PluginDiagMenuClient::Expansion_SetRememberCode enabled=" + enabled.ToString() + " id=" + id);

		auto settings = GetExpansionSettings().GetBaseBuilding();
		settings.RememberCode = enabled;
		Expansion_UpdateBBSettings();

		Man player = g_Game.GetPlayer();
		auto param = new Param1<bool>(enabled);
		g_Game.RPCSingleParam(player, EXPANSION_REMEMBER_CODELOCK_PIN_RPC, param, true, player.GetIdentity());
	}
}
#endif
