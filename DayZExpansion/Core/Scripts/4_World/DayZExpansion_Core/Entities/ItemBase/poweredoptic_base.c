modded class PoweredOptic_Base
{
	//! Vanilla ItemOptics::OnWorkStart doesn't call super and is not moddable
	override void OnWorkStart()
	{
		super.OnWorkStart();

		Expansion_OnWorkStart();
	}

	//! Vanilla ItemOptics::OnWorkStop doesn't call super and is not moddable
	override void OnWorkStop()
	{
		super.OnWorkStop();

		Expansion_OnWorkStop();
	}
}
