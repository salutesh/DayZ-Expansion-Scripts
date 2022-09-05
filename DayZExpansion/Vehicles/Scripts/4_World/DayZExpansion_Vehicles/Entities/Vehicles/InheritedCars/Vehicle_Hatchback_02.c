class Vehicle_Hatchback_02 extends ExpansionVehicleCarBase
{
	void Expansion_Hatchback_02()
	{
		m_dmgContactCoef = 0.070;
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return VehicleAnimInstances.GOLF;
	}

	override int GetSeatAnimationType(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatAnimationType").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		case 1:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		case 2:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 3:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		}

		return 0;
	}

	override CarRearLightBase CreateRearLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateRearLight");
#endif
		return CarRearLightBase.Cast(ScriptedLightBase.CreateLight(Hatchback_02RearLight));
	}

	override CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		return CarLightBase.Cast(ScriptedLightBase.CreateLight(Hatchback_02FrontLight));
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CanReleaseAttachment").Add(attachment);
#endif

		if (!super.CanReleaseAttachment(attachment))
			return false;

		string attType = attachment.GetType();

		if (EngineIsOn() || GetCarDoorsState("Hatchback_02_Hood") == CarDoorState.DOORS_CLOSED)
		{
			if (attType == "CarRadiator" || attType == "CarBattery" || attType == "SparkPlug")
				return false;
		}

		return true;
	}

	override bool CanDisplayAttachmentCategory(string category_name)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CanDisplayAttachmentCategory").Add(category_name);
#endif

		if (!super.CanDisplayAttachmentCategory(category_name))
			return false;

		category_name.ToLower();
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

		if (category_name.Contains("engine"))
		{
			if (GetCarDoorsState("Hatchback_02_Hood") == CarDoorState.DOORS_CLOSED)
				return false;
		}

		return true;
	}

	override bool CanDisplayCargo()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CanDisplayCargo");
#endif

		if (!super.CanDisplayCargo())
			return false;

		if (GetCarDoorsState("Hatchback_02_Trunk") == CarDoorState.DOORS_CLOSED)
			return false;

		return false;
	}

	override int GetCarDoorsState(string slotType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetCarDoorsState").Add(slotType);
#endif

		CarDoor carDoor;

		Class.CastTo(carDoor, FindAttachmentBySlotName(slotType));
		if (!carDoor)
			return CarDoorState.DOORS_MISSING;

		switch (slotType)
		{
		case "Hatchback_02_Door_1_1":
			if (GetAnimationPhase("DoorsDriver") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;

			break;

		case "Hatchback_02_Door_2_1":
			if (GetAnimationPhase("DoorsCoDriver") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;

			break;

		case "Hatchback_02_Door_1_2":
			if (GetAnimationPhase("DoorsCargo1") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;

			break;

		case "Hatchback_02_Door_2_2":
			if (GetAnimationPhase("DoorsCargo2") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;
			break;

		case "Hatchback_02_Hood":
			if (GetAnimationPhase("DoorsHood") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;

		case "Hatchback_02_Trunk":
			if (GetAnimationPhase("DoorsTrunk") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;

			break;
		}

		return CarDoorState.DOORS_MISSING;
	}

	override bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			if (GetCarDoorsState("Hatchback_02_Door_1_1") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
			break;

		case 1:
			if (GetCarDoorsState("Hatchback_02_Door_2_1") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
			break;

		case 2:
			if (GetCarDoorsState("Hatchback_02_Door_1_2") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
			break;

		case 3:
			if (GetCarDoorsState("Hatchback_02_Door_2_2") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
			break;
		}

		return false;
	}

	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorSelectionNameFromSeatPos").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return "doors_driver";
			break;
		case 1:
			return "doors_codriver";
			break;
		case 2:
			return "doors_cargo1";
			break;
		case 3:
			return "doors_cargo2";
			break;
		}

		return super.GetDoorSelectionNameFromSeatPos(posIdx);
	}

	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorInvSlotNameFromSeatPos").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return "Hatchback_02_Door_1_1";
			break;
		case 1:
			return "Hatchback_02_Door_2_1";
			break;
		case 2:
			return "Hatchback_02_Door_1_2";
			break;
		case 3:
			return "Hatchback_02_Door_2_2";
			break;
		}

		return super.GetDoorInvSlotNameFromSeatPos(posIdx);
	}

	override float OnSound(CarSoundCtrl ctrl, float oldValue)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "OnSound").Add(ctrl).Add(oldValue);
#endif

		switch (ctrl)
		{
		case CarSoundCtrl.DOORS:
			float newValue = 0;

			//-----
			if (GetCarDoorsState("Hatchback_02_Door_1_1") == CarDoorState.DOORS_CLOSED)
				newValue += 0.25;

			//-----
			if (GetCarDoorsState("Hatchback_02_Door_2_1") == CarDoorState.DOORS_CLOSED)
				newValue += 0.25;

			//-----
			if (GetCarDoorsState("Hatchback_02_Door_1_2") == CarDoorState.DOORS_CLOSED)
				newValue += 0.25;

			//-----
			if (GetCarDoorsState("Hatchback_02_Door_2_2") == CarDoorState.DOORS_CLOSED)
				newValue += 0.25;

			if (newValue > 1)
				newValue = 1;

			return newValue;
			break;
		}

		return super.OnSound(ctrl, oldValue);
	}

	override string GetAnimSourceFromSelection(string selection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif

		switch (selection)
		{
		case "doors_driver":
			return "DoorsDriver";
		case "doors_codriver":
			return "DoorsCoDriver";
		case "doors_cargo1":
			return "DoorsCargo1";
		case "doors_cargo2":
			return "DoorsCargo2";
		case "doors_hood":
			return "DoorsHood";
		case "doors_trunk":
			return "DoorsTrunk";
		}

		return "";
	}

	override bool IsVitalTruckBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalTruckBattery");
#endif

		return false;
	}

	override bool IsVitalGlowPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalGlowPlug");
#endif

		return false;
	}

	override bool CanReachSeatFromSeat(int currentSeat, int nextSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachSeatFromSeat").Add(currentSeat).Add(nextSeat);
#endif

		switch (currentSeat)
		{
		case 0:
			if (nextSeat == 1)
				return true;
			break;
		case 1:
			if (nextSeat == 0)
				return true;
			break;
		case 2:
			if (nextSeat == 3)
				return true;
			break;
		case 3:
			if (nextSeat == 2)
				return true;
			break;
		}

		return false;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		switch (pCurrentSeat)
		{
		case 0:
			if (pDoorsSelection == "DoorsDriver")
			{
				return true;
			}
			break;
		case 1:
			if (pDoorsSelection == "DoorsCoDriver")
			{
				return true;
			}
			break;
		case 2:
			if (pDoorsSelection == "DoorsCargo1")
			{
				return true;
			}
			break;
		case 3:
			if (pDoorsSelection == "DoorsCargo2")
			{
				return true;
			}
			break;
		}

		return false;
	}

	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("Expansion_Hatchback_02_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Hatchback_02_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Hatchback_02_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Hatchback_02_Wheel");

			entity.GetInventory().CreateInInventory("CarBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("CarRadiator");

			entity.GetInventory().CreateInInventory("Hatchback_02_Door_1_1");
			entity.GetInventory().CreateInInventory("Hatchback_02_Door_1_2");
			entity.GetInventory().CreateInInventory("Hatchback_02_Door_2_1");
			entity.GetInventory().CreateInInventory("Hatchback_02_Door_2_2");
			entity.GetInventory().CreateInInventory("Hatchback_02_Hood");
			entity.GetInventory().CreateInInventory("Hatchback_02_Trunk");

			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};
