/**
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

modded class CarScript
{
	override void DeferredInit()
	{
		super.DeferredInit();

		if (m_Expansion_IsStoreLoaded && !IsSetForDeletion() && ExpansionGarageModule.s_Instance.IsInGarage(this))
		{
			EXPrint("Deleting " + this + " " + GetPosition() + " global ID " + m_Expansion_GlobalID.IDToString());
			Delete();
		}
	}
}
