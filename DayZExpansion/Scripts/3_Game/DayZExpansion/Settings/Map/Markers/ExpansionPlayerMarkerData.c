class ExpansionPlayerMarkerData : ExpansionMarkerData
{
	override ExpansionMapMarkerType GetType()
	{
		return ExpansionMapMarkerType.PLAYER;
	}

	override vector GetAdjustedPosition()
	{
		vector position = GetPosition();
		Object object = GetObject();

		Human human;
		if ( Class.CastTo( human, object ) )
		{
			UpdateObject( human );

			if ( human.IsAlive() )
			{
				position = human.GetBonePositionWS( human.GetBoneIndexByName( "Head" ) ) + "0.0 0.25 0.0";
			} else
			{
				position = human.GetPosition();
			}
		}

		return position;
	}
};