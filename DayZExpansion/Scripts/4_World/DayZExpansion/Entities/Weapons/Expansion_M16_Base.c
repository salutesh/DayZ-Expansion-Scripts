class Expansion_M16_Base : RifleBoltLock_Base
{
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::EEItemAttached - Start");
		#endif		

		super.EEItemAttached(item,slot_name);

		if (ItemOptics.Cast(item))
		{ 
			EntityAI ParentItem = this;
			FoldOpticsDown(ParentItem);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_16_Base::EEItemAttached - End");
		#endif		
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::EEItemDettached - Start");
		#endif

		super.EEItemDetached(item,slot_name);
		if (ItemOptics.Cast(item))
		{ 
			EntityAI ParentItem = this;
			FoldOpticsUp(ParentItem);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::EEItemDettached - End");
		#endif
	}	

	override RecoilBase SpawnRecoilObject()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M16_Base::SpawnRecoilObject");
		#endif
		return new M4a1Recoil(this);
	}
}