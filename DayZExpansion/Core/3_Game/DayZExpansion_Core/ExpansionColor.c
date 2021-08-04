/**
 * ExpansionColor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionColor
{
	static const ref TStringArray HEX_BYTES = {
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"
	};

	//! DayZ ARGB int to RRGGBBAA hex color string
	static string ARGBToHex(int color)
	{
		int a, r, g, b;

		a = color >> 24;
		if (a < 0)
			a += 256;
		r = (color & 0x00ff0000) >> 16;
		g = (color & 0x0000ff00) >> 8;
		b = (color & 0x000000ff);

		return ARGBToHex(a, r, g, b);
	}

	//! ARGB to RRGGBBAA hex color string
	static string ARGBToHex(int a, int r, int g, int b)
	{
		string hexcolor;

		hexcolor += IntToHex(r);
		hexcolor += IntToHex(g);
		hexcolor += IntToHex(b);
		hexcolor += IntToHex(a);

		return hexcolor;
	}

	//! RRGGBB or RRGGBBAA hex color string to DayZ ARGB color int
	static int HexToARGB(string hexcolor)
	{
		if (hexcolor.Length() != 6 && hexcolor.Length() != 8)
			return 0;

		int color[4];

		if (hexcolor.Length() == 6)
			color[3] = 255;  //! Set alpha to fully opaque

		for (int i = 0; i < hexcolor.Length() / 2; i++)
		{
			color[i] = HexToInt(hexcolor.Substring(i * 2, 2));
		}

		return ARGB(color[3], color[0], color[1], color[2]);
	}

	static int HexToInt(string hex)
	{
		hex.ToUpper();

		if (hex.Length() % 2)
			hex = "0" + hex;

		int n;

		for (int i = 0; i < hex.Length();)
		{
			n += (HEX_BYTES.Find(hex[i]) * 16) + HEX_BYTES.Find(hex[i + 1]);
			i += 2;
		}

		return n;
	}

	static string IntToHex(int n)
	{
		int orig = n;
		string hex;

		while (true)
		{
			hex = HEX_BYTES[n % 16] + hex;
			n /= 16;
			if (n <= 0)
				break;
		}

		if (hex.Length() % 2)
			hex = "0" + hex;

		return hex;
	}
}
