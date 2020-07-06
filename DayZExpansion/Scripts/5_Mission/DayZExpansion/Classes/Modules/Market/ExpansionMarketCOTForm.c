class ExpansionMarketCOTForm extends JMFormBase
{
	private Widget m_ActionsWrapper;

	private UIActionEditableText m_Number;

   	private autoptr array< ExpansionMoneyBase > m_Money;

	void ExpansionMarketCOTForm()
	{
		m_Money = new array< ExpansionMoneyBase >;
	}

	void ~ExpansionMarketCOTForm()
	{
	}

	override void OnInit()
	{
		Widget wrapper = layoutRoot.FindAnyWidget( "actions_wrapper" );

		m_Number = UIActionManager.CreateEditableText( wrapper, "Number:" );
		m_Number.SetOnlyNumbers( true );

		UIActionManager.CreateButton( wrapper, "Give Money", this, "Give" );

		UIActionManager.CreateButton( wrapper, "Reserve Money", this, "Reserve" );

		UIActionManager.CreateButton( wrapper, "Delete Money", this, "Delete" );

		UIActionManager.CreateButton( wrapper, "Unlock Money", this, "Unlock" );

		UIActionManager.CreateButton( wrapper, "Open Random Market Menu", this, "OpenRandom" );
	}

	void OpenRandom( UIEvent eid, ref UIActionBase action ) 
	{
		ExpansionMarketModule mod;
		if ( Class.CastTo( mod, GetModuleManager().GetModule( ExpansionMarketModule ) ) )
		{
			int count = ExpansionTraderBase.GetAll().Count();
			ExpansionTraderBase trader = ExpansionTraderBase.GetAll()[ Math.RandomInt( 0, count ) ];
			
			GetCommunityOnlineToolsBase().SetOpen( false );

			ExpansionMarketCOTModule testMod;
			if ( Class.CastTo( testMod, GetModuleManager().GetModule( ExpansionMarketCOTModule ) ) )
			{
				testMod.Hide();

				mod.OpenTraderMenu();
				mod.LoadTraderData( trader, NULL );
			}
		}
	}

	void Give( UIEvent eid, ref UIActionBase action ) 
	{
		ExpansionMarketModule mod;
		if ( Class.CastTo( mod, GetModuleManager().GetModule( ExpansionMarketModule ) ) )
		{
			float amount = m_Number.GetText().ToFloat();

			mod.SpawnMoney( PlayerBase.Cast(GetGame().GetPlayer()), amount );
		}
	}

	void Reserve( UIEvent eid, ref UIActionBase action ) 
	{
		ExpansionMarketModule mod;
		if ( Class.CastTo( mod, GetModuleManager().GetModule( ExpansionMarketModule ) ) )
		{
			float amount = m_Number.GetText().ToFloat();

			mod.FindMoneyAndReserve( PlayerBase.Cast(GetGame().GetPlayer()), amount, m_Money );
		}
	}

	void Delete( UIEvent eid, ref UIActionBase action ) 
	{
		ExpansionMarketModule mod;
		if ( Class.CastTo( mod, GetModuleManager().GetModule( ExpansionMarketModule ) ) )
		{
			mod.RemoveMoney( m_Money );
		}
	}

	void Unlock( UIEvent eid, ref UIActionBase action ) 
	{
		ExpansionMarketModule mod;
		if ( Class.CastTo( mod, GetModuleManager().GetModule( ExpansionMarketModule ) ) )
		{
			mod.UnlockMoney( PlayerBase.Cast(GetGame().GetPlayer()) );
		}
	}
}