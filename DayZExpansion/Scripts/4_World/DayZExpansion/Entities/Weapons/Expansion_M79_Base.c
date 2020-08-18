class Expansion_M79_Base extends RifleSingleShot_Base
{
	void Expansion_M79_Base ()
	{
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		return new Izh18Recoil(this);
	}
};