modded class JMVehicleSpawnerModule
{
	override void OnSpawnVehicle(EntityAI entity)
	{
		super.OnSpawnVehicle(entity);

		ExpansionVehicleBase car;
		if (Class.CastTo(car, entity))
		{
			car.SetPosition( car.GetPosition() + "0 5 0" );
			
			FillEV(car, CarFluid.FUEL);
			FillEV(car, CarFluid.OIL);
			FillEV(car, CarFluid.BRAKE);
			FillEV(car, CarFluid.COOLANT);
		}
	}

	private void FillEV( ExpansionVehicleBase car, CarFluid fluid )
	{
		float cap = car.GetFluidCapacity( fluid );
		car.Fill( fluid, cap );
	}
};