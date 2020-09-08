/**
 * ExpansionESPModificationModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionESPModificationModuleRPC
{
	INVALID = 20720,
	RequestCode,
	RemoveCode,
	COUNT
};

modded class JMESPModule
{
	ref JMESPMeta EXP_GetMeta( Object target )
	{
		return m_MappedESPObjects.Get( target );
	}
};

class ExpansionESPModificationModule : JMModuleBase
{
	private JMESPModule m_BaseModule;

	void ExpansionESPModificationModule()
	{
		GetPermissionsManager().RegisterPermission( "ESP.Object.BaseBuilding.Codelock" );
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		
		m_BaseModule = JMESPModule.Cast( GetModuleManager().GetModule( JMESPModule ) );
	}

	override int GetRPCMin()
	{
		return ExpansionESPModificationModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionESPModificationModuleRPC.COUNT;
	}

	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
		case ExpansionESPModificationModuleRPC.RequestCode:
			RPC_RequestCode( ctx, sender, target );
			break;
		case ExpansionESPModificationModuleRPC.RemoveCode:
			RPC_RemoveCode( ctx, sender, target );
			break;
		}
	}

	void RequestCode( Object target )
	{
		if ( IsMissionOffline() )
		{
			ItemBase item = ItemBase.Cast( target );
			if ( !item )
				return;

			Exec_RequestCode( item, item.GetCode() );
		} else if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( target, ExpansionESPModificationModuleRPC.RequestCode, false, NULL );
		}
	}

	private void Exec_RequestCode( ItemBase target, string code )
	{
		if ( !target )
			return;

		JMESPMetaBaseBuilding meta = JMESPMetaBaseBuilding.Cast( m_BaseModule.EXP_GetMeta( target ) );
		if ( meta )
		{
			meta.SetCode( code );
		}
	}

	private void RPC_RequestCode( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		ItemBase item = ItemBase.Cast( target );
		if ( !item )
			return;

		if ( GetGame().IsServer() )
		{
			JMPlayerInstance instance;
			if ( !GetPermissionsManager().HasPermission( "ESP.Object.BaseBuilding.Codelock", senderRPC, instance ) )
				return;

			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( item.GetCode() );
			rpc.Send( target, ExpansionESPModificationModuleRPC.RequestCode, false, NULL );
		} else
		{
			string code;
			if ( !ctx.Read( code ) )
				return;

			Exec_RequestCode( item, code );
		}
	}

	void RemoveCode( Object target )
	{
		if ( IsMissionOffline() )
		{
			ItemBase item = ItemBase.Cast( target );
			if ( !item )
				return;

			item.SetCode( "" );
		} else if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( target, ExpansionESPModificationModuleRPC.RemoveCode, false, NULL );
		}
	}

	private void RPC_RemoveCode( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		ItemBase item = ItemBase.Cast( target );
		if ( !item )
			return;

		JMPlayerInstance instance;
		if ( !GetPermissionsManager().HasPermission( "ESP.Object.BaseBuilding.Codelock", senderRPC, instance ) )
			return;

		item.SetCode( "" );

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( "" );
		rpc.Send( target, ExpansionESPModificationModuleRPC.RequestCode, false, NULL );
	}
};