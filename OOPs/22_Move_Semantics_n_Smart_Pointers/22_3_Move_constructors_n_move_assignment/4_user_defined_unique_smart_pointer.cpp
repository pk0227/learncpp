#include <iostream>

template <typename T>
class Auto_ptr
{
	T* m_ptr{};

public:
	Auto_ptr(T* ptr=nullptr) : m_ptr{ptr}
	{
	}

	Auto_ptr(const Auto_ptr&) = delete;
	Auto_ptr* operator=(const Auto_ptr&) = delete;

	Auto_ptr(Auto_ptr&&) noexcept;
	Auto_ptr& operator=(Auto_ptr&&) noexcept;

	~Auto_ptr()
	{
		delete m_ptr;
	}

	T& operator*() const {
		return *m_ptr;
	}
	T* operator->() const {
		return m_ptr;
	}
	bool isNULL() {
		return m_ptr == nullptr;
	}
};


template <typename T>
Auto_ptr<T>::Auto_ptr(Auto_ptr&& ref) noexcept : m_ptr {
	ref.m_ptr
}
{
	std::cout << "Move-Con'tor called\n";
	ref.m_ptr = nullptr;
}

template <typename T>
Auto_ptr<T>& Auto_ptr<T>::operator=(Auto_ptr&& ref) noexcept
{
	std::cout << "Move-Operator called\n";
	if(&ref != this)
	{
		delete m_ptr;
		m_ptr = ref.m_ptr;
		ref.m_ptr = nullptr;
	}
	return *this;
}

class Resource
{

public:
	Resource() {
		std::cout << "Resource acquired\n";
	}
	~Resource() {
		std::cout << "Resource destroyed\n";
	}
	void print() const {
		std::cout << "Hello\n";
	}
};

// Test 1: Simple return of local variable - eligible for implicit move
Auto_ptr<Resource> test1_SimpleReturn() {
  Auto_ptr<Resource> local{new Resource{}};
  return local; // Implicit move (or copy elision)
}

int main()
{
	Auto_ptr<Resource> res1{new Resource{}};
	res1->print();
	std::cout << "-------------------------------\n";
	//Auto_ptr<Resource> res2{res1};                                    // compilation error as copy constructor is deleted
	//Auto_ptr<Resource> res2 = res1;                                   // compilation error as copy constructor is deleted

	//Auto_ptr<Resource> res2{};
	//res2 = res1;                                                      // compilation error as copy operator is deleted

	 // In below two cases, Move constructor is expected. But, Since C++17, when you initialize an object from a prvalue, 
	 // the compiler must directly construct the object in place - this is called mandatory copy elision or guaranteed copy elision. 
	 // This is NOT an optimization that can be disabled - it's part of the C++ language specification.
	 // The move constructor is not even considered because no temporary object is created.

	//Auto_ptr<Resource> res2{Auto_ptr<Resource>{new Resource{}}};      // Move constructor is expected
	//Auto_ptr<Resource> res2 = Auto_ptr<Resource>{new Resource{}};      // Move constructor is expected

    //Auto_ptr<Resource> res2{test1_SimpleReturn()};      // Move constructor is invoked if copy elision is disable using -fno-elide-constructors.
    Auto_ptr<Resource> res2 = test1_SimpleReturn();       // Move constructor is invoked if copy elision is disable using -fno-elide-constructors.
	
    //Auto_ptr<Resource> res2{};                    
	//res2 = Auto_ptr<Resource>{new Resource{}};                        // Move operator is invoked

	return 0;
}