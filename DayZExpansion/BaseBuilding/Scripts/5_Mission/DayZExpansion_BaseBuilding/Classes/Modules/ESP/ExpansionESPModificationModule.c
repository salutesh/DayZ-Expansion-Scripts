/**
 * ExpansionESPModificationModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
#ifdef EXPANSIONCOTESPMODULE
modded class ExpansionESPModificationModule
{
	void ExpansionESPModificationModule()
	{
		GetPermissionsManager().RegisterPermission( "ESP.Object.BaseBuilding.Codelock" );
	}

	#ifdef CF_BUGFIX_REF
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	#else
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	#endif
	{
		switch ( rpc_type )
		{
		case ExpansionESPModificationModuleRPC.RequestCode:
			RPC_RequestCode( ctx, sender, target );
			return;
		case ExpansionESPModificationModuleRPC.RemoveCode:
			RPC_RemoveCode( ctx, sender, target );
			return;
		}

		super.OnRPC(sender, target, rpc_type, ctx);
	}

	//! ===============================================
	//! CODELOCK ESP FUNCTIONS
	//! ===============================================

	void RequestCode( Object target )
	{
		if ( IsMissionOffline() )
		{
			ItemBase item = ItemBase.Cast( target );
			Container_Base container = Container_Base.Cast( target );
			if ( !item && !container )
				return;
			if( !item )
				Exec_RequestCode( container, container.GetCode() );
			else
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
		} else {
			JMESPMetaTents metat = JMESPMetaTents.Cast( m_BaseModule.EXP_GetMeta( target ) );
			if ( metat )
			{
				metat.SetCode( code );
			} else {
				JMESPMetaContainer metac = JMESPMetaContainer.Cast( m_BaseModule.EXP_GetMeta( target ) );
				if ( metac )
				{
					metac.SetCode( code );
				}
			}
		}
	}

	private void RPC_RequestCode( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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

	private void RPC_RemoveCode( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
#endif
#endif
