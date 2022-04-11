class ExpansionFSMHelper
{
	static void DoorAnimationSource(Transport transport, int seatPos, out bool exists, out string selectionName)
	{
		auto car = CarScript.Cast(transport);
		exists = car.FindAttachmentBySlotName(car.GetDoorInvSlotNameFromSeatPos(seatPos)) != null;
		
		if (exists)
		{
			selectionName = car.GetDoorSelectionNameFromSeatPos(seatPos);
		}
	}
};
