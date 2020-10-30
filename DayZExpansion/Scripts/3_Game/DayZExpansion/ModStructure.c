#ifdef CF_MOD_STORAGE
modded class ModStructure
{
	override bool OnLoad( string modName )
	{
		if ( modName != "DZ_Expansion" )
			return super.OnLoad( modName );

		//! Set the storage version for this mod
		SetStorageVersion( EXPANSION_VERSION_CURRENT_SAVE );

		return true;
	}
};
#endif