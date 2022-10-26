/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef WRDG_DOGTAGS
modded class Dogtag_Base
{
	protected int m_HardlineHumanity;
	
	void Dogtag_Base()
	{
		RegisterNetSyncVariableInt("m_HardlineHumanity");
	}
	
	override bool DescriptionOverride(out string output)
	{
		bool modified = super.DescriptionOverride(output);
		
		if (GetExpansionSettings().GetHardline().UseHumanity)
		{
			PlayerBase player;
			if (Class.CastTo(player, GetHierarchyRootPlayer()) && player.GetDogtag() == this)
				m_HardlineHumanity = player.GetHumanity();
			output += string.Format("#STR_EXPANSION_HARDLINE_HUMANITY_LABEL %1\n", m_HardlineHumanity);
			modified = true;
		}
		
		return modified;
	}
	
	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Hardline];
		if (!ctx) return;
		
		ctx.Write(m_HardlineHumanity);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Hardline];
		if (!ctx) return true;

		if (!ctx.Read(m_HardlineHumanity))
			return false;

		return true;
	}
	#endif
	
	//! Bad ref here!! Need to be fixed in the DogTags mod!
	override void OnRPCRead(PlayerIdentity sender, ref ParamsReadContext ctx)
	{
		super.OnRPCRead(sender, ctx);
		if (GetExpansionSettings().GetHardline().UseHumanity)
		{
			ctx.Read(m_HardlineHumanity);
		}
	}

	override void OnRPCWrite(ScriptRPC rpc)
	{
		super.OnRPCWrite(rpc);
		
		if (GetExpansionSettings().GetHardline().UseHumanity)
		{
			rpc.Write(m_HardlineHumanity);
		}
	}
	
	void SetHardlineHumanity(float humanity)
	{
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			m_HardlineHumanity = humanity;
			SetSynchDirty();
		}
	}
	
	int GetHardlineHumanity()
	{
		return m_HardlineHumanity;
	}
};

class Dogtag_Scout : Dogtag_Base {};
class Dogtag_Pathfinder : Dogtag_Base {};
class Dogtag_Superhero : Dogtag_Base {};
class Dogtag_Legend : Dogtag_Base {};
class Dogtag_Kleptomaniac : Dogtag_Base {};
class Dogtag_Bully : Dogtag_Base {};
class Dogtag_Killer : Dogtag_Base {};
class Dogtag_Madman : Dogtag_Base {};
#endif