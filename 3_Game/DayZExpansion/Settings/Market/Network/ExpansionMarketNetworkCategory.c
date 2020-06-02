class ExpansionMarketNetworkCategory
{
	string Name;

	int CategoryID;

	void ExpansionMarketNetworkCategory( ExpansionMarketCategory category )
	{
		Name = category.DisplayName;
		CategoryID = category.CategoryID;
	}
}