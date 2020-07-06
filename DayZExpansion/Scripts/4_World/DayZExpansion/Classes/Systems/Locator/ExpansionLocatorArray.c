

/**@class		ExpansionLocatorArray
 * @brief		This class handle expansion locator array
 **/
class ExpansionLocatorArray
{
	vector position;
	string name;
	string type;
	
	void ExpansionLocatorArray( vector pos, string Name, string Type ) 
	{
		this.position = pos;
		this.name = Name;
		this.type = Type;
	}
}