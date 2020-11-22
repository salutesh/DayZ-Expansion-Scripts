class ExpansionNetSyncDebugObject extends ItemBase
{
	int m_NetIDLow;
	int m_NetIDHigh;

	void ExpansionNetSyncDebugObject()
	{
		SetEventMask( EntityEvent.SIMULATE | EntityEvent.POSTSIMULATE | EntityEvent.INIT | EntityEvent.CONTACT | EntityEvent.FRAME | EntityEvent.PHYSICSMOVE );

		m_NetIDLow = 0;
		m_NetIDHigh = 0;

		RegisterNetSyncVariableInt( "m_NetIDLow" );
		RegisterNetSyncVariableInt( "m_NetIDHigh" );
	}

	void SetSyncObject( EntityAI object )
	{
		object.GetNetworkID( m_NetIDLow, m_NetIDHigh );
	}
	
	override void EOnSimulate( IEntity owner, float dt ) 
	{
		Object obj = GetGame().GetObjectByNetworkId( m_NetIDLow, m_NetIDHigh );

		if ( obj )
		{
			vector info[2];
			obj.ClippingInfo( info );

			vector trans[4];
			GetTransform(trans);

			trans[0][0] = (Math.AbsFloat(info[0][0]) + Math.AbsFloat(info[1][0])) / 2;
			trans[1][1] = (Math.AbsFloat(info[0][1]) + Math.AbsFloat(info[1][1])) / 2;
			trans[2][2] = (Math.AbsFloat(info[0][2]) + Math.AbsFloat(info[1][2])) / 2;
			
			SetTransform(trans);
		}
	}
};