/**
 * ExpansionGetPlayerObjectByIdentity.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static PlayerBase ExpansionGetPlayerObjectByIdentity( PlayerIdentity identityGetPlayerObjectByIdentity )
{
	if ( !GetGame().IsMultiplayer() )
		return PlayerBase.Cast( GetGame().GetPlayer() );
	
	if ( identityGetPlayerObjectByIdentity == NULL )
		return NULL;

	int networkIdLowBits;
	int networkIdHighBits;
	GetGame().GetPlayerNetworkIDByIdentityID( identityGetPlayerObjectByIdentity.GetPlayerId(), networkIdLowBits, networkIdHighBits );

	return PlayerBase.Cast( GetGame().GetObjectByNetworkId( networkIdLowBits, networkIdHighBits ) );
}
