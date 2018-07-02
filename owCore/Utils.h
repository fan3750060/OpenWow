#pragma once

class Utils
{
public:
    // Convert
    static   string ToString(const type_info& type, void* value);

    // Common types
    template <typename T>
    static   T ToType(cstring _string);

    static   bool ToBool(cstring _string);

    // My types
    static  vec2 ToPoint(string& _string);
    static  vec3 ToVector3(string& _string);
    static  Color ToColorFromName(string& _string);
    static  Color ToColorFromRGB(string& _string);
    static  Color ToColorFromRGBA(string& _string);
    static  TextAlignW ToTextAlignW(string& _string);
    static  TextAlignH ToTextAlignH(string& _string);
    static  InputMode ToInputMode(string& _string);

    // Parse
    static   string ParseSectionName(string& _string);
    static   string ParseSectionAndIncludeName(string& _string, string& _includeSectionName);
    static   void ParseKeyPair(string& _string, string& key, string& val);

    static   bool TryParse(const type_info& type, cstring _string, void* output);

    // String
    static   string Trim(string& _string, cstring delimiters = " \f\n\r\t\v");
    static   string TrimLeft(string& _string, cstring delimiters = " \f\n\r\t\v");
    static   string TrimRight(string& _string, cstring delimiters = " \f\n\r\t\v");

    static   string ToLower(cstring _string);
	static   string ToUpper(cstring _string);

    static   int popFirstInt(string& _string, char separator = 0);
    static   double popFirstDouble(string& _string, char separator = 0);
    static   float popFirstFloat(string& _string, char separator = 0);

    static   string GetFirstSubString(string& _string, char separator = 0);
    static   string getNextToken(cstring _string, uint32_t& cursor, char separator);
    static   string stripCarriageReturn(cstring line);
    static   string getLine(ifstream& infile);

    static   bool PointInRectangle(const vec2 _point, const vec2 _rectStart, const vec2 _rectEnd);
};

// Common types
template <typename T>
inline T Utils::ToType(cstring _string)
{
    T result;
    if (!(stringstream(_string) >> result))
    {
        result = 0;
    }
    return result;
}

inline bool getBitH2L(uint8* data, uint32 bit)
{
    uint8 mask = 0x80;
    mask >>= (bit % 8);

    return ((data[bit / 8]) & mask) == mask;
}

inline bool getBitL2H(uint8* data, uint32 bit)
{
    uint8 mask = 0x1;
    mask <<= (bit % 8);

    return ((data[bit / 8]) & mask) == mask;
}

inline vec4 fromARGB(uint32 color)
{
    const float a = ((color & 0xFF000000) >> 24) / 255.0f;
    const float r = ((color & 0x00FF0000) >> 16) / 255.0f;
    const float g = ((color & 0x0000FF00) >> 8) / 255.0f;
    const float b = ((color & 0x000000FF)) / 255.0f;
    return vec4(r, g, b, a);
}

inline vec3 fromRGB(uint32 color)
{
    const float r = ((color & 0xFF0000) >> 16) / 255.0f;
    const float g = ((color & 0x00FF00) >> 8) / 255.0f;
    const float b = ((color & 0x0000FF)) / 255.0f;
    return vec3(r, g, b);
}

inline vec4 fromBGRA(uint32 color)
{
    const float b = ((color & 0xFF000000) >> 24) / 255.0f;
    const float g = ((color & 0x00FF0000) >> 16) / 255.0f;
    const float r = ((color & 0x0000FF00) >> 8) / 255.0f;
    const float a = ((color & 0x000000FF)) / 255.0f;
    return vec4(r, g, b, a);
}

inline vec4 fromABGR(uint32 color)
{
    const float a = ((color & 0xFF000000) >> 24) / 255.0f;
    const float b = ((color & 0x00FF0000) >> 16) / 255.0f;
    const float g = ((color & 0x0000FF00) >> 8) / 255.0f;
    const float r = ((color & 0x000000FF)) / 255.0f;
    return vec4(r, g, b, a);
}

static inline void SwitchBool(bool& _value)
{
	_value = !_value;
}


static vec3 rotateV(cvec3 v, float theta)
{
	float cs = cosf(theta);
	float sn = sinf(theta);

	float X = v.x * cs - v.z * sn;
	float Z = v.x * sn + v.z * cs;

	vec3 neW = vec3(X, v.y, Z);

	return neW;
}



inline string Utf8_to_cp1251(const char *str)
{
	string res;

	int result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
	if (!result_u)
	{
		return 0;
	}

	wchar_t* ures = new wchar_t[result_u];
	if (!MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u))
	{
		delete[] ures;
		return 0;
	}

	int result_c = WideCharToMultiByte(1251, 0, ures, -1, 0, 0, 0, 0);
	if (!result_c)
	{
		delete[] ures;
		return 0;
	}

	char *cres = new char[result_c];
	if (!WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, 0, 0))
	{
		delete[] cres;
		return 0;
	}

	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}


inline string cp1251_to_utf8(const char *str)
{
	string res;
	int result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
	if (!result_u)
	{
		return 0;
	}

	wchar_t *ures = new wchar_t[result_u];
	if (!MultiByteToWideChar(1251, 0, str, -1, ures, result_u))
	{
		delete[] ures;
		return 0;
	}

	int result_c = WideCharToMultiByte(CP_UTF8, 0, ures, -1, 0, 0, 0, 0);
	if (!result_c)
	{
		delete[] ures;
		return 0;
	}

	char* cres = new char[result_c];
	if (!WideCharToMultiByte(CP_UTF8, 0, ures, -1, cres, result_c, 0, 0))
	{
		delete[] cres;
		return 0;
	}

	delete[] ures;
	res.append(cres);
	delete[] cres;

	return res;
}