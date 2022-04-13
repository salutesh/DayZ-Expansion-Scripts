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

	override void OnRPC(Class sender, CF_EventArgs args)
	{
		auto rpc = CF_EventRPCArgs.Cast(args);

		switch ( rpc.ID )
		{
		case ExpansionESPModificationModuleRPC.RequestCode:
			RPC_RequestCode( rpc.Context, rpc.Sender, rpc.Target );
			return;
		case ExpansionESPModificationModuleRPC.RemoveCode:
			RPC_RemoveCode( rpc.Context, rpc.Sender, rpc.Target );
			return;
		}

		super.OnRPC(sender, args);
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
