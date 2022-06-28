class eAINPCBase: eAIBase
{
	override void Init()
	{
		super.Init();

		if (GetGame().IsServer())
		{
			GetGroup().SetFaction(new eAIFactionPassive());
		}
	}
}
