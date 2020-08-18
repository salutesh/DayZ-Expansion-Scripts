/*
class ExpansionCarDoorFix: CarDoor
{
	private bool m_EEOnAfterLoadCalled = false;

	void ExpansionCarDoorFix()
	{
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.PerformSkinFixOldVersion, 1000, false );
	}

	void ~ExpansionCarDoorFix()
	{
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( this.PerformSkinFixOldVersion );
	}

	void PerformSkinFixOldVersion()
	{
		if ( !m_EEOnAfterLoadCalled )
		{
			if ( m_CanBeSkinned && m_SkinModule )
			{
				m_SkinModule.PerformSkinFixOldVersion( this, m_CurrentSkinName );
			}
		}
	}
	
	override void EEOnAfterLoad()
	{
		PerformSkinFixOldVersion();

		m_EEOnAfterLoadCalled = true;
	}
}
*/