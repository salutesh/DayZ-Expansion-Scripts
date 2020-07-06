class ExpansionMoneyBase extends ItemBase
{
	int ReservedAmount;

	bool IsReserved;

	float Worth;

	void ExpansionMoneyBase()
	{
		ExpansionMarketModule module;
		if ( Class.CastTo( module, GetModuleManager().GetModule( ExpansionMarketModule ) ) )
		{
			Worth = module.GetMoneyPrice( GetType() );
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( GetType() + " Worth is " + Worth );
		#endif

		RegisterNetSyncVariableBool( "IsReserved" );
	}

	// register net var "IsReserved"
	// any methods that need to be overridden, do so to prevent this 
	// item from being moved to any other inventory except the player 
	// while IsReserved is true.
	
	override bool CanRemoveFromCargo( EntityAI parent )
	{
		if ( !super.CanRemoveFromCargo( parent ) )
			return false;

		return !IsReserved;
	}

	override bool CanRemoveFromHands( EntityAI parent )
	{
		if ( !super.CanRemoveFromHands( parent ) )
			return false;

		return !IsReserved;
	}
	
}

class ExpansionSilverNugget: ExpansionMoneyBase {}
class ExpansionSilverBar: ExpansionMoneyBase {}
class ExpansionGoldNugget: ExpansionMoneyBase {}
class ExpansionGoldBar: ExpansionMoneyBase {}
 