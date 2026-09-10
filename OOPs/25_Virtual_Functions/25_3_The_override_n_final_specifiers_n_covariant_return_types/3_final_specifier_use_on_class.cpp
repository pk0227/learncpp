#include <string_view>

class A
{
public:
	virtual std::string_view getName() const { return "A"; }
};

class B final : public A // note use of final specifier here
{
public:
	std::string_view getName() const override { return "B"; }
};

// class C : public B // compile error: cannot inherit from final class
// {
// public:
// 	std::string_view getName() const override { return "C"; }
// };

int main()
{
	return 0;
}