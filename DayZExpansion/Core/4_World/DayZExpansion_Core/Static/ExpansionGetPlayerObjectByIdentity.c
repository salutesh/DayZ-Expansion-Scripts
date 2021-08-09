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
