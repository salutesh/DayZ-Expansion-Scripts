/*
 * CarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**
 * @class CarScript
 * @brief		
 */
modded class CarScript
{
	override void OnStoreSave(ParamsWriteContext ctx)
	{
#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("CarScript::OnStoreSave " + this + " " + GetGame().SaveVersion());
#endif

#ifdef EXPANSION_MODSTORAGE
		if (GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET)
		{
			super.OnStoreSave(ctx);
			return;
		}
#endif

		super.OnStoreSave(ctx);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("CarScript::OnStoreLoad - Start");
#endif

#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("CarScript::OnStoreLoad " + this + " " + version);
#endif

		if (Expansion_Assert_False(super.OnStoreLoad(ctx, version), "[" + this + "] Failed reading OnStoreLoad super"))
			return false;

#ifdef EXPANSION_MODSTORAGE
		if (version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET)
			return true;
#endif

		if (GetExpansionSaveVersion() < 16)
			return true;

		if (GetExpansionSaveVersion() >= 21)
			return true;

		string currentSkinName = m_CurrentSkinName;

		if (Expansion_Assert_False(ctx.Read(m_CurrentSkinName), "[" + this + "] Failed reading m_CurrentSkinName"))
			return false;

		if (m_CurrentSkinName == "")
			m_CurrentSkinName = currentSkinName;

		return true;
	}
};
