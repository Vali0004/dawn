#pragma once
#include "data/types.h"

class persist_impl
{
public:
	persist_impl(fptr<void()> cb1, fptr<void()> cb2)
		: m_constructor(cb1), m_deconstructor(cb2) 
	{
		m_constructor();
	}
	~persist_impl()
	{
		m_deconstructor();
	}
private:
	fptr<void()> m_constructor{};
	fptr<void()> m_deconstructor{};
};