class ExpansionPersonalMarkerData : ExpansionMarkerData
{
	override ExpansionMapMarkerType GetType()
	{
		return ExpansionMapMarkerType.PERSONAL;
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_Visibility ), "[" + this + "] Failed reading m_Visibility" ) )
			return false;

		return true;
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );

		ctx.Write( m_Visibility );
	}
};