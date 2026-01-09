#include <array>
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <random>
#include <sstream>
#include <thread>
#include <vector>

template <typename T>
class user_stack 
{
    std::size_t m_size{};
    T *m_sptr{};
    std::ptrdiff_t m_counter{-1};
public:
    user_stack(std::size_t size) : m_size{size}, m_sptr{ new T[m_size]{} }
    {  
    }
   
    user_stack(const user_stack&)= delete;
    user_stack& operator=(const user_stack&) = delete;

    bool is_empty() const 
    {
        if(m_counter == -1)
            return true;
        else 
            return false;
    }

    bool is_full() const 
    {
        if(m_counter >= static_cast<std::ptrdiff_t>(m_size-1))
            return true;
        else 
            return false;
    }

    bool push(T data)
    {
        if(!is_full())
        {
            m_sptr[++m_counter] = data;
            return true;
        }
        return false;
    }

    bool pop(T& out)
    {
        if(!is_empty())
        {
            out = m_sptr[m_counter--];
            return true;
        }

        return false;
    }

    bool peek(T& out) const
    {
        if(!is_empty())
        {
            out = m_sptr[m_counter];
            return true;
        }

        return false;
    }

    ~user_stack()
    {
        delete[] m_sptr;
    }
};

void validate_parentheses(const char* str)
{
    std::map<char, char> cmap{};
    cmap[')'] = '(';
    cmap['}'] = '{';
    cmap[']'] = '[';

    user_stack<char> us(std::strlen(str));
    const char* push_group {"({["};
    const char* pop_group {"]})"};

    const char *it{str};
    bool unbalance{false};

    for(; *it; it++)
    {
        if(std::strchr(push_group, *it))
            us.push(*it);
        else if(std::strchr(pop_group, *it))
        {
            char out{};
            if(us.peek(out))
            {
                auto val {cmap.find(*it)};
                if(val != cmap.end())
                {
                    if(out == val->second)
                        us.pop(out);
                    else
                    {
                        unbalance = true;
                        break;
                    }
                }
            }
            else 
            {
                unbalance = true;
                break;
            }
        }
    }

    if(us.is_empty() && !unbalance)
        std::cout << "valid parentheses...!\n";
    else 
        std::cout << "NOT Valid parentheses\n";
}

template <typename T, std::size_t N>
void next_greater_element(const std::array<T, N>& arr, std::array<T, N>& result)
{
    user_stack<std::ptrdiff_t> us(N);
    std::ptrdiff_t idx{};

    for(auto i{0Z}; i < std::ssize(arr); i++)
    {
        while(!us.is_empty())
        {
            us.peek(idx);
            if(arr[i] > arr[idx])
            {
                us.pop(idx);
                result[idx] = arr[i];
            }
            else 
                break;
        }

        us.push(i);
    }

    while(!us.is_empty())
    {
        us.pop(idx);
        result[idx] = -1;
    }
}

template <typename T, std::size_t N>
class min_stack 
{
    user_stack<T> m_main{N};
    user_stack<T> m_min{N};
public:
    min_stack() = default;

    void min_stack_push();

    bool push(T &in)
    {
        if(m_min.is_empty())
            m_min.push(in);
        else 
        {
            T out{};
            m_min.peek(out);
            if(in < out)
                m_min.push(in);
        }
        
        if(!m_main.is_full())
        {
            m_main.push(in);
            return true;        
        }
        else 
            return false;
    }

    bool pop(T& out)
    {
       if(!m_main.is_empty())
       {
           T out{}, min_out{};
           m_main.pop(out);

           m_min.peek(min_out);
           if(min_out == out)
               m_min.pop(min_out);
        
           return true;
       }
       else 
           return false;
    }

    bool top(T& out)
    {
        if(m_main.is_empty())
            return false;
        else 
            return m_main.peek(out);
    }

    bool getMin(T& out)
    {
        if(m_min.is_empty())
            return false;
        else 
            return m_min.peek(out);
    }
};

template <typename T, std::size_t N>
void print_array(const std::array<T, N>& arr)
{
    std::stringstream ss{};
    ss << "[ ";

    for(auto i{0Z}; i < std::ssize(arr); i++)
    {
        if((i+1) != std::ssize(arr))
            ss << arr[i] << ", ";
        else 
            ss << arr[i] << " ]";
    }

    std::cout << ss.str() << "\n";
}

template <typename T, std::size_t N>
std::array<T, N> get_arry()
{
    std::array<T, N> arr{};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

    for(auto i{0Z}; i < std::ssize(arr); i++)
    {
        arr[i] = dist(gen);
    }

    return arr;
}

int main(int argc, char* argv[])
{
    if(argc == 2)
        validate_parentheses(argv[1]);

    std::array<int, 10> arr{ get_arry<int, 10>() }, result{};
    
    std::cout << "Before NGE...\n";
    print_array(arr);

    next_greater_element(arr, result);

    std::cout << "After NGE...\n";
    print_array(result);

    min_stack<int, 10> ms{};

    for(auto i{0Z}; ms.push(arr[i]); i++);


    // below logic has error, not working properly, check it.
    while(true)
    {
        int pop_out{}, min_out{};
        bool pop_flag{}, min_flag{};

        pop_flag = ms.pop(pop_out);
        min_flag = ms.getMin(min_out);

        if(!pop_flag && !min_flag)
            break;
        else 
        {
            if(pop_out == min_out)
                std::cout << pop_out << " - " << min_out << "\n";
            else 
                std::cout << pop_out << " - \n";
        }

        std::cout << "pop_flag : " << pop_flag << "\n";
        std::cout << "min_flag : " << min_flag << "\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
