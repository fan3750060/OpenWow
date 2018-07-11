#pragma once

struct IRefItem
{
	virtual ~IRefItem() = 0 {};

	virtual void AddRef() = 0;
	virtual void Release() = 0;
	virtual uint32 GetRefsCount() const = 0;
};