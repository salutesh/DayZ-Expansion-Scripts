modded class PortableGasStove
{
	//! Vanilla PortableGasStove::OnWorkStop doesn't call super
	override void OnWorkStop()
	{
		super.OnWorkStop();

		Expansion_OnWorkStop();
	}
}
