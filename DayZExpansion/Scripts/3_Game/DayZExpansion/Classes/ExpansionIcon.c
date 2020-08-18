class ExpansionIcon : Managed
{
	string Name;
	string Localisation;
	string Path;

	void ExpansionIcon( string name, string localisation, string path )
	{
		Name = name;
		Localisation = localisation;
		Path = path;
	}
};