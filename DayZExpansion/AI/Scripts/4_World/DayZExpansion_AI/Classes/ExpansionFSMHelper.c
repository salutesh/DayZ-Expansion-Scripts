class ExpansionFSMHelper
{
	static void DoorAnimationSource(EntityAI transport, int seatPos, out bool exists, out string selectionName)
	{
		CarScript car;
		if (!Class.CastTo(car, transport))
		{
			return;
		}
		
		exists = car.FindAttachmentBySlotName(car.GetDoorInvSlotNameFromSeatPos(seatPos)) != null;
		
		if (exists)
		{
			selectionName = car.GetDoorSelectionNameFromSeatPos(seatPos);
		}
	}
};
