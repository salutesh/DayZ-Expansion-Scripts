modded class PlateCarrierPouches
{
	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (m_Expansion_RestoringContents)
			return true;

		return super.CanReceiveItemIntoCargo(item);
	}
}
