#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Core/StringUtils.hpp"

int ParseXmlAttribute(XmlElement const& element, char const* attributeName, int defaultValue)
{
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValue;
	}

	return std::stoi(attr);

}

char ParseXmlAttribute(XmlElement const& element, char const* attributeName, char defaultValue)
{
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValue;
	}

	return attr[0];
}

bool ParseXmlAttribute(XmlElement const& element, char const* attributeName, bool defaultValue)
{
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValue;
	}

	std::string result = attr;

	if (result == "true" || result == "1")
	{
		return true;
	}
	else if (result == "false" || result == "0")
	{
		return false;
	}

	return defaultValue;
}

float ParseXmlAttribute(XmlElement const& element, char const* attributeName, float defaultValue)
{
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValue;
	}
	return std::stof(attr);

}

Rgba8 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Rgba8 const& defaultValue)
{
	Rgba8 result;
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValue;
	}
	result.SetFromText(attr);
	return result;

}

Vec2 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Vec2 const& defaultValue)
{
	Vec2 result;
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValue;
	}

	result.SetFromText(attr);
	return result;
}

Vec3 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Vec3 const& defaultValue)
{
	Vec3 result;
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValue;
	}

	result.SetFromText(attr);
	return result;
}

IntVec2 ParseXmlAttribute(XmlElement const& element, char const* attributeName, IntVec2 const& defaultValue)
{
	IntVec2 result;
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValue;
	}
	result.SetFromText(attr);
	return result;

}

std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, std::string const& defaultValue)
{
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValue;
	}
	std::string result = attr;
	return result;

}

Strings ParseXmlAttribute(XmlElement const& element, char const* attributeName, Strings const& defaultValues)
{
	std::string attr = element.Attribute(attributeName);
	Strings result;

	if (!attr.empty())
	{
		result = SplitStringOnDelimiter(attr.c_str(), ',');
		return result;
	}

	return defaultValues;
}

FloatRange ParseXmlAttribute(XmlElement const& element, char const* attributeName, FloatRange const& defaultValues)
{
	FloatRange result;
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValues;
	}

	result.SetFromText(attr);
	return result;
}

std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, char const* defaultValue)
{
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValue;
	}
	return attr;

}

EulerAngles ParseXmlAttribute(XmlElement const& element, char const* attributeName, EulerAngles const& defaultValue)
{
	EulerAngles result;
	const char* attr = element.Attribute(attributeName);

	if (attr == NULL)
	{
		return defaultValue;
	}

	result.SetFromText(attr);
	return result;
}
