#include <iostream>
#include <string_view>

class A
{
public:
	virtual std::string_view getName1(int x) { return "A"; }
	virtual std::string_view getName2(int x) { return "A"; }
};

class B : public A
{
public:
	// virtual std::string_view getName1(short x) { return "B"; } // note: parameter is a short, 
                                                                  // not override function to Base version. So, not partcipate in run-time polymorphism.
                                                                  // To make it partcipate in run-time polymorphism, use verride keyword.
	std::string_view getName1(short x) override { return "B"; }   // now, override keyword ensure it to participate run-time polymorphism and match with base version signature.
                                                                  // error: ‘std::string_view B::getName1(short int)’ marked ‘override’, but does not override
                                                                  // Because the function is meant to be override, but isn't. 
	virtual std::string_view getName2(int x) const { return "B"; } // note: function is const
};

int main()
{
	B b{};
	A& rBase{ b };
	std::cout << rBase.getName1(1) << '\n';
	std::cout << rBase.getName2(2) << '\n';

	return 0;
}