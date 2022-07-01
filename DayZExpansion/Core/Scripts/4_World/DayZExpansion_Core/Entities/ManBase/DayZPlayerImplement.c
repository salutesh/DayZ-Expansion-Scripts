/**
 * DayZPlayerImplement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerImplement
{
	ref ExpansionHumanST m_ExpansionST;

	protected bool m_Expansion_CanBeLooted = true;

	ref ExpansionNameOverride m_Expansion_NameOverride;

	void Expansion_SetCanBeLooted(bool canBeLooted)
	{
		m_Expansion_CanBeLooted = canBeLooted;
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		if (!m_Expansion_CanBeLooted)
			Expansion_LockInventory();
	}

	void Expansion_LockInventory()
	{
		int attcount = GetInventory().AttachmentCount();
		for (int att = 0; att < attcount; att++)
		{ 
			EntityAI attachmentEntity = GetInventory().GetAttachmentFromIndex(att);
			if (attachmentEntity)
				attachmentEntity.GetInventory().LockInventory(10134);
		}
		GetInventory().LockInventory(10134);
	}

	void Expansion_UnlockInventory()
	{
		int attcount = GetInventory().AttachmentCount();
		for (int att = 0; att < attcount; att++)
		{ 
			EntityAI attachmentEntity = GetInventory().GetAttachmentFromIndex(att);
			if (attachmentEntity)
				attachmentEntity.GetInventory().UnlockInventory(10134);
		}
		GetInventory().UnlockInventory(10134);
	}

	override bool CanBeSkinned()
	{
		if (!m_Expansion_CanBeLooted)
			return false;

		return super.CanBeSkinned();
	}

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
};
