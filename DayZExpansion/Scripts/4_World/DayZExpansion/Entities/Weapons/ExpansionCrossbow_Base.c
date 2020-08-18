class ExpansionCrossbow_Base extends RifleSingleShot_Base
{
	void ExpansionCrossbow_Base ()
	{
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		return new Izh18Recoil(this);
	}
}