#pragma once

#include <boost/uuid/uuid.hpp>

/**
 * The Object class is the base class of all objects
 * in the game.
 */

class Object
{
public:
	// Check to see if this object is the same as another.
	virtual bool operator==(const Object& rhs) const;

protected:
	// Objects should not be created or destroyed unless explicitly stated
	// by overriding these methods
	Object();

	enum ConstructorType
	{
		NoUUID,
	};
	// Generating UUIDs is expensive.
	// When creating temporary objects, use this constructor.
	Object(ConstructorType type);
	virtual ~Object();

	boost::uuids::uuid GetUUID() const;

private:
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;

	boost::uuids::uuid m_UUID;
};