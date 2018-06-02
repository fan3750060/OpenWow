#pragma once

__interface IRefItem
{
	void AddRef();
	void Release();
	uint32 GetRefsCount() const;
};