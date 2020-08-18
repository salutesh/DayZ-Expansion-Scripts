class Expansion_Kar98_Base extends BoltActionRifle_InnerMagazine_Base
{
	void Expansion_Kar98_Base()
	{
	}
	
	override bool CanEnterIronsights()
	{
		ItemOptics optic = GetAttachedOptics();
		if (optic && PUScopeOptic.Cast(optic))
			return true;
		return super.CanEnterIronsights();
	}
	override RecoilBase SpawnRecoilObject()
	{
		return new MosinRecoil(this);
	}	
};