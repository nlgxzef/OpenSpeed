///////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This file implements the class template optional that represents optional objects.
//
// An optional object is an object that contains the storage for another object and
// manages the lifetime of this contained object, if any. The contained object may be
// initialized after the optional object has been initialized, and may be destroyed before
// the optional object has been destroyed.
//
// Any instance of optional<T> at any given time either contains a value or does not
// contain a value. When an instance of optional<T> contains a value, it means that an
// object of type T, referred to as the optional object's contained value, is allocated
// within the storage of the optional object. Implementations are not permitted to use
// additional storage, such as dynamic memory, to allocate its contained value.
//
// The contained value is allocated in the optional<T> storage suitably
// aligned for the type T. When an object of type optional<T> is contextually converted to
// bool, the conversion returns true if the object contains a value; otherwise the
// conversion returns false.
//
// T shall be an object type and satisfy the requirements of Destructible.
///////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_OPTIONAL_H
#define EASTL_OPTIONAL_H

#include <OpenSpeed/Core/EASTL/EASTL/internal/config.h>
#include <OpenSpeed/Core/EASTL/EASTL/initializer_list.h>
#include <OpenSpeed/Core/EASTL/EASTL/memory.h> // eastl::addressof
#include <OpenSpeed/Core/EASTL/EASTL/internal/in_place_t.h> // eastl::in_place_t

#if EASTL_EXCEPTIONS_ENABLED
	EA_DISABLE_ALL_VC_WARNINGS()
	#include <stdexcept> // std::logic_error.
	EA_RESTORE_ALL_VC_WARNINGS()
#endif

#if defined(EASTL_OPTIONAL_ENABLED) && EASTL_OPTIONAL_ENABLED

EA_DISABLE_VC_WARNING(4582 4583) // constructor/destructor is not implicitly called

namespace eastl
{
	#if EASTL_EXCEPTIONS_ENABLED
		#define EASTL_OPTIONAL_NOEXCEPT
	#else
		#define EASTL_OPTIONAL_NOEXCEPT EA_NOEXCEPT
	#endif

	///////////////////////////////////////////////////////////////////////////////
	/// nullopt_t
	///
	/// nullopt_t is class type used to indicate eastl::optional type with uninitialized state.
	///
	struct nullopt_tag_t {};

	struct nullopt_t
	{
		EA_CONSTEXPR nullopt_t(nullopt_tag_t) {}
	};

	EA_CONSTEXPR nullopt_t nullopt{nullopt_tag_t{}};


    ///////////////////////////////////////////////////////////////////////////////
	/// bad_optional_access
	///
	#if EASTL_EXCEPTIONS_ENABLED
		struct bad_optional_access : public std::logic_error
		{
			bad_optional_access() : std::logic_error("eastl::bad_optional_access exception") {}
			virtual ~bad_optional_access() EA_NOEXCEPT {}
		};
	#endif

	namespace Internal
	{
		///////////////////////////////////////////////////////////////////////////////
		/// optional_storage
		///
		template<typename T, bool IsTriviallyDestructible = eastl::is_trivially_destructible_v<T>>
		struct optional_storage
		{
			typedef typename eastl::remove_const<T>::type value_type;

			optional_storage() EA_NOEXCEPT = default;

			inline optional_storage(const value_type& v)
				: engaged(true)
			{
				::new (eastl::addressof(val)) value_type(v);
			}

			inline optional_storage(value_type&& v)
				: engaged(true)
			{
				::new (eastl::addressof(val)) value_type(eastl::move(v));
			}

			inline ~optional_storage()
			{
				if (engaged)
					destruct_value();
			}

			template <class... Args>
			inline explicit optional_storage(in_place_t, Args&&... args)
			    : engaged(true)
			{
				::new (eastl::addressof(val)) T{eastl::forward<Args>(args)...};
			}

			template <typename U,
			          typename... Args,
			          typename = eastl::enable_if_t<eastl::is_constructible_v<T, std::initializer_list<U>&, Args&&...>>>
			inline explicit optional_storage(in_place_t, std::initializer_list<U> ilist, Args&&... args)
			    : engaged(true)
			{
				::new (eastl::addressof(val)) value_type{ilist, eastl::forward<Args>(args)...};
			}

			inline void destruct_value() { (*(value_type*)eastl::addressof(val)).~value_type(); }


			eastl::aligned_storage_t<sizeof(value_type), eastl::alignment_of_v<value_type>> val;
			bool engaged = false;
		};


		/// optional_storage<T, true>
		///
		/// Template specialization for trivial types to satisfy the requirement that optional<T> is trivially
		/// destructible when T is trivially destructible.
		///
		template<typename T>
		struct optional_storage<T, true>
		{
			typedef eastl::remove_const_t<T> value_type;

			optional_storage() EA_NOEXCEPT = default;

			inline optional_storage(const value_type& v)
				: engaged(true)
			{
				::new (eastl::addressof(val)) value_type(v);
			}

			inline optional_storage(value_type&& v)
				: engaged(true)
			{
				::new (eastl::addressof(val)) value_type(eastl::move(v));
			}

			// Removed to make optional<T> trivially destructible when T is trivially destructible.
			//
			// inline ~optional_storage()
			// {
			//     if (engaged)
			//         destruct_value();
			// }
			~optional_storage() EA_NOEXCEPT = default;

			template <class... Args>
			inline explicit optional_storage(in_place_t, Args&&... args)
			    : engaged(true)
			{
				::new (eastl::addressof(val)) value_type{eastl::forward<Args>(args)...};
			}

			template <typename U,
			          typename... Args,
			          typename = eastl::enable_if_t<eastl::is_constructible_v<T, std::initializer_list<U>&, Args&&...>>>
			inline explicit optional_storage(in_place_t, std::initializer_list<U> ilist, Args&&... args)
			    : engaged(true)
			{
				::new (eastl::addressof(val)) value_type{ilist, eastl::forward<Args>(args)...};
			}

			inline void destruct_value() {}  // no implementation necessary since T is trivially destructible.


			eastl::aligned_storage_t<sizeof(value_type), eastl::alignment_of_v<value_type>> val;
			bool engaged = false;
		};
	} // namespace Internal


	///////////////////////////////////////////////////////////////////////////////
	/// optional
	///
    template <typename T>
    class optional : private Internal::optional_storage<T>
    {
		typedef Internal::optional_storage<T> base_type;

		using base_type::destruct_value;
		using base_type::engaged;
		using base_type::val;

	public:
		typedef T value_type;

	    // (ISOCPP 20.6.3) A program that necessitates the instantiation of template optional for a reference type, or
	    // for possibly cv-qualified types in_place_t or nullopt_t is ill-formed.
	    static_assert(!eastl::is_reference<value_type>::value, "eastl::optional of a reference type is ill-formed");
		static_assert(!eastl::is_same<value_type, in_place_t>::value, "eastl::optional of a in_place_t type is ill-formed");
		static_assert(!eastl::is_same<value_type, nullopt_t>::value, "eastl::optional of a nullopt_t type is ill-formed");

	    inline EA_CONSTEXPR optional() EA_NOEXCEPT {}
	    inline EA_CONSTEXPR optional(nullopt_t) EA_NOEXCEPT {}
	    inline EA_CONSTEXPR optional(const value_type& value) : base_type(value) {}
		inline EA_CONSTEXPR optional(value_type&& value) EA_NOEXCEPT_IF(eastl::is_nothrow_move_constructible_v<T>)
		    : base_type(eastl::move(value))
		{
		}

		optional(const optional& other)
		{
			engaged = other.engaged;

			if (engaged)
			{
				auto* pOtherValue = reinterpret_cast<const T*>(eastl::addressof(other.val));
				::new (eastl::addressof(val)) value_type(*pOtherValue);
			}
		}

		optional(optional&& other)
		{
			engaged = other.engaged;

			if (engaged)
			{
				auto* pOtherValue = reinterpret_cast<T*>(eastl::addressof(other.val));
				::new (eastl::addressof(val)) value_type(eastl::move(*pOtherValue));
			}
		}

		template <typename... Args>
		inline EA_CONSTEXPR explicit optional(in_place_t, Args&&... args)
		    : base_type(in_place, eastl::forward<Args>(args)...)
		{
		}

		template <typename U,
		          typename... Args,
		          typename = eastl::enable_if_t<eastl::is_constructible_v<T, std::initializer_list<U>&, Args&&...>>>
		inline explicit optional(in_place_t, std::initializer_list<U> ilist, Args&&... args)
		    : base_type(in_place, ilist, eastl::forward<Args>(args)...)
		{
		}

		template <typename U = value_type,
		          typename = eastl::enable_if_t<eastl::is_constructible_v<T, U&&> &&
		                                        !eastl::is_same_v<eastl::remove_cvref_t<U>, eastl::in_place_t> &&
		                                        !eastl::is_same_v<eastl::remove_cvref_t<U>, optional>>>
		inline explicit EA_CONSTEXPR optional(U&& value)
		    : base_type(in_place, eastl::forward<U>(value))
		{
		}

		inline optional& operator=(nullopt_t)
	    {
		    reset();
		    return *this;
	    }

	    inline optional& operator=(const optional& other)
		{
			auto* pOtherValue = reinterpret_cast<const T*>(eastl::addressof(other.val));
			if (engaged == other.engaged)
			{
				if (engaged)
					*get_value_address() = *pOtherValue;
			}
			else
			{
				if (engaged)
				{
					destruct_value();
					engaged = false;
				}
				else
				{
					construct_value(*pOtherValue);
					engaged = true;
				}
			}
			return *this;
		}

	    inline optional& operator=(optional&& other)
	        EA_NOEXCEPT_IF(EA_NOEXCEPT(eastl::is_nothrow_move_assignable<value_type>::value &&
	                                       eastl::is_nothrow_move_constructible<value_type>::value))
	    {
			auto* pOtherValue = reinterpret_cast<T*>(eastl::addressof(other.val));
			if (engaged == other.engaged)
			{
				if (engaged)
					*get_value_address() = eastl::move(*pOtherValue);
			}
			else
			{
				if (engaged)
				{
					destruct_value();
					engaged = false;
				}
				else
				{
					construct_value(eastl::move(*pOtherValue));
					engaged = true;
				}
			}
		    return *this;
	    }

	    template <class U, typename = typename eastl::enable_if<eastl::is_same<eastl::decay_t<U>, T>::value>::type>
	    inline optional& operator=(U&& u)
	    {
			if(engaged)
			{
				*get_value_address() = eastl::forward<U>(u);
			}
			else
			{
				engaged = true;
				construct_value(eastl::forward<U>(u));
			}

		    return *this;
	    }

	    EA_CONSTEXPR inline explicit operator bool() const { return engaged; }

		EA_CONSTEXPR inline bool has_value() const EA_NOEXCEPT { return engaged; }

	    template <class U>
	    inline value_type value_or(U&& default_value) const
			{ return engaged ? *get_value_address() : static_cast<value_type>(eastl::forward<U>(default_value)); }

	    template <class U>
	    inline value_type value_or(U&& default_value)
			{ return engaged ? *get_value_address() : static_cast<value_type>(eastl::forward<U>(default_value)); }

		inline T& value()&                    { return get_value_ref(); }
		inline const T& value() const&        { return get_value_ref(); }
		inline T&& value()&&                  { return get_rvalue_ref(); }
		inline const T&& value() const&&      { return get_rvalue_ref(); }

	    inline T* operator->()                { return get_value_address(); }
	    inline const T* operator->() const    { return get_value_address(); }
	    inline T& operator*()&                { return get_value_ref(); }
		inline T&& operator*()&&              { return get_rvalue_ref(); }
	    inline const T& operator*() const&    { return get_value_ref(); }
		inline const T&& operator*() const&&  { return get_rvalue_ref(); }

		template <class... Args>
		void emplace(Args&&... args)
		{
			if (engaged)
			{
				destruct_value();
				engaged = false;
			}
			construct_value(eastl::forward<Args>(args)...);
			engaged = true;
		}

		template <class U, class... Args>
		void emplace(std::initializer_list<U> ilist, Args&&... args)
		{
			if (engaged)
			{
				destruct_value();
				engaged = false;
			}
			construct_value(ilist, eastl::forward<Args>(args)...);
			engaged = true;
		}

	    inline void swap(optional& other)
	        EA_NOEXCEPT_IF(eastl::is_nothrow_move_constructible<T>::value&& eastl::is_nothrow_swappable<T>::value)
	    {
		    using eastl::swap;
		    if (engaged == other.engaged)
		    {
			    if (engaged)
					swap(**this, *other);
		    }
		    else
		    {
			    if (engaged)
			    {
					other.construct_value(eastl::move(*(value_type*)eastl::addressof(val)));
					destruct_value();
			    }
			    else
			    {
					construct_value(eastl::move(*((value_type*)eastl::addressof(other.val))));
				    other.destruct_value();
			    }

			    swap(engaged, other.engaged);
		    }
	    }

		inline void reset()
		{
			if (engaged)
			{
				destruct_value();
				engaged = false;
			}
		}

	private:

		template <class... Args>
		inline void construct_value(Args&&... args)
		{ ::new (eastl::addressof(val)) value_type(eastl::forward<Args>(args)...); }

	    inline T* get_value_address() EASTL_OPTIONAL_NOEXCEPT
	    {
            #if EASTL_EXCEPTIONS_ENABLED
				if(!engaged)
					throw bad_optional_access();
			#elif EASTL_ASSERT_ENABLED
				EASTL_ASSERT_MSG(engaged, "no value to retrieve");
			#endif
			return reinterpret_cast<T*>(eastl::addressof(val));
	    }

	    inline const T* get_value_address() const EASTL_OPTIONAL_NOEXCEPT
	    {
            #if EASTL_EXCEPTIONS_ENABLED
				if(!engaged)
					throw bad_optional_access();
			#elif EASTL_ASSERT_ENABLED
				EASTL_ASSERT_MSG(engaged, "no value to retrieve");
			#endif
			return reinterpret_cast<const T*>(eastl::addressof(val));
	    }

	    inline value_type& get_value_ref() EASTL_OPTIONAL_NOEXCEPT
	    {
            #if EASTL_EXCEPTIONS_ENABLED
				if(!engaged)
					throw bad_optional_access();
			#elif EASTL_ASSERT_ENABLED
				EASTL_ASSERT_MSG(engaged, "no value to retrieve");
			#endif
		    return *(value_type*)eastl::addressof(val);
	    }

	    inline const value_type& get_value_ref() const EASTL_OPTIONAL_NOEXCEPT
	    {
            #if EASTL_EXCEPTIONS_ENABLED
				if(!engaged)
					throw bad_optional_access();
			#elif EASTL_ASSERT_ENABLED
				EASTL_ASSERT_MSG(engaged, "no value to retrieve");
			#endif
		    return *(value_type*)eastl::addressof(val);
	    }

	    inline value_type&& get_rvalue_ref() EASTL_OPTIONAL_NOEXCEPT
	    {
            #if EASTL_EXCEPTIONS_ENABLED
				if(!engaged)
					throw bad_optional_access();
			#elif EASTL_ASSERT_ENABLED
				EASTL_ASSERT_MSG(engaged, "no value to retrieve");
			#endif
		    return eastl::move(*((value_type*)eastl::addressof(val)));
	    }
    }; // class optional


    ///////////////////////////////////////////////////////////////////////////////
	/// global swap
	///
    template <class T>
    void swap(optional<T>& lhs, optional<T>& rhs) EA_NOEXCEPT_IF(EA_NOEXCEPT(lhs.swap(rhs)))
		{ lhs.swap(rhs); }


    ///////////////////////////////////////////////////////////////////////////////
	/// global comparisions
	///
	/// http://en.cppreference.com/w/cpp/utility/optional/operator_cmp
	///

    ///////////////////////////////////////////////////////////////////////////////
	// Compare two optional objects
	//
	template <class T>
	inline EA_CONSTEXPR bool operator==(const optional<T>& lhs, const optional<T>& rhs)
	{
		// NOTE:
		//
		// Code collapsed onto a single line to satisfy requirements for constexpr expressions
		// being a single line return statement.
		//
		// if(bool(lhs) != bool(rhs))
		//     return false;

		// if(bool(lhs) == false)
		//     return true;

		// return *lhs == *rhs;

		return (bool(lhs) != bool(rhs)) ? false : (bool(lhs) == false) ? true : *lhs == *rhs;
	}

	template <class T>
	inline EA_CONSTEXPR bool operator<(const optional<T>& lhs, const optional<T>& rhs)
	{
		// NOTE:
		//
		// Code collapsed onto a single line to satisify requirements for constexpr expressions
		// being a single line return statement.
		//
		// if (!bool(rhs))
		//     return false;

		// if (!bool(lhs))
		//     return true;

		//  return *lhs < *rhs;

	    return (!bool(rhs)) ? false : (!bool(lhs)) ? true : *lhs < *rhs;
    }

    template <class T>
	inline EA_CONSTEXPR bool operator!=(const optional<T>& lhs, const optional<T>& rhs)
		{ return !(lhs == rhs); }

	template <class T>
	inline EA_CONSTEXPR bool operator<=(const optional<T>& lhs, const optional<T>& rhs)
		{ return !(rhs < lhs); }

	template <class T>
	inline EA_CONSTEXPR bool operator>(const optional<T>& lhs, const optional<T>& rhs)
		{ return rhs < lhs; }

	template <class T>
	inline EA_CONSTEXPR bool operator>=(const optional<T>& lhs, const optional<T>& rhs)
		{ return !(lhs < rhs);	}


    ///////////////////////////////////////////////////////////////////////////////
	// Compare an optional object with a nullopt
	//
    template <class T>
    inline EA_CONSTEXPR bool operator==(const optional<T>& opt, eastl::nullopt_t) EA_NOEXCEPT
		{ return !opt; }

    template <class T>
    inline EA_CONSTEXPR bool operator==(eastl::nullopt_t, const optional<T>& opt) EA_NOEXCEPT
		{ return !opt; }

    template <class T>
    inline EA_CONSTEXPR bool operator!=(const optional<T>& opt, eastl::nullopt_t) EA_NOEXCEPT
		{ return bool(opt); }

    template <class T>
    inline EA_CONSTEXPR bool operator!=(eastl::nullopt_t, const optional<T>& opt) EA_NOEXCEPT
		{ return bool(opt); }

    template <class T>
    inline EA_CONSTEXPR bool operator<(const optional<T>&, eastl::nullopt_t) EA_NOEXCEPT
		{ return false; }

    template <class T>
    inline EA_CONSTEXPR bool operator<(eastl::nullopt_t, const optional<T>& opt) EA_NOEXCEPT
		{ return bool(opt); }

    template <class T>
    inline EA_CONSTEXPR bool operator<=(const optional<T>& opt, eastl::nullopt_t) EA_NOEXCEPT
		{ return !opt; }

    template <class T>
    inline EA_CONSTEXPR bool operator<=(eastl::nullopt_t, const optional<T>&) EA_NOEXCEPT
		{ return true; }

    template <class T>
    inline EA_CONSTEXPR bool operator>(const optional<T>& opt, eastl::nullopt_t) EA_NOEXCEPT
		{ return bool(opt); }

    template <class T>
    inline EA_CONSTEXPR bool operator>(eastl::nullopt_t, const optional<T>&) EA_NOEXCEPT
		{ return false; }

    template <class T>
    inline EA_CONSTEXPR bool operator>=(const optional<T>&, eastl::nullopt_t) EA_NOEXCEPT
		{ return true; }

    template <class T>
    inline EA_CONSTEXPR bool operator>=(eastl::nullopt_t, const optional<T>& opt) EA_NOEXCEPT
		{ return !opt; }


    ///////////////////////////////////////////////////////////////////////////////
    // Compare an optional object with a T
	//
    template <class T>
    inline EA_CONSTEXPR bool operator==(const optional<T>& opt, const T& value)
		{ return bool(opt) ? *opt == value : false; }

    template <class T>
    inline EA_CONSTEXPR bool operator==(const T& value, const optional<T>& opt)
		{ return bool(opt) ? value == *opt : false; }

    template <class T>
    inline EA_CONSTEXPR bool operator!=(const optional<T>& opt, const T& value)
		{ return bool(opt) ? !(*opt == value) : true; }

    template <class T>
    inline EA_CONSTEXPR bool operator!=(const T& value, const optional<T>& opt)
		{ return bool(opt) ? !(value == *opt) : true; }

    template <class T>
    inline EA_CONSTEXPR bool operator<(const optional<T>& opt, const T& value)
		{ return bool(opt) ? *opt < value  : true; }

    template <class T>
    inline EA_CONSTEXPR bool operator<(const T& value, const optional<T>& opt)
		{ return bool(opt) ? value < *opt  : false;  }

    template <class T>
    inline EA_CONSTEXPR bool operator<=(const optional<T>& opt, const T& value)
		{ return !(opt > value); }

    template <class T>
    inline EA_CONSTEXPR bool operator<=(const T& value, const optional<T>& opt)
		{ return !(value > opt); }

    template <class T>
    inline EA_CONSTEXPR bool operator>(const optional<T>& opt, const T& value)
		{ return bool(opt) ? value < *opt  : false; }

    template <class T>
    inline EA_CONSTEXPR bool operator>(const T& value, const optional<T>& opt)
		{ return bool(opt) ? *opt < value  : true; }

    template <class T>
    inline EA_CONSTEXPR bool operator>=(const optional<T>& opt, const T& value)
		{ return !(opt < value); }

    template <class T>
    inline EA_CONSTEXPR bool operator>=(const T& value, const optional<T>& opt)
		{ return !(value < opt);  }


    ///////////////////////////////////////////////////////////////////////////////
	/// hash
	///
	template <typename T>
	struct hash<eastl::optional<T>>
	{
		typedef eastl::optional<T> argument_type;
		typedef size_t result_type;

	    result_type operator()(const argument_type& opt) const EA_NOEXCEPT
	    {
		    if (opt)
			    return eastl::hash<T>()(*opt);
		    else
			    return 0;  // no value to generate a hash from
	    }
    };


    ///////////////////////////////////////////////////////////////////////////////
	/// make_optional
	///
	template <class T>
	inline EA_CONSTEXPR optional<decay_t<T>> make_optional(T&& value)
	{
		return optional<decay_t<T>>(eastl::forward<T>(value));
	}

	template <class T, class... Args>
	inline EA_CONSTEXPR optional<T> make_optional(Args&&... args)
	{
		return optional<T>(eastl::in_place, eastl::forward<Args>(args)...);
	}

	template <class T, class U, class... Args>
	inline EA_CONSTEXPR optional<T> make_optional(std::initializer_list<U> il, Args&&... args)
	{
		return eastl::optional<T>(eastl::in_place, il, eastl::forward<Args>(args)...);
	}


    #undef EASTL_OPTIONAL_NOEXCEPT

}  // namespace eastl

EA_RESTORE_VC_WARNING()

#endif  // EASTL_OPTIONAL_ENABLED
#endif  // EASTL_OPTIONAL_H
