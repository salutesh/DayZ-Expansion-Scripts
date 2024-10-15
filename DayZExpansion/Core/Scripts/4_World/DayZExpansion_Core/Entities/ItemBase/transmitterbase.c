modded class TransmitterBase
{
	//! Vanilla TransmitterBase::OnWorkStart doesn't call super
	override void OnWorkStart()
	{
		super.OnWorkStart();

		Expansion_OnWorkStart();
	}

	//! Vanilla TransmitterBase::OnWorkStop doesn't call super
	override void OnWorkStop()
	{
		super.OnWorkStop();

		Expansion_OnWorkStop();
	}
}
