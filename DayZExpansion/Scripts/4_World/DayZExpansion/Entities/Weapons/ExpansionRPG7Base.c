class ExpansionRPG7Base extends RifleSingleShot_Base
{
	void ExpansionRPG7Base()
	{
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		return new Izh18Recoil(this);
	}

	override void EEFired(int muzzleType, int mode, string ammoType)
  	{
		if ( IsMissionClient() )
		{
			Particle.PlayOnObject( ParticleList.EXPANSION_ROCKET_DUST, this, GetMemoryPointPos("usti hlavne") );
		}

		super.EEFired(muzzleType, mode, ammoType);
	}
}