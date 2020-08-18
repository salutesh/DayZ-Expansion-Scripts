class Expansion_M9_Base : Pistol_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M9_Base::SpawnRecoilObject");
		#endif

		return new Cz75Recoil(this);
	}
};