modded class CattleProd
{
	//! Vanilla CattleProd::OnWorkStart doesn't call super
	override void OnWorkStart()
	{
		super.OnWorkStart();

		Expansion_OnWorkStart();
	}

	//! Vanilla CattleProd::OnWorkStop doesn't call super
	override void OnWorkStop()
	{
		super.OnWorkStop();

		Expansion_OnWorkStop();
	}
}
