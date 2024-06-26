#pragma once
#define CHECK_IS_ARRAY(x )( 0 * sizeof( ::union_cast<const ::Passed_Argument_to_NELEM_is_not_an_array*>(x) ) +  \
							0 * sizeof( ::Passed_Argument_to_NELEM_is_not_an_array::check_type((x), &(x)) ) )
#define NELEM(x)  int( (CHECK_IS_ARRAY(x )) +  (sizeof(x) / sizeof((x)[0]) )  )
#define MacroJoin( X, Y ) MacroDoJoin(X, Y)
#define MacroDoJoin( X, Y ) MacroDoJoin2(X, Y)
#define MacroDoJoin2( X, Y ) X##Y
#define CompileTimeAssert(e) typedef char MacroJoin(MacroJoin(__COMPILETIME_ASSERT, __LINE__), __)[(e)?1:-1]
template <typename To, typename From>
__forceinline
To union_cast(From fromPtr)
{
	CompileTimeAssert(sizeof(From) == sizeof(To));
	// The return value will either be partly undefined, or the input will be partly truncated!

	union
	{
		From fr;
		To to;
	} Temp;
	Temp.fr = fromPtr;
	return Temp.to;
}
// implementation details
class Passed_Argument_to_NELEM_is_not_an_array
{
public:
	class Is_pointer;  // intentionally incomplete type
	class Is_array {};
	template<typename T>
	static Is_pointer check_type(volatile const T*, volatile const T* volatile const*);
	static Is_array check_type(const volatile void*, volatile const void*);
};