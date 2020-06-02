class ExpansionMarketNetworkItem
{
	int CategoryID;

	string ClassName;
	TStringArray SkinsClassNames;

	int Stock;

	// todo: optimize and add min/max stock/price
	// todo: write a byte buffer class to push optimized strings

	// type byte
	// catid byte
	// numSkins byte
	// clsNameLen byte
	// className byte[]
	// arr[numSkins]:
	// 	skinNameLen byte
	// 	skinName byte[]
	// maxStock int16
	// minStock int16
	// maxPrice int16
	// minPrice int16
	// stock in16
	// endBuffer byte[]
}