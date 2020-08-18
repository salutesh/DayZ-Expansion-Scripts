class Expansion_G36_Base : RifleBoltLock_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_G36_Base::SpawnRecoilObject");
		#endif

		return new M4a1Recoil(this);
	}
}