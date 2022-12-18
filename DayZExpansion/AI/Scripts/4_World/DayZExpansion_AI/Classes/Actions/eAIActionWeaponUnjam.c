[RegisterAction(eAIActionWeaponUnjam)]
class eAIActionWeaponUnjam : eAIActionWeaponControl
{
	override void Start(ActionData action_data)
	{
		super.Start(action_data);
		
		action_data.m_Player.GetWeaponManager().Unjam(this);
	}
};
