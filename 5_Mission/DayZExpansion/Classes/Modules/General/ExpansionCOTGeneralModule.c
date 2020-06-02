class ExpansionCOTGeneralModule: JMRenderableModuleBase
{
	void ExpansionCOTGeneralModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.General.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.General.Apply" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.General.View" );
	}
	
	override void OnSettingsUpdated()
	{
		super.OnSettingsUpdated();
	}

	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();
	}

	override void OnMissionFinish()
	{
		super.OnMissionFinish();
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/General_Menu.layout";
	}

	override string GetTitle()
	{
		return "General";
	}
	
	override string GetIconName()
	{
		return "EX";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override int GetRPCMin()
	{
		return ExpansionCOTGeneralModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTGeneralModuleRPC.COUNT;
	}

	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
		case ExpansionCOTGeneralModuleRPC.Update:
			RPC_Update( ctx, sender, target );
			break;
		}
	}

	void Update( ref ExpansionGeneralSettings settings )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( settings );
			rpc.Send( NULL, ExpansionCOTGeneralModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetExpansionSettings().GetGeneral().Update( settings );
		}
	}

	private void RPC_Update( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionGeneralSettings settings;
		if ( !ctx.Read( settings ) )
			return;

		if ( !GetPermissionsManager().HasPermission( "Expansion.General.View", senderRPC ) )
			return;

		GetExpansionSettings().GetGeneral().Update( settings );
	}
}