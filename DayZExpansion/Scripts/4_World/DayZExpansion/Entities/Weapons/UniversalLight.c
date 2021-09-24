/**
 * UniversalLight.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class UniversalLight
{
	override bool CanPutAsAttachment( EntityAI parent )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("UniversalLight::CanPutAsAttachment - Start");
		#endif
		
		if ( parent.IsKindOf("Expansion_G36_Base") || parent.IsKindOf("Expansion_M16_Base") || parent.IsKindOf("Expansion_BenelliM4_Base") || parent.IsKindOf("Expansion_MP7_Base") ) 
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("UniversalLight::CanPutAsAttachment - Return true");
			#endif
			return true;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("UniversalLight::CanPutAsAttachment - Return super");
		#endif
		return super.CanPutAsAttachment(parent);
	}
};