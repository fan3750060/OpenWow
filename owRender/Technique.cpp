#include "stdafx.h"

// General
#include "Technique.h"

#pragma region Language

string ProcessInclude(IFile* f)
{
	if (f == nullptr)
	{
		Log::Error("Error open shader [%s].", f->Path_Name().c_str());
		return "";
	}

	string data = "";

	while (!f->isEof())
	{
		string line = f->readLine();

		// Skip empty lines
		if (line.length() == 0)
		{
			continue;
		}

		// Find directive
		if (line[0] == '#' && line[1] == 'i' && line[2] == 'n' && line[3] == 'c' && line[4] == 'l')
		{
			uint32_t firstBracketPosition = line.find('"');
            assert1(firstBracketPosition != string::npos);

			uint32_t lastBracketPosition = line.find_last_of('"');
			assert1(firstBracketPosition != lastBracketPosition);

			string inludeFileName = line.substr(firstBracketPosition + 1, lastBracketPosition - firstBracketPosition - 1);
			CFile::FixFilePath(inludeFileName);
			data += ProcessInclude(GetManager<IFilesManager>()->Open(f->Path() + inludeFileName)) + '\n';

			continue;
		}

		data += line + '\n';
	}

	return data;
}

#pragma endregion

Technique::Technique(RenderDevice* _RenderDevice, cstring _fileName)
	: m_RenderDevice(_RenderDevice)
{
	string shVS = ProcessInclude(GetManager<IFilesManager>()->Open(_fileName + ".vs"));
	string shFS = ProcessInclude(GetManager<IFilesManager>()->Open(_fileName + ".fs"));

    Process(_fileName, shVS.c_str(), shFS.c_str(), nullptr);
}

Technique::Technique(RenderDevice* _RenderDevice, cstring _fileNameVS, cstring _fileNameFS)
	: m_RenderDevice(_RenderDevice)
{
	string shVS = ProcessInclude(GetManager<IFilesManager>()->Open(_fileNameVS));
	string shFS = ProcessInclude(GetManager<IFilesManager>()->Open(_fileNameFS));

    Process(_fileNameVS, shVS.c_str(), shFS.c_str(), nullptr);
}

Technique::Technique(RenderDevice* _RenderDevice, cstring _fileNameVS, cstring _fileNameFS, cstring _fileNameGS)
	: m_RenderDevice(_RenderDevice)
{
    string shVS = ProcessInclude(GetManager<IFilesManager>()->Open(_fileNameVS));
    string shFS = ProcessInclude(GetManager<IFilesManager>()->Open(_fileNameFS));
	if (!_fileNameGS.empty())
	{
		string shGS = ProcessInclude(GetManager<IFilesManager>()->Open(_fileNameGS));
		Process(_fileNameVS, shVS.c_str(), shFS.c_str(), shGS.c_str());
		return;
	}
	

    Process(_fileNameVS, shVS.c_str(), shFS.c_str(), nullptr);
}

void Technique::Process(cstring fileName, const char* vertexShaderSrc, const char* fragmentShaderSrc, const char* geometryShaderSrc)
{
    m_Shader = m_RenderDevice->createShader(vertexShaderSrc, fragmentShaderSrc, geometryShaderSrc, nullptr, nullptr, nullptr);
    if (m_RenderDevice->getShaderLog().empty())
    {
        Log::Green("Shader[%s]: Successfull. Id [%d].", fileName.c_str(), m_Shader->m_ProgramGLObj);
    }
    else
    {
        Log::Error("Shader[%s]: Error.", fileName.c_str());
        Log::Error(m_RenderDevice->getShaderLog().c_str());
    }
}