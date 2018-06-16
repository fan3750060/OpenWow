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
	}

	float ADT_MCNK_Distance;
	float ADT_MDX_Distance;
	float ADT_WMO_Distance;
	float WMO_MODD_Distance;
	float ADT_MCNK_HighRes_Distance;

	float culldistance;
	float fogdistance;

	// ISettingGroup
	void InitDefault() override;

private:
	const float C_ADT_MCNK_Distance = 998.0f;
	const float C_ADT_MCNK_HighRes_Distance = 384.0f;
	const float C_ADT_MDX_Distance = 384.0f;
	const float C_ADT_WMO_Distance = ADT_MDX_Distance * 1.5f;
	const float C_WMO_MODD_Distance = 64.0f;
};