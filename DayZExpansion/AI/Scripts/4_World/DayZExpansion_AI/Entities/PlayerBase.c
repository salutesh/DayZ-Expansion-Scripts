modded class PlayerBase
{
	override void Init()
	{
		super.Init();

		if (!IsAI())
			RegisterNetSyncVariableFloat("m_eAI_LastAggressionTimeout", 0.0, float.MAX, 2);
	}

	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		AddAction(ExpansionActionRecruitAI, InputActionMap);
		AddAction(ExpansionActionDismissAI, InputActionMap);
	}
};
