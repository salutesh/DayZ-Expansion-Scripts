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

	override void OnInit()
	{
		super.OnInit();

		Expansion_RegisterServerRPC("RPC_RemoveCode");
		Expansion_RegisterServerRPC("RPC_RequestCode");
		Expansion_RegisterClientRPC("RPC_RequestCode");
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
			auto rpc = Expansion_CreateRPC("RPC_RequestCode");
			rpc.Expansion_Send(target, true, NULL);
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

	private void RPC_RequestCode(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
		ItemBase item = ItemBase.Cast( target );
		if ( !item )
			return;

		if ( GetGame().IsServer() )
		{
			JMPlayerInstance instance;
			if ( !GetPermissionsManager().HasPermission( "ESP.Object.BaseBuilding.Codelock", senderRPC, instance ) )
				return;

			auto rpc = Expansion_CreateRPC("RPC_RequestCode");
			rpc.Write( item.GetCode() );
			rpc.Expansion_Send(target, true, senderRPC );
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
			auto rpc = Expansion_CreateRPC("RPC_RemoveCode");
			rpc.Expansion_Send(target, true, NULL);
		}
	}

	private void RPC_RemoveCode(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
		ItemBase item = ItemBase.Cast( target );
		if ( !item )
			return;

		JMPlayerInstance instance;
		if ( !GetPermissionsManager().HasPermission( "ESP.Object.BaseBuilding.Codelock", senderRPC, instance ) )
			return;

		item.SetCode( "" );

		auto rpc = Expansion_CreateRPC("RPC_RequestCode");
		rpc.Write( "" );
		rpc.Expansion_Send(target, true, senderRPC);
	}
};
#endif
#endif
