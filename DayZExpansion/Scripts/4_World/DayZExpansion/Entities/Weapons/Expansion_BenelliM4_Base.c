class Expansion_BenelliM4_Base: ExpansionSemiShotGun
{
	override RecoilBase SpawnRecoilObject()
	{
		return new Mp133Recoil(this);
	}
};