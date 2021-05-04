#ifdef CF_MODULE_MODSTORAGE
modded class ModStructure
{
	override bool OnLoad( string modName )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("ModStructure::OnLoad " + m_Name + " (" + modName + ")");
		#endif

		if ( m_Name != "DZ_Expansion" )
			return super.OnLoad( modName );

		//! Set the storage version for this mod
		SetStorageVersion( EXPANSION_VERSION_CURRENT_SAVE );

		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("ModStructure::OnLoad storage version = " + m_StorageVersion);
		#endif

		return true;
	}
};
#endif