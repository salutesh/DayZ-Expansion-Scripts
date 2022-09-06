class Vehicle_OffroadHatchback extends ExpansionVehicleCarBase
{
	void Vehicle_OffroadHatchback()
	{
		m_dmgContactCoef = 0.075;
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return VehicleAnimInstances.HATCHBACK;
	}

	override float GetTransportCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraDistance");
#endif

		return 3.5;
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

		return CarRearLightBase.Cast(ScriptedLightBase.CreateLight(OffroadHatchbackRearLight));
	}

	override CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		return CarLightBase.Cast(ScriptedLightBase.CreateLight(OffroadHatchbackFrontLight));
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CanReleaseAttachment").Add(attachment);
#endif

		if (!super.CanReleaseAttachment(attachment))
			return false;

		string attType = attachment.GetType();

		if (EngineIsOn() || GetCarDoorsState("NivaHood") == CarDoorState.DOORS_CLOSED)
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
			if (GetCarDoorsState("NivaHood") == CarDoorState.DOORS_CLOSED)
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

		if (GetCarDoorsState("NivaTrunk") == CarDoorState.DOORS_CLOSED)
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
		case "NivaDriverDoors":
			if (GetAnimationPhase("DoorsDriver") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;

			break;

		case "NivaCoDriverDoors":
			if (GetAnimationPhase("DoorsCoDriver") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;

			break;

		case "NivaHood":
			if (GetAnimationPhase("DoorsHood") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;

			break;

		case "NivaTrunk":
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

		CarDoor carDoor;
		switch (posIdx)
		{
		case 0:
			if (GetCarDoorsState("NivaDriverDoors") == CarDoorState.DOORS_CLOSED)
				return false;
			else if (GetAnimationPhase("SeatDriver") > 0.5)
				return false;

			return true;
			break;

		case 1:
			if (GetCarDoorsState("NivaCoDriverDoors") == CarDoorState.DOORS_CLOSED)
				return false;
			else if (GetAnimationPhase("SeatCoDriver") > 0.5)
				return false;

			return true;
			break;

		case 2:
			if (GetCarDoorsState("NivaDriverDoors") == CarDoorState.DOORS_CLOSED)
				return false;
			else if (GetAnimationPhase("SeatDriver") <= 0.5)
				return false;

			return true;
			break;

		case 3:
			if (GetCarDoorsState("NivaCoDriverDoors") == CarDoorState.DOORS_CLOSED)
				return false;
			else if (GetAnimationPhase("SeatCoDriver") <= 0.5)
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
		case 2:
			return "doors_driver";
			break;
		case 1:
		case 3:
			return "doors_codriver";
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
		case 2:
			return "NivaDriverDoors";
			break;
		case 1:
		case 3:
			return "NivaCoDriverDoors";
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
			if (GetCarDoorsState("NivaDriverDoors") == CarDoorState.DOORS_CLOSED)
				newValue += 0.5;

			if (GetCarDoorsState("NivaCoDriverDoors") == CarDoorState.DOORS_CLOSED)
				newValue += 0.5;

			if (GetCarDoorsState("NivaTrunk") == CarDoorState.DOORS_CLOSED)
				newValue += 0.3;

			if (GetHealthLevel("WindowFront") == GameConstants.STATE_RUINED)
				newValue -= 0.6;

			if (GetHealthLevel("WindowLR") == GameConstants.STATE_RUINED)
				newValue -= 0.2;

			if (GetHealthLevel("WindowRR") == GameConstants.STATE_RUINED)
				newValue -= 0.2;

			if (newValue > 1)
				newValue = 1;

			if (newValue < 0)
				newValue = 0;

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
		case "doors_hood":
			return "DoorsHood";
		case "doors_trunk":
			return "DoorsTrunk";
		case "seatback_driver":
			return "SeatDriver";
		case "seatback_codriver":
			return "SeatCoDriver";
		}

		return "";
	}

	override string GetDoorConditionPointFromSelection(string selection)
	{
		switch (selection)
		{
		case "seat_driver":
		case "seatback_driver":
		case "seat_cargo1":
			return "seat_con_1_1";
			break;
		case "seat_codriver":
		case "seatback_codriver":
		case "seat_cargo2":
			return "seat_con_2_1";
			break;
		}

		return "";
	}

	override int GetCrewIndex(string selection)
	{
		switch (selection)
		{
		case "seatback_driver":
			return 0;
		case "seatback_codriver":
			return 1;
		}

		return -1;
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

		if (nextSeat == 0 && GetAnimationPhase("SeatDriver") > 0.5)
			return false;

		if (nextSeat == 1 && GetAnimationPhase("SeatCoDriver") > 0.5)
			return false;

		switch (currentSeat)
		{
		case 0:
			if (nextSeat == 1)
			{
				return true;
			}
			break;
		case 1:
			if (nextSeat == 0)
			{
				return true;
			}
			break;
		case 2:
			if (nextSeat == 3)
			{
				return true;
			}
			break;
		case 3:
			if (nextSeat == 2)
			{
				return true;
			}
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
			if (pDoorsSelection == "DoorsDriver")
			{
				return true;
			}
			break;
		case 3:
			if (pDoorsSelection == "DoorsCoDriver")
			{
				return true;
			}
			break;
		}

		return false;
	}

	override int GetSeatIndexFromDoor(string pDoorSelection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatIndexFromDoor").Add(pDoorSelection);
#endif

		switch (pDoorSelection)
		{
		case "DoorsDriver":
			return 0;
			break;
		case "DoorsCoDriver":
			return 1;
			break;
		}
		return -1;
	}

	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("Expansion_HatchbackWheel");
			entity.GetInventory().CreateInInventory("Expansion_HatchbackWheel");
			entity.GetInventory().CreateInInventory("Expansion_HatchbackWheel");
			entity.GetInventory().CreateInInventory("Expansion_HatchbackWheel");

			entity.GetInventory().CreateInInventory("CarBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("CarRadiator");

			entity.GetInventory().CreateInInventory("HatchbackDoors_Driver");
			entity.GetInventory().CreateInInventory("HatchbackDoors_CoDriver");
			entity.GetInventory().CreateInInventory("HatchbackHood");
			entity.GetInventory().CreateInInventory("HatchbackTrunk");

			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");
		};

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}

	override void SetActions()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "SetActions");
#endif

		super.SetActions();

		AddAction(ActionAnimateSeats);
	}
};
