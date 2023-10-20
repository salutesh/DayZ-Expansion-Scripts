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

	override void SplitItemToInventoryLocation( notnull InventoryLocation dst )
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if ( !CanBeSplit() )
			return;

		Magazine new_pile = Magazine.Cast( GameInventory.LocationCreateEntity( dst, GetType(), ECE_IN_INVENTORY, RF_DEFAULT ) );
		ExSplitItem(new_pile);
	}

	override void SplitItem(PlayerBase player)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if ( !CanBeSplit() )
			return;

		Magazine new_pile = Magazine.Cast( player.CreateCopyOfItemInInventoryOrGround( this ) );
		ExSplitItem(new_pile);
	}

	void ExSplitItem(Magazine new_pile)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		new_pile.ServerSetAmmoCount(0);
		int quantity = GetAmmoCount();
		float averageDmg;
		int numberOfTransferedBullets;
		for (int i = 0; i < Math.Floor( quantity * 0.5 ); ++i)
		{
			float damage;
			string cartrige_name;
			ServerAcquireCartridge(damage, cartrige_name);
			if (new_pile.ServerStoreCartridge(damage, cartrige_name))
			{
				numberOfTransferedBullets++;
				averageDmg += damage;
			}
		}

		if (GetGame().IsServer())
		{
			float resultingHealth = 1 - averageDmg / numberOfTransferedBullets;
			new_pile.SetHealth01("", "", resultingHealth);
		}

		new_pile.SetSynchDirty();
		SetSynchDirty();

#ifdef EXPANSIONMODQUESTS
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (player && player.GetIdentity())
			CheckAssignedObjectivesForEntity(ExpansionQuestItemState.STACKSIZE_CHANGED, player);
#endif
	}

	override void CombineItems( ItemBase other_item, bool use_stack_max = false )
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if ( !CanBeCombined(other_item) )
			return;

		if ( other_item.GetType() != GetType() )
			return;

		Magazine other_magazine;
		if ( Class.CastTo(other_magazine, other_item) )
		{
			//int other_item_quantity = other_magazine.GetAmmoCount();
			int this_free_space = GetAmmoMax() - GetAmmoCount();
			int numberOfTransferredBullets = 0;
			int currentAmount = GetAmmoCount();

			for (int i = 0; i < this_free_space && other_magazine.GetAmmoCount() > 0 ; i++)
			{
				float damage;
				string cartrige_name;
				other_magazine.ServerAcquireCartridge(damage, cartrige_name);
				if (ServerStoreCartridge(damage, cartrige_name))
					++numberOfTransferredBullets;
			}

			other_magazine.SetSynchDirty();
			SetSynchDirty();
		}
	}

	void ExDebugPrint(string text)
	{
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	}
};

