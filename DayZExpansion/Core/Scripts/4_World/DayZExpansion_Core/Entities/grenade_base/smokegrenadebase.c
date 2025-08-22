modded class SmokeGrenadeBase
{
	//! Vanilla SmokeGrenadeBase::OnWorkStart doesn't call super
	override void OnWorkStart()
	{
		super.OnWorkStart();

		Expansion_OnWorkStart();
	}

	//! Vanilla SmokeGrenadeBase::OnWorkStop doesn't call super
	override void OnWorkStop()
	{
		super.OnWorkStop();

		Expansion_OnWorkStop();
	}

	override bool Expansion_IsExplosive()
	{
		return false;
	}
}
