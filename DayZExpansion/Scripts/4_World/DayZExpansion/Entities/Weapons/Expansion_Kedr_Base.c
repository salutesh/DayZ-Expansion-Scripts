class Expansion_Kedr_Base : RifleBoltLock_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_Kedr_Base::SpawnRecoilObject");
		#endif

		return new Cz61Recoil(this);
	}
}