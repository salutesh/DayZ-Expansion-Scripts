/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Ammunition_Base
{
	void Ammunition_Base()
	{
		SetEventMask(EntityEvent.CONTACT);
		SetFlags(EntityFlags.TOUCHTRIGGERS, false);
	}

	override void EOnContact(IEntity other, Contact extra)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		if (GetGame().IsServer())
		{
			Expansion_Anomaly_Base anomaly = Expansion_Anomaly_Base.Cast(other);
			if (!anomaly)
				return;

			ExpansionAnomalyTriggerBase anomalyTrigger = anomaly.GetAnomalyTrigger();
			if (anomalyTrigger)
			{
				anomalyTrigger.EOnEnter(other, 0);
			}
		}
	}

	override int Expansion_TransferCartridges(Magazine dst, int amount, bool resetDstAmmoCount = true, out float totalDamage = 0.0)
	{
		int numberOfTransferredBullets = super.Expansion_TransferCartridges(dst, amount, resetDstAmmoCount, totalDamage);

		if (GetGame().IsServer())
		{
			float resultingHealth = 1 - totalDamage / numberOfTransferredBullets;
			dst.SetHealth01("", "", resultingHealth);
		}

		return numberOfTransferredBullets;
	}

	//! @note Can't call super here, we don't want health of this ammo pile to change after combining
	override void CombineItems( ItemBase other_item, bool use_stack_max = false )
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if ( !CanBeCombined(other_item) )
			return;

		if ( other_item.GetType() != GetType() )
			return;

		Ammunition_Base otherPile;
		if ( Class.CastTo(otherPile, other_item) )
		{
			int numberOfTransferredBullets = otherPile.Expansion_TransferCartridges(this, otherPile.GetAmmoCount(), false);

			OnCombine(other_item);

		#ifdef EXPANSIONMODQUESTS
			if (numberOfTransferredBullets)
			{
				Expansion_OnStackSizeChanged(numberOfTransferredBullets);
				otherPile.Expansion_OnStackSizeChanged(-numberOfTransferredBullets);
			}
		#endif
		}
	}

	void ExDebugPrint(string text)
	{
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	}
};

