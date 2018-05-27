#include "stdafx.h"

// General
#include "File.h"

File::File(cstring _fullFileName) :
	m_Name(_fullFileName),
	m_Path("")
{
	ParsePathAndExtension();
}

File::File(cstring _name, cstring _path) :
	m_Name(_name),
	m_Path(_path)
{
	ParsePathAndExtension();
}

File::~File()
{
}

void File::FixFilePath(string& _string)
{
	size_t index = 0;
	while (true)
	{
		index = _string.find('/', index);
		if (index == std::string::npos)
		{
			break;
		}
		_string.replace(index, 1, "\\");
		index += 3;
	}
}

void File::ParsePathAndExtension()
{
	// Replace slahes
	FixFilePath(m_Name);

	// Find name and path
	auto lastSlashPos = m_Name.find_last_of('\\');
	if (lastSlashPos != string::npos)
	{
		m_Path += m_Name.substr(0, lastSlashPos + 1);
		m_Name = m_Name.substr(lastSlashPos + 1);
	}

	// Extension
	auto lastPointPos = m_Name.find_last_of('.');
	if (lastPointPos != string::npos)
	{
		m_Extension = Utils::ToLower(m_Name.substr(lastPointPos + 1));
	}
}