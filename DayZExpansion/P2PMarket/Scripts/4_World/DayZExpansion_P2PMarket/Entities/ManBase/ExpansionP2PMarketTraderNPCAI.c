/**
 * ExpansionP2PMarketTraderNPCAI.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef ENFUSION_AI_PROJECT
/**@class		ExpansionP2PMarketTraderNPCAI
 * @brief
 **/
#ifdef EXPANSIONMODAI
class ExpansionP2PMarketTraderNPCAI: eAINPCBase
#else
class ExpansionP2PMarketTraderNPCAI: eAIBase
#endif
{
	protected int m_P2PTraderID;	 //! Unique black marker trader id. Used to get and identify the trader in the black marker module.
	void ExpansionP2PMarketTraderNPCAI()
	{
	#ifdef EXPANSIONMODAI
		SetMovementSpeedLimits(1.0);  //! Always walk
	#endif

		m_P2PTraderID = -1;
		RegisterNetSyncVariableInt("m_P2PTraderID");
	}

	override void Expansion_Init()
	{
	#ifdef EXPANSIONMODAI
		if (GetGame().IsServer())
			m_eAI_FactionType = eAIFactionInvincibleObservers;
	#else
		Expansion_SetAllowDamage(false);
	#endif

		super.Expansion_Init();

	#ifndef EXPANSIONMODAI
		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
	#endif
	}

#ifndef EXPANSIONMODAI
	//! Vanilla, prevent being targeted by Zs/animals
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		return false;
	}

	override bool PlayerIsEnemy(PlayerBase other)
	{
		return false;
	}
#endif

	void SetP2PTraderID(int id)
	{
		m_P2PTraderID = id;
		EXPrint(ToString() + "::SetP2PTraderID - ID: " + m_P2PTraderID);
	}

	int GetP2PTraderID()
	{
		return m_P2PTraderID;
	}
};

class ExpansionP2PTraderAIMirek: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIDenis: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIBoris: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAICyril: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIElias: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIFrancis: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIGuo: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIHassan: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIIndar: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIJose: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIKaito: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAILewis: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIManua: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAINiki: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIOliver: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIPeter: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIQuinn: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIRolf: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAISeth: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAITaiki: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAILinda: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIMaria: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIFrida: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIGabi: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIHelga: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIIrena: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIJudy: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIKeiko: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIEva: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAINaomi: ExpansionP2PMarketTraderNPCAI {};
class ExpansionP2PTraderAIBaty: ExpansionP2PMarketTraderNPCAI {};
#endif