//! Workaround for untakeable hand item if dying proned
//! https://feedback.bistudio.com/T176038
modded class DayZPlayerImplement
{
	override void EEKilled(Object killer)
	{
		if (GetGame().IsServer() && IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEIDX_RAISEDPRONE))
		{
			EntityAI itemInHands = GetHumanInventory().GetEntityInHands();
			if (itemInHands && GetExpansionSettings().GetDebug().EnableProneDeathHandItemDropFix)
			{
				EXPrint("Player is prone w/ item in hand, applying workaround for T176038 by dropping hand item to ground prematurely " + itemInHands);
				
				int boneIndex = GetBoneIndexByName("RightHand_Dummy");
			
				vector m4[4];
				GetBoneTransformWS(boneIndex, m4);
			
				GetInventory().DropEntityWithTransform(InventoryMode.SERVER, this, itemInHands, m4);

				if (!Expansion_CanBeLooted())
				{
					ExpansionItemBaseModule.SetLootable(itemInHands, false);
					itemInHands.SetLifetimeMax(120);  //! Make sure it despawns quickly when left alone
				}
			}
		}

		super.EEKilled(killer);
	}
};
