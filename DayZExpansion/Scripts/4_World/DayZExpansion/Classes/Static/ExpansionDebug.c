static string Expansion_Debug_Player( Man man )
{
	PlayerBase player = PlayerBase.Cast( man );
	if ( player )
	{
		return "[" + player.GetIdentityName() + ":" + player.GetIdentityUID() + "] ";
	}

	if ( man )
	{
		return "[Unknown Player] ";
	}

	return "[NULL Player] ";
}