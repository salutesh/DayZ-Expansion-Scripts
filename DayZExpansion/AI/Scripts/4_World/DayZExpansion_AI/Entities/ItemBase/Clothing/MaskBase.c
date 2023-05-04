modded class MaskBase
{
	override bool ConsumeQuantity(float quantity, PlayerBase consumer_player)
	{
		//! AI don't consume gasmask/filter
		if (consumer_player.IsAI())
			return false;

		return super.ConsumeQuantity(quantity, consumer_player);
	}
}