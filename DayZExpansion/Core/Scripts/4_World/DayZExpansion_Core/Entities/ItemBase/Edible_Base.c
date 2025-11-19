/**
 * Edible_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Edible_Base
{
	//! Similar to vanilla ProcessDecay, but call to GetHealth01 has been replaced with GetHealthLevelValue so it works on client (used in P2P market and personal storage previews)
	//! @note due to some intended randomness in the decay values, computed state may be different on client than actual state on server
	FoodStageType Expansion_GetProcessedFoodStageDecay( float delta, bool hasRootAsPlayer )
	{
		int decayTimer;
		FoodStageType processedFoodStage = FoodStageType.NONE;
		FoodStageType lastDecayStage;
		float decayDelta = 0.0;
		decayDelta *= DayZGame.Cast(g_Game).GetFoodDecayModifier();
		decayDelta += ( 1 + ( 1 - GetHealthLevelValue(GetHealthLevel()) ) );
		if ( hasRootAsPlayer )
			decayDelta += GameConstants.DECAY_RATE_ON_PLAYER;

		if ( IsFruit() || IsMushroom() )
		{
			switch ( GetFoodStageType() )
			{
				case FoodStageType.RAW:
					decayTimer = ( GameConstants.DECAY_FOOD_RAW_FRVG + ( Math.RandomFloat01() * ( GameConstants.DECAY_FOOD_RAW_FRVG * ( GameConstants.DECAY_TIMER_RANDOM_PERCENTAGE / 100.0 ) ) ) );
					lastDecayStage = FoodStageType.RAW;
					break;

				case FoodStageType.BOILED:
					decayTimer = ( GameConstants.DECAY_FOOD_BOILED_FRVG + ( Math.RandomFloat01() * ( GameConstants.DECAY_FOOD_BOILED_FRVG * ( GameConstants.DECAY_TIMER_RANDOM_PERCENTAGE / 100.0 ) ) ) );
					lastDecayStage = FoodStageType.BOILED;
					break;

				case FoodStageType.BAKED:
					decayTimer = ( GameConstants.DECAY_FOOD_BAKED_FRVG + ( Math.RandomFloat01() * ( GameConstants.DECAY_FOOD_BAKED_FRVG * ( GameConstants.DECAY_TIMER_RANDOM_PERCENTAGE / 100.0 ) ) ) );
					lastDecayStage = FoodStageType.BAKED;
					break;

				case FoodStageType.DRIED:
				case FoodStageType.BURNED:
				case FoodStageType.ROTTEN:
				default:
					decayTimer = -1;
					lastDecayStage = FoodStageType.NONE;
					break;
			}

			decayTimer -= ( delta * decayDelta );

			if ( decayTimer <= 0 )
			{
				if ( lastDecayStage != FoodStageType.NONE )
				{
					// switch to decayed stage
					if ( ( lastDecayStage == FoodStageType.BOILED ) || ( lastDecayStage == FoodStageType.BAKED ) )
					{
						processedFoodStage = FoodStageType.ROTTEN;
					}
					if ( lastDecayStage == FoodStageType.RAW )
					{
						int rng = Math.RandomIntInclusive( 0, 100 );
						if ( rng > GameConstants.DECAY_FOOD_FRVG_DRIED_CHANCE )
						{
							processedFoodStage = FoodStageType.ROTTEN;
						}
						else
						{
							if ( CanChangeToNewStage( FoodStageType.DRIED ) )
							{
								processedFoodStage = FoodStageType.DRIED;
							}
							else
							{
								processedFoodStage = FoodStageType.ROTTEN;
							}
						}
					}
				}
			}

		}
		else if ( IsMeat() )
		{
			switch ( GetFoodStageType() )
			{
				case FoodStageType.RAW:
					decayTimer = ( GameConstants.DECAY_FOOD_RAW_MEAT + ( Math.RandomFloat01() * ( GameConstants.DECAY_FOOD_RAW_MEAT * ( GameConstants.DECAY_TIMER_RANDOM_PERCENTAGE / 100.0 ) ) ) );
					lastDecayStage = FoodStageType.RAW;
					break;

				case FoodStageType.BOILED:
					decayTimer = ( GameConstants.DECAY_FOOD_BOILED_MEAT + ( Math.RandomFloat01() * ( GameConstants.DECAY_FOOD_BOILED_MEAT * ( GameConstants.DECAY_TIMER_RANDOM_PERCENTAGE / 100.0 ) ) ) );
					lastDecayStage = FoodStageType.BOILED;
					break;

				case FoodStageType.BAKED:
					decayTimer = ( GameConstants.DECAY_FOOD_BAKED_MEAT + ( Math.RandomFloat01() * ( GameConstants.DECAY_FOOD_BAKED_MEAT * ( GameConstants.DECAY_TIMER_RANDOM_PERCENTAGE / 100.0 ) ) ) );
					lastDecayStage = FoodStageType.BAKED;
					break;

				case FoodStageType.DRIED:
					decayTimer = ( GameConstants.DECAY_FOOD_DRIED_MEAT + ( Math.RandomFloat01() * ( GameConstants.DECAY_FOOD_DRIED_MEAT * ( GameConstants.DECAY_TIMER_RANDOM_PERCENTAGE / 100.0 ) ) ) );
					lastDecayStage = FoodStageType.DRIED;
					break;

				case FoodStageType.BURNED:
				case FoodStageType.ROTTEN:
				default:
					decayTimer = -1;
					lastDecayStage = FoodStageType.NONE;
					break;
			}

			decayTimer -= ( delta * decayDelta );

			if ( decayTimer <= 0 )
			{
				if ( lastDecayStage != FoodStageType.NONE )
				{
					// switch to decayed stage
					if ( ( lastDecayStage == FoodStageType.DRIED ) || ( lastDecayStage == FoodStageType.RAW ) || ( lastDecayStage == FoodStageType.BOILED ) || ( lastDecayStage == FoodStageType.BAKED ) )
					{
						processedFoodStage = FoodStageType.ROTTEN;
					}
				}
			}
		}
		else if ( IsCorpse() )
		{
			// corpse
			if ( lastDecayStage != GetFoodStageType() )
			{
				switch ( GetFoodStageType() )
				{
					case FoodStageType.RAW:
						decayTimer = ( GameConstants.DECAY_FOOD_RAW_CORPSE + ( Math.RandomFloat01() * ( GameConstants.DECAY_FOOD_RAW_CORPSE * ( GameConstants.DECAY_TIMER_RANDOM_PERCENTAGE / 100.0 ) ) ) );
						lastDecayStage = FoodStageType.RAW;
						break;

					case FoodStageType.BURNED:
					case FoodStageType.ROTTEN:
					default:
						decayTimer = -1;
						lastDecayStage = FoodStageType.NONE;
						break;
				}
			}

			decayTimer -= ( delta * decayDelta );

			if ( decayTimer <= 0 )
			{
				if ( lastDecayStage != FoodStageType.NONE )
				{
					// switch to decayed stage
					if ( ( lastDecayStage == FoodStageType.DRIED ) || ( lastDecayStage == FoodStageType.RAW ) || ( lastDecayStage == FoodStageType.BOILED ) || ( lastDecayStage == FoodStageType.BAKED ) )
					{
						processedFoodStage = FoodStageType.ROTTEN;
					}
				}
			}
		}

		if (processedFoodStage == FoodStageType.NONE && m_FoodStage)
			processedFoodStage = GetFoodStageType();

		return processedFoodStage;
	}
};