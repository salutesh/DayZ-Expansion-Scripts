class ExpansionSettingBase
{
	void ExpansionSettingBase()
	{
	}

	void ~ExpansionSettingBase()
	{
	}

	void HandleRPC( ref ParamsReadContext ctx )
	{
		Error("Not implemented");
	}

	bool Load()
	{
		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			Defaults();
			return false;
		}

		return OnLoad();
	}

	protected bool OnLoad()
	{
		Error("Not implemented");
		return false;
	}

	void Unload()
	{
		Error("Not implemented");
	}

	bool Save()
	{
		if ( GetGame().IsClient() )
		{
			return false;
		}

		return OnSave();
	}

	protected bool OnSave()
	{
		Error("Not implemented");
		return false;
	}

	void Defaults()
	{
		Error("Not implemented");
	}

	bool IsLoaded()
	{
		Error("Not implemented");
		return true;
	}

	bool Copy( ExpansionSettingBase setting )
	{
		Error("Not implemented");
		return false;
	}

	int Send( PlayerIdentity identity )
	{
		Error("Not implemented");
		return 0;
	}

	void Update( ExpansionSettingBase setting )
	{
		Copy( setting );
		Save();

		if ( !IsMissionClient() )
			Send( NULL );
	}
}