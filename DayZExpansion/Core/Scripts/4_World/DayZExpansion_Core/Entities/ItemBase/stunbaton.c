modded class StunBaton
{
	//! Vanilla StunBaton::OnWorkStart doesn't call super
	override void OnWorkStart()
	{
		super.OnWorkStart();

		Expansion_OnWorkStart();
	}

	//! Vanilla StunBaton::OnWorkStop doesn't call super
	override void OnWorkStop()
	{
		super.OnWorkStop();

		Expansion_OnWorkStop();
	}
}
