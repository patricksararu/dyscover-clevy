// ---------------------------------------------------------------------------
#pragma hdrstop
// ---------------------------------------------------------------------------
#include "Result.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------

namespace lokiCpp
{ // opening namespace core elements

	TResult::TResult(ResultType aType, std::string aMessage)
		: Type(aType)
	{
		Type    = aType;
		Message = aMessage;
	}

	ResultType TResult:: operator()() const
	{
		return Type;
	}

	TResult &TResult:: operator = (TResult const &other)
	{
		Type    = other.Type;
		Message = other.Message;

		return *this;
	}

	void TResult:: operator = (ResultType const &other)
	{
		Type = other;
	}

	void TResult:: operator = (std::string const &other)
	{
		Message = other;
	}

	void TResult:: operator = (int const &other)
	{
		Type = static_cast < ResultType > (other);
	}

	int TResult::toInt() const
	{
		return static_cast < int > (Type);
	}

	TResult:: operator int() const
	{
		return toInt();
	}

} // end of namespace lokiCpp
