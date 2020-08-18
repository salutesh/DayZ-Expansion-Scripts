class Expansion_MP5_Base : RifleBoltLock_Base
{
	void Expansion_MP5_Base ()
	{
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		return new Mp5kRecoil(this);
	}	
};