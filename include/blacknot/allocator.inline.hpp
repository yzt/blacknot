//======================================================================
// This is part of Project Blacknot:
//  https://github.com/yzt/blacknot
//======================================================================
#pragma once
//======================================================================

#include <blacknot/macros.hpp>

#include <new>
#include <utility>

//======================================================================

namespace Blacknot {

//======================================================================
// Allocator Implementation:
//======================================================================
//----------------------------------------------------------------------

template <typename T, typename... ArgTypes>
inline T * Allocator::create (AllocDebugParams const & dbg_params, ArgTypes &&... args)
{
	T * p = static_cast<T *>(alloc (sizeof(T), dbg_params));

	if (BKNT_PTR_VALID(p))
	{
		try {
			BKNT_PLACEMENT_NEW(p, T, std::forward<ArgTypes>(args)...);
		}
		catch (...)
		{
			free (p, sizeof(T), dbg_params);
			throw;
		}
	}

	return p;
}

//----------------------------------------------------------------------

template <typename T>
inline void Allocator::destroy (T * p, AllocDebugParams const & dbg_params)
{
	// TODO: Investigate using type_traits to check whether T is the
	//  concrete type of *p...

	if (BKNT_PTR_VALID(p))
	{
		p->T::~T();

		free (p, sizeof(T), dbg_params);
	}
}

//----------------------------------------------------------------------

template <typename T, typename... ArgTypes>
inline T * Allocator::createArray (size_t n, AllocDebugParams const & dbg_params, ArgTypes &&... args)
{
	size_t size = n * sizeof(T);
	T * p = static_cast<T *>(alloc (size, dbg_params));

	if (BKNT_PTR_VALID(p))
	{
		size_t i = 0;
		try {
			for (; i < n; ++i)
				BKNT_PLACEMENT_NEW(p + i, T, std::forward<ArgTypes>(args)...);
		}
		catch (...)
		{
			for ( ; i > 0; --i)
				(p + i - 1)->T::~T();

			free (p, size, dbg_params);
			throw;
		}
	}

	return p;
}

//----------------------------------------------------------------------

template <typename T>
inline void Allocator::destroyArray (T * p, size_t n, AllocDebugParams const & dbg_params)
{
	if (BKNT_PTR_VALID(p))
	{
		for (size_t i = n; i > 0; --i)
			(p + i - 1)->T::~T();

		free (p, n * sizeof(T), dbg_params);
	}
}

//----------------------------------------------------------------------
//======================================================================

}	// namespace Blacknot

//======================================================================
