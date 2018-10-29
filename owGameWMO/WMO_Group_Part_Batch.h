#pragma once

#include "WMO_Headers.h"

#include "WMO_Part_Material.h"

// FORWARD BEGIN
class WMO;
class WMO_Group;
// FORWARD END

class WMO_Group_Part_Batch : public MeshWrapper
{
public:
	WMO_Group_Part_Batch(const std::weak_ptr<const WMO> _parentWMO, std::shared_ptr<IMesh> _mesh, const SWMO_Group_BatchDef& _proto);

	uint32 getBlendMode() const { return m_WMOMaterial->getBlendMode(); }

	void Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0) override;

private:
	const SWMO_Group_BatchDef	m_Proto;
	std::shared_ptr<const WMO_Part_Material>	m_WMOMaterial;
	BoundingBox					m_Bounds;

	// Parent
	const std::weak_ptr<const WMO>       m_ParentWMO;
	const std::weak_ptr<const WMO_Group> m_ParentGroup;
	const CGroupQuality&                 m_Quality;
};

struct WMO_Group_Part_BatchCompare
{
	bool operator() (const std::shared_ptr<WMO_Group_Part_Batch> left, const std::shared_ptr<WMO_Group_Part_Batch> right) const
	{
		return left->getBlendMode() < right->getBlendMode();
	}
};