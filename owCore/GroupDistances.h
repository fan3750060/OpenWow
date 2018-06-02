#pragma once

class 
	__declspec(uuid("125FA86D-6524-4D3D-BFD1-4EA463D5ED1B"))
	CGroupDistances : public ISettingGroup, public IUpdatable
{
public:
	CGroupDistances();
	~CGroupDistances();

	void Input(double _time, double _dTime) override {};
	void Update(double _time, double _dTime) override
	{
		highresdistance2 = highresdistance * highresdistance;
		mapdrawdistance2 = mapdrawdistance * mapdrawdistance;
		modeldrawdistance2 = modeldrawdistance * modeldrawdistance;
		doodaddrawdistance2 = doodaddrawdistance * doodaddrawdistance;
		culldistance2 = culldistance * culldistance;
	}

	float mapdrawdistance;
	float modeldrawdistance;
	float doodaddrawdistance;
	float highresdistance;
	float culldistance;
	float fogdistance;

	float mapdrawdistance2;
	float modeldrawdistance2;
	float doodaddrawdistance2;
	float highresdistance2;
	float culldistance2;

	// ISettingGroup
	void InitDefault() override;
};