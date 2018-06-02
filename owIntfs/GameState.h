#pragma once

__interface IGameState
{
	bool Init();
	void Destroy();
	bool Set();
	void Unset();
	void SetInited(bool _value);
	bool IsInited() const;
	void SetCurrent(bool _value);
	bool IsCurrent() const;
};