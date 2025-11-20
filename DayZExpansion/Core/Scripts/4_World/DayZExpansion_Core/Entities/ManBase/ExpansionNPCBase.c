/**
 * ExpansionNPCBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionNPCBase: ExpansionNotPlayerBase
{
	override void Expansion_Init()
	{
		Expansion_SetAllowDamage(false);

		#ifdef EXPANSIONMODAI
		if (g_Game.IsServer())
		{
			m_eAI_FactionType = eAIFactionPassive;
			eAI_SetPassive();  //! In case this NPC joins another group that's part of a non-passive faction, we still want the NPC to be passive
		}
		#endif

		super.Expansion_Init();

		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
	}

	override bool IsInventoryVisible()
	{
		return false;
	}

	override bool CanDisplayCargo()
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished) 
	{
		dBodyActive(this, ActiveState.INACTIVE);
	}

	//! Vanilla, prevent being targeted by Zs/animals
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		return false;
	}
}

class ExpansionNPCMirek: ExpansionNPCBase {};
class ExpansionNPCDenis: ExpansionNPCBase {};
class ExpansionNPCBoris: ExpansionNPCBase {};
class ExpansionNPCCyril: ExpansionNPCBase {};
class ExpansionNPCElias: ExpansionNPCBase {};
class ExpansionNPCFrancis: ExpansionNPCBase {};
class ExpansionNPCGuo: ExpansionNPCBase {};
class ExpansionNPCHassan: ExpansionNPCBase {};
class ExpansionNPCIndar: ExpansionNPCBase {};
class ExpansionNPCJose: ExpansionNPCBase {};
class ExpansionNPCKaito: ExpansionNPCBase {};
class ExpansionNPCLewis: ExpansionNPCBase {};
class ExpansionNPCManua: ExpansionNPCBase {};
class ExpansionNPCNiki: ExpansionNPCBase {};
class ExpansionNPCOliver: ExpansionNPCBase {};
class ExpansionNPCPeter: ExpansionNPCBase {};
class ExpansionNPCQuinn: ExpansionNPCBase {};
class ExpansionNPCRolf: ExpansionNPCBase {};
class ExpansionNPCSeth: ExpansionNPCBase {};
class ExpansionNPCTaiki: ExpansionNPCBase {};
class ExpansionNPCLinda: ExpansionNPCBase {};
class ExpansionNPCMaria: ExpansionNPCBase {};
class ExpansionNPCFrida: ExpansionNPCBase {};
class ExpansionNPCGabi: ExpansionNPCBase {};
class ExpansionNPCHelga: ExpansionNPCBase {};
class ExpansionNPCIrena: ExpansionNPCBase {};
class ExpansionNPCJudy: ExpansionNPCBase {};
class ExpansionNPCKeiko: ExpansionNPCBase {};
class ExpansionNPCEva: ExpansionNPCBase {};
class ExpansionNPCNaomi: ExpansionNPCBase {};
class ExpansionNPCBaty: ExpansionNPCBase {};
