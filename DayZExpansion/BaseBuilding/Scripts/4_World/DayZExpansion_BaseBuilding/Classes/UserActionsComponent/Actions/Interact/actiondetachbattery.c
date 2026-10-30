modded class ActionDetachBattery
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (super.ActionCondition(player, target, item))
			return true;

		DigitalCodeLock codeLock = DigitalCodeLock.Cast(target.GetObject());
		if (codeLock)
		{
			EntityAI attachment = codeLock.GetInventory().FindAttachment(Battery9V.SLOT_ID);
			if (!attachment)
				return false;

			if (codeLock.IsLocked() || codeLock.IsLockedInSlot())
				return false;

			//! Enable detach battery from DigitalCodeLock attached to ExpansionBaseBuilding
			if (!codeLock.IsPlayerOnBackside(player))
				return false;

			if (player.GetInventory().CanAddEntityIntoHands(attachment))
			{
				if (attachment.CanDetachAttachment(codeLock) && codeLock.CanReleaseAttachment(attachment))
				{
					m_Text = "#detach_battery " + attachment.GetDisplayName();
					return true;
				}
			}
		}

		return false;
	}
}
