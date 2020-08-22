modded class ActionFillCoolant
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if( !target )
		{
			Print("Radiator debug target null")
			return false;
		}
		if( item.GetQuantity() <= 0 )
		{
			Print("Radiator debug item.GetQuantity() <= 0")
			return false;
		}
		if( item.GetLiquidType() != LIQUID_WATER && item.GetLiquidType() != LIQUID_RIVERWATER )
		{
			Print("Radiator debug item.GetLiquidType()")
			return false;
		}
		Car car = Car.Cast( target.GetParent() );
		if( !car )
		{
			Print("Radiator debug car null")
			return false;
		}
		if( car.GetFluidFraction( CarFluid.COOLANT ) >= 0.95 )
		{
			Print("Radiator debug car full")
			return false;
		}
		array<string> selections = new array<string>;
		target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);

		CarScript carS = CarScript.Cast(car);
		
		if( carS )
		{
			for (int s = 0; s < selections.Count(); s++)
			{
				if ( selections[s] == carS.GetActionCompNameCoolant() )
				{
					float dist = vector.Distance( carS.GetCoolantPtcPosWS(), player.GetPosition() );

					if ( dist < carS.GetActionDistanceCoolant() )
						return true;
				}
			}
		}
		Print("Radiator debug particle point or selection not found")
		return false;
	}
};