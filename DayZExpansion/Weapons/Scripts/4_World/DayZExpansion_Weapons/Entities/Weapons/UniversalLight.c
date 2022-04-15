/**
 * UniversalLight.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class UniversalLight
{
	override bool CanPutAsAttachment( EntityAI parent )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "CanPutAsAttachment");
#endif
		
		if ( parent.IsKindOf("Expansion_G36_Base") || parent.IsKindOf("Expansion_M16_Base") || parent.IsKindOf("Expansion_BenelliM4_Base") || parent.IsKindOf("Expansion_MP7_Base") ) 
		{
			return true;
		}
		
		return super.CanPutAsAttachment(parent);
	}
};
