// ---------------------------------------------------------------------------
#ifndef ResultH
#define ResultH
// ---------------------------------------------------------------------------
#include <string>
// ---------------------------------------------------------------------------

namespace lokiCpp
{ // opening namespace core elements

	enum ResultType
	{
		rtUNDEFINED = -1, rtSUCCESS = 1, rtFAIL, rtEOF, rtEMPTY, rtBOF, rtLOCKED, rtABORT
	};

	class TResult
	{
		std::string Message;

		ResultType Type;

	public:
		TResult(ResultType aType = ResultType::rtUNDEFINED, std::string aMessage = "");
		// ~TResult() {}

		ResultType operator()() const ; // returns the Type as a const value.

		TResult &operator = (TResult const &other);
		void     operator = (ResultType const &other);
		void     operator = (std::string const &other);
		void     operator = (int const &other);

		bool operator == (TResult const &other) const
		{
			return (Type == other.Type); // && Message.Compare(other.Message) == 0);
		}

		bool operator == (ResultType const &other) const
		{
			return (Type == other); // && Message.Compare(other.Message) == 0);
		}

		bool operator != (TResult const &other) const
		{
			return !(*this == other);
		}

		bool operator != (ResultType const &other) const
		{
			return !(*this == other);
		}

		int toInt() const ;

		// conversion operator
		operator int() const ;

	protected:
	private:
	};

} // end of namespace lokiCpp

#endif
