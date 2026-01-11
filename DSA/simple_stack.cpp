#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <cstring>
#include <iterator>
#include <limits>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace sort 
{
    template <typename T, std::size_t N>
    void insertion_sort(std::array<T, N>& arr)
    {
        std::ptrdiff_t size { std::ssize(arr) };
        for(auto i{1Z}; i < size; i++)
        {
            auto key { arr[i] };
            std::ptrdiff_t j{i-1};

            while(j>=0 && (arr[j] >= key))
            {
                arr[j+1] = arr[j];
                j--;
            }

            arr[j+1] = key;
        }
    }

    template <typename T, std::size_t N>
    void bubble_sort(std::array<T, N>& arr)
    {
        std::ptrdiff_t size { std::ssize(arr) };

        for(auto i{0Z}; i < size-1; i++)
        {
            bool swapped{};
            for(auto j{0Z}; j < size-1-i; j++)
            {
                if(arr[j+1] < arr[j])
                {
                    std::swap(arr[j+1], arr[j]);
                    swapped = true;
                }
            }

            if(!swapped)
                break;
        }
    }

    template <typename T, std::size_t N>
    void selection_sort(std::array<T, N>& arr)
    {
        std::ptrdiff_t size { std::ssize(arr) };

        for(auto i{0Z}; i < size-1; i++)
        {
            auto min_index{i};
            for(auto j{i+1}; j < size; j++)
            {
                if(arr[j] < arr[min_index])
                    min_index = j;
            }
            std::swap(arr[i], arr[min_index]);
        }
    }

    template <typename T, std::size_t N>
    void merge(std::array<T, N>& arr, std::ptrdiff_t low, std::ptrdiff_t mid, std::ptrdiff_t high)
    {
        std::ptrdiff_t left {low};
        std::ptrdiff_t right {mid+1};
       
        T temp[high-low+1];
        std::ptrdiff_t index{};

        while((left <= mid) && (right <= high))
        {
            if(arr[left] <= arr[right])
                temp[index++] = arr[left++];
            else 
                temp[index++] = arr[right++];
        }

        while(left <= mid)
            temp[index++] = arr[left++];

        while(right <= high)
            temp[index++] = arr[right++];
        
        for(index = low; index <= high; index++)
            arr[index] = temp[index-low];
    }

    template <typename T, std::size_t N>
    void merge_sort(std::array<T, N>& arr, std::ptrdiff_t low, std::ptrdiff_t high)
    {
        if(low < high)
        {
            std::ptrdiff_t mid { (low+high)/2 };
            merge_sort(arr, low, mid);
            merge_sort(arr, mid+1, high);
            merge(arr, low, mid, high);
        }
    }

    template <typename T, std::size_t N>
    std::ptrdiff_t partition(std::array<T, N>& arr, std::ptrdiff_t low, std::ptrdiff_t high)
    {
        T pivot { arr[high] };
        std::ptrdiff_t pi { low-1 };

        for(auto i{low}; i < high; i++)     // i should start from low, not from 0
        {
            if(arr[i] < pivot)
            {
                pi++;
                std::swap(arr[i], arr[pi]);
            }
        }
        std::swap(arr[pi+1], arr[high]);
        return pi+1;
    }

    template <typename T, std::size_t N>
    void quick_sort(std::array<T, N>& arr, std::ptrdiff_t low, std::ptrdiff_t high)
    {
        if(low < high)
        {
            std::ptrdiff_t pivot { partition(arr, low, high) };
            quick_sort(arr, low, pivot-1);
            quick_sort(arr, pivot+1, high);
        }
    }
}

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
        if(!m_main.is_full() && !m_min.is_full())
        {
            if(m_min.is_empty())
                m_min.push(in);
            else 
            {
                T out{};
                m_min.peek(out);
                if(in < out)
                {
                    m_min.push(in);
                }
            }

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
           T min_out{};
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

    std::cout << std::format("{:<50}", ss.str());
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

int get_int_value_from_user(const char* str)
{
    int input{};
    while(true)
    {
        std::cout << str << " : ";
        std::cin >> input;
        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        break;
    }

    return input;
}

template <typename T, std::size_t M, std::size_t N>
auto merge_two_sorted_arrays(std::array<T, M>& arr1, std::array<T, N>& arr2)
{
    //user_stack<T> us(M);
    std::ptrdiff_t i{}, j{}, k{};

    T result[M+N]{};

    while((i < M) && (j < N))
    {
        if(arr1[i] <= arr2[j])
        {
            result[k++] = arr1[i++];
            //us.push(arr1[i++]);
        }
        else
        {
            result[k++] = arr2[j++];
            //us.push(arr2[j++]);
        }
/*
        if(!us.is_empty())
        {
            T out{};
            us.pop(out);
            result[k++] = out;
        }
*/
    }

    while(i < M)
        result[k++] = arr1[i++];

    while(j < N)
        result[k++] = arr2[j++];

    for(const auto& el : result)
        std::cout << el << " ";
    std::cout << "\n";

    return true;
}

int main(int argc, char* argv[])
{
    std::cout << "=======================================================\n";
    std::cout << "1 - Validate Parentheses\n";
    std::cout << "2 - Next Greater Element\n";
    std::cout << "3 - Min Stack\n";
    std::cout << "4 - Merge two sorted arays in result array\n";
    std::cout << "100 - Exit\n\n\n";

    bool exit{};

    while(!exit)
    {
        int input {get_int_value_from_user("Enter input")};
        std::cout << "\n";
        switch(input)
        {
            case 1:
                {
                    if(argc == 2)
                        validate_parentheses(argv[1]);
                
                    break;
                }

            case 2:
                {
                    std::array<int, 10> arr{ get_arry<int, 10>() }, result{};

                    std::cout << "Before NGE...\n";
                    print_array(arr);
                    std::cout << "\n";

                    next_greater_element(arr, result);

                    std::cout << "After NGE...\n";

                    break;
                }

            case 3:
                {
                    std::array<int, 10> arr{ get_arry<int, 10>() };
                    print_array(arr);
                    std::cout << "\n";

                    min_stack<int, 10> ms{};

                    for(auto i{0Z}; ms.push(arr[i]); i++);

                    int out{};
                    ms.getMin(out);
                    std::cout << "getMin : " << out << "\n";

                    int pop_out{}, min_out{};
                    bool pop_flag{}, min_flag{};

                    while(true)
                    {
                        min_flag = ms.getMin(min_out);
                        pop_flag = ms.pop(pop_out);

                        if(min_flag && pop_flag)
                        {
                            if(pop_out == min_out)
                                std::cout << pop_out << " - " << min_out << "\n";
                            else 
                                std::cout << pop_out << " - \n";

                        }
                        else 
                            break;
                    }

                    break;
                }

            case 4:
                {
                    /*
                    std::array<int, 10> arr{ get_arry<int, 10>() }, result{};
                    
                    result = arr;
                    std::cout << "Quick Sorted Array :\n";
                    print_array(result);
                    sort::quick_sort(result, 0, std::ssize(result)-1);
                    std::cout << "  =>  ";
                    print_array(result);
                    std::cout << "\n";

                    std::cout << "==========================================================\n";

                    result = arr;
                    std::cout << "Merge Sorted Array :\n";
                    print_array(result);
                    sort::merge_sort(result, 0, std::ssize(result)-1);
                    std::cout << "  =>  ";
                    print_array(result);
                    std::cout << "\n";

                    std::cout << "==========================================================\n";

                    result = arr;
                    std::cout << "Bubble Sorted Array :\n";
                    print_array(result);
                    sort::bubble_sort(result);
                    std::cout << "  =>  ";
                    print_array(result);
                    std::cout << "\n";

                    std::cout << "==========================================================\n";

                    result = arr;
                    std::cout << "Selection Sorted Array :\n";
                    print_array(result);
                    sort::selection_sort(result);
                    std::cout << "  =>  ";
                    print_array(result);
                    std::cout << "\n";

                    std::cout << "==========================================================\n";

                    result = arr;
                    std::cout << "Insertion Sorted Array :\n";
                    print_array(result);
                    sort::insertion_sort(result);
                    std::cout << "  =>  ";
                    print_array(result);
                    std::cout << "\n";

                    std::cout << "==========================================================\n";
                    */

                    std::array<int, 10> arr1{ get_arry<int, 10>() };
                    std::array<int, 5> arr2{ get_arry<int, 5>() };
                    
                    print_array(arr1);
                    std::cout << "  =>  ";
                    sort::insertion_sort(arr1);
                    print_array(arr1);
                    std::cout << "\n";

                    print_array(arr2);
                    std::cout << "  =>  ";
                    sort::selection_sort(arr2);
                    print_array(arr2);
                    std::cout << "\n";

                    auto result { merge_two_sorted_arrays(arr1, arr2) };

                    break;
                }

            case 100:
                {
                    exit = true;
                    break;
                }

            default:
                {
                    std::cout << "Invalid Input\n";
                    break;
                }
        }

    }

    return 0;
}
