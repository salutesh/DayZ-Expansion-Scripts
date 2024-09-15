modded class ExpansionVehicle
{
	override void DeferredInit()
	{
		super.DeferredInit();

		EntityAI entity = GetEntity();
		if (IsStoreLoaded() && !entity.IsSetForDeletion() && ExpansionGarageModule.s_Instance.IsInGarage(this))
		{
			EXPrint("Deleting " + entity + " " + entity.GetPosition() + " global ID " + GetGlobalID().IDToString());
			entity.Delete();
		}
	}
}
