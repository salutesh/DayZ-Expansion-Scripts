class ExpansionFSMHelper
{
	static void DoorAnimationSource(EntityAI transport, int seatPos, out bool exists, out string animSource)
	{
		CarScript car;
		if (!Class.CastTo(car, transport))
		{
			return;
		}
		
		exists = car.FindAttachmentBySlotName(car.GetDoorInvSlotNameFromSeatPos(seatPos)) != null;
		
		if (exists)
		{
			string selectionName = car.GetDoorSelectionNameFromSeatPos(seatPos);
			animSource = car.GetAnimSourceFromSelection(selectionName);
		}
	}
};
