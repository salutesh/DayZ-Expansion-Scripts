modded class RecoilBase
{
	override void PostInit( Weapon_Base weapon )
	{
		if ( m_Player )
			super.PostInit( weapon );
	}
}