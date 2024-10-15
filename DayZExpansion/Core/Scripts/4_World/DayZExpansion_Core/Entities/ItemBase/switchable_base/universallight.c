modded class UniversalLight
{
	//! Vanilla UniversalLight::OnWorkStart doesn't call super
	override void OnWorkStart()
	{
		super.OnWorkStart();

		Expansion_OnWorkStart();
	}

	//! Vanilla UniversalLight::OnWorkStop doesn't call super
	override void OnWorkStop()
	{
		super.OnWorkStop();

		Expansion_OnWorkStop();
	}
}
