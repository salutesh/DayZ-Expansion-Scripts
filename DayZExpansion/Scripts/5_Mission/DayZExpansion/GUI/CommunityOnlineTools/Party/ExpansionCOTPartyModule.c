/**
 * ExpansionCOTPartyModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTPartyModule: ExpansionCOTModuleBase
{
	void ExpansionCOTPartyModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Party.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Party.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Party.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/party/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Party Settings";
	}

	override string GetIconName()
	{
		return "P";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion Party Settings";
	}

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetParty();
	}

	override void OnRegisterSettings()
	{
		//! Party misc
		CreateToggle( "EnableParties", "Enable Parties", "", "" );
		CreateSlider( "MaxMembersInParty", "Max Members In a Party", "", "", 0.0, 60.0, 1.0);
		CreateToggle( "UseWholeMapForInviteList", "Use Whole Map For Invite List", "", "" );
		CreateToggle( "EnablePartyMembersMapMarkers", "Show Party Members On The Map", "", "" );

		//! MemberMarkers
		CreateToggle( "EnablePartyMemberMarker", "Enable Party Members Markers", "", "" );
		CreateToggle( "ShowDistanceUnderPartyMembersMarkers", "Show Name On Party Members Markers", "", "" );
		CreateToggle( "ShowNameOnPartyMembersMarkers", "Show Distance On Party Members Markers", "", "" );

		//! QuickMarkers
		CreateToggle( "EnableQuickMarker", "Enable Quick Marker", "", "" );
		CreateToggle( "ShowDistanceUnderQuickMarkers", "Show Distance Under Quick Markers", "", "" );
		CreateToggle( "ShowNameOnQuickMarkers", "Show Name Under Quick Markers", "", "" );

		//! PartyMarker
		CreateToggle( "CanCreatePartyMarkers", "Can Create Party Markers", "", "" );
	}

	override int GetRPCMin()
	{
		return ExpansionCOTPartyModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTPartyModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTPartyModuleRPC.Update;
	}

	override void OnSend( ref ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTPartyModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionPartySettings setting = new ExpansionPartySettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Party.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};