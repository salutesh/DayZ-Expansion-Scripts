/**
 * BuildingBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class BuildingBase
{
	ref ExpansionNameOverride m_Expansion_NameOverride;

	override bool NameOverride(out string output)
	{
		if (m_Expansion_NameOverride && m_Expansion_NameOverride.Get(output))
			return true;
		else
			return super.NameOverride(output);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (m_Expansion_NameOverride)
			m_Expansion_NameOverride.OnRPC(sender, rpc_type, ctx);
	}
}
