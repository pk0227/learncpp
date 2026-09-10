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
	// std::string_view getName1(short x) override { return "B"; }   // error: ‘std::string_view B::getName1(short int)’ marked ‘override’, but does not override Base::getName1(int)
	std::string_view getName1(int x) override { return "B"; }         // Correct: matches Base::getName1(int) signature
	virtual std::string_view getName2(int x) const { return "B"; } // note: function is const, hides Base::getName2(int) rather than overriding
};

int main()
{
	B b{};
	A& rBase{ b };
	std::cout << rBase.getName1(1) << '\n';
	std::cout << rBase.getName2(2) << '\n';

	return 0;
}