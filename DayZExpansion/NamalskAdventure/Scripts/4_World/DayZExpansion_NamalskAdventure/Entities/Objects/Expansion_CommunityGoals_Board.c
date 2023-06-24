/**
 * Expansion_CommunityGoals_Board.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_CommunityGoals_Board: House
{
	#ifdef EXPANSIONMODAI
	protected int m_FactionID;
	#endif

	void Expansion_CommunityGoals_Board()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		#ifdef EXPANSIONMODAI
		RegisterNetSyncVariableInt("m_FactionID");
		#endif
	}

	void ~Expansion_CommunityGoals_Board()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}

	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEInit();
	}
	
	#ifdef EXPANSIONMODAI
	void SetFactionID(int factionID)
	{
		m_FactionID = factionID;
	}
	
	int GetFactionID()
	{
		return m_FactionID;
	}
	#endif
	
	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionOpenCommunityGoalsMenu);
	}

	override bool IsInventoryVisible()
	{
		return true;
	}

	override bool CanDisplayCargo()
	{
		return true;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI player)
	{
		return false;
	}

	override bool CanRemoveFromCargo(EntityAI parent)
	{
		return true;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return true;
	}

	override bool CanLoadItemIntoCargo(EntityAI item)
	{
		return true;
	}

	override bool DisableVicinityIcon()
    {
        return true;
    }

	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.AfterStoreLoad();

		GetGame().ObjectDelete(this);
	}

	protected void ExDebugPrint(string text)
	{
		#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
		#endif
	}
};