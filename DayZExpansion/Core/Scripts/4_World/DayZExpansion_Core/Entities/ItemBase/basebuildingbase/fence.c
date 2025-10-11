modded class Fence
{
	override void Expansion_OnPartBuiltServer(ConstructionPart constructionPart)
	{
		//! Equivalent to vanilla Fence::OnPartBuiltServer (minus requirement for player)

		super.Expansion_OnPartBuiltServer(constructionPart);

		SetGateState(CheckGateState());

		UpdateVisuals();
	}
}
