modded class Blowtorch
{
	//! Vanilla Blowtorch::OnWorkStop doesn't call super
	override void OnWorkStop()
	{
		super.OnWorkStop();

		Expansion_OnWorkStop();
	}
}
