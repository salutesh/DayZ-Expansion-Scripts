modded class BoatScript
{
	override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
	{
		super.GetActions(action_input_type, actions);

		//! Prevent NULL ptr log spam in ToggleVehicleLightsActionInput::UpdatePossibleActions (https://report.bistudio.com/issues/DZG-2522)
		if (!actions)
			actions = {};  
	}
}
