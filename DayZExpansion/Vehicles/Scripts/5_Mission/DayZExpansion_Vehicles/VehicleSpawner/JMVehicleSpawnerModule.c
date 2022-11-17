#ifdef JM_COT
modded class JMVehicleSpawnerModule
{
	#ifdef JM_COT_VEHICLE_ONSPAWNVEHICLE
	override void OnSpawnVehicle(EntityAI entity)
	{
		super.OnSpawnVehicle(entity);

		ExpansionVehicleBase car;
		if (Class.CastTo(car, entity))
		{			
			FillEV(car, CarFluid.FUEL);
			FillEV(car, CarFluid.OIL);
			FillEV(car, CarFluid.BRAKE);
			FillEV(car, CarFluid.COOLANT);
		}
	}
	#endif

	private void FillEV( ExpansionVehicleBase car, CarFluid fluid )
	{
		float cap = car.GetFluidCapacity( fluid );
		car.Fill( fluid, cap );
	}
};
#endif