class Expansion_DT11_Base : DoubleBarrel_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		return new B95Recoil(this);
	}
};