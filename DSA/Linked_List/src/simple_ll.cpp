#include <ios>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <string_view>

struct Node 
{
    int val{};
    struct Node *next{};
};

using ll_node = struct Node;

ll_node* insert_at_begining(ll_node* head, ll_node* temp)
{
    if(head)
    {
        temp->next = head;
        head = temp;
        return head;
    }
    
    return temp;
}

ll_node* insert_at_end(ll_node* head, ll_node* temp)
{
    if(head)
    {
        ll_node* it {head};
        for( ;it->next; it = it->next);
        it->next = temp;
        return head;
    }

    return temp;
}

ll_node* insert_at_position(ll_node* head, ll_node* temp, int position)
{
    if(head == nullptr)
    {
        std::cout << "linked_list doesn't exist\n";
        return head;
    }

    ll_node* it{head}, *last_node{nullptr};
    int i{};

    for(i; (i < position) && (it); i++)
    {
        if(it)
            last_node = it;

        it = it->next;
    }

    if( (it == nullptr) && (i != position) )
    {
        std::cout << "\n***** Invalid position *****\n";
        return head;
    }

    if(last_node && it)
    {
        temp->next = it;
        last_node->next = temp;
    }
    else if( last_node && (it == nullptr) )
    {
        last_node->next = temp;
    }
    else 
    {
        temp->next = head;
        head = temp;
    }

    return head;
}
/*
{
    ll_node* it {nullptr}, *last_node{nullptr};
    bool it_started {false};
    for(int i{}; i < position; i++)
    {
        if(!it)
            it = head;
        else
        {
            if(!it_started)
                it_started = true;

            last_node = it;
            it = it->next; 
        }
            
    }

    if(!it)
    {
        if(!it_started)
        {
            temp->next = head;
            head = temp;
        }
        else 
        {
            last_node->next = temp; 
        }
    }
    else 
    {
        temp->next = it->next;
        it->next = temp;
    }

    return head;
}
*/
ll_node* load_linked_list(int num = 3)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

    ll_node* head{nullptr};

    for(int i{}; i < num; i++)
    {
        int val { dist(gen) };
        ll_node* temp { new ll_node{val, nullptr} };
        //head = insert_at_begining(head, temp);
        head = insert_at_end(head, temp);
        std::cout << val << " ";
    }
    std::cout << "\n";

    return head;
}

void print_linked_list(ll_node* head)
{
    while (head) 
    {
        if(head->next)
            std::cout << head->val << " -> ";
        else
            std::cout << head->val << "\n";

        head = head->next;
    }
}

void free_linked_list(ll_node* head)
{
    while(head)
    {
        ll_node* tmp { head };
        head = head->next;
        delete tmp;
    }
}

ll_node* delete_at_begining(ll_node* head)
{
    if(head == nullptr)
    {
        std::cout << "nothing to delete\n";
        return head;
    }

        ll_node* tmp = head;
        head = head->next;
        delete tmp;
    return head;
}

ll_node* delete_at_end(ll_node* head)
{
    if(head == nullptr)
    {
        std::cout << "nothing to delete\n";
        return head;
    }

        ll_node* it{head}, *last_node{nullptr};
        while(it)
        {
            if(it->next)
                last_node = it;
            it = it->next;
        }
        
        if(last_node && last_node->next)
        {
            delete last_node->next;
            last_node->next = nullptr;
        }
        else 
        {
            delete head;
            head = nullptr;
        }
            
    return head;
}

ll_node* delete_at_position(ll_node* head, int position)
{
    if(head == nullptr)
    {
        std::cout << "nothing to delete\n";
        return head;
    }

    ll_node* it{head}, *last_node{nullptr};
    int i{};

    for(i; (i < position) && (it); i++)
    {
        if(it)
            last_node = it;

        it = it->next;
    }

    if( (it == nullptr) && (i >= position) )
    {
        std::cout << "\n***** Invalid position *****\n";
        return head;
    }

    if(last_node && it)
    {
        last_node->next = it->next;
        delete it;
    }
    else 
    {
        ll_node* temp = head;
        head = head->next;

        delete temp; 
    }

    return head;
}
/*
{
    if(head == nullptr)
    {
        std::cout << "nothing to delete\n";
        return head;
    }

    ll_node* it{head}, *last_node{nullptr};
    for(int i{}; (i < position) && it; i++)
    {
            if(it->next)
                last_node = it;

            it = it->next;

            if(it == nullptr)
            {
                std::cout << "invalid position to delete\n";
                return head;
            }
    }

    if(last_node && it->next)
    {
        last_node->next = it->next;
        delete it;
    }
    else if(last_node && (it->next == nullptr))
    {
        delete last_node->next;
        last_node->next = nullptr;
    }
    else 
    {
        ll_node* tmp {head};
        head = head->next;
        delete tmp;
    }

    return head;
}
*/

ll_node* get_middle_node(ll_node* head)
{
    if(head == nullptr)
    {
        std::cout << "***** head is empty *****\n";
        return head;
    }
    else if( head->next == nullptr )
        return head;

    ll_node* fast{head}, *slow{nullptr};

    while(fast && fast->next)
    {
        if(slow)
            slow = slow->next;
        else 
            slow = head;
        fast = fast->next->next;
    }

    if(fast == nullptr)
        return slow;

    if(fast->next == nullptr)
        return slow->next;
/*
    ll_node* fast{head}, *slow{head};

    while(fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
*/

    return head;
}

void print_reverse(ll_node* head)
{
    if(head == nullptr)
    {
        std::cout << "***** head is empty *****\n";
    }

    if(head)
        print_reverse(head->next);
    else 
        return;

    std::cout << head->val << " ";
}

ll_node* make_loop_at_position(ll_node* head, int position)
{
    if(head == nullptr)
    {
        std::cout << "***** head is empty *****\n";
        return head;
    }
    
    ll_node* temp{nullptr}, *it{head}, *last_node{nullptr};
    
    for(int i{}; it; i++)
    {
        if(i == position)
            temp = it;
        
        last_node = it;
        it = it->next;
    }

    if(temp == nullptr)
        return temp;
    else 
    {
        last_node->next = temp;
    }
        return head;
}

ll_node* check_loop_exists(ll_node* head)
{
    if(head == nullptr)
    {
        std::cout << "***** head is empty *****\n";
        return head;
    }
    
    ll_node* fast{head}, *slow{head};

    while(fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if(fast == slow)
            break;
    }

    return fast;
}

int loop_starts_at_position(ll_node* head)
{
    if(head == nullptr)
    {
        std::cout << "***** head is empty *****\n";
        return -1;
    }

    if(head->next == head)
        return 0;
    else if(head->next == nullptr)
        return -1;

    ll_node* fast{head}, *slow{head};
    int position{-1};

    while(fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if(fast == slow)
            break;
    }

    if(fast == slow)
    {
        slow = head;
        position = 0;

        while(slow != fast)
        {
            slow = slow->next;
            fast = fast->next;
            
            position++;
        }
    }

    return position;
}

ll_node* loop_starts_at_node(ll_node* head)
{
    if(head == nullptr)
    {
        std::cout << "***** head is empty *****\n";
        return head;
    }

    if(head->next == nullptr)
        return head->next;

    ll_node* fast{head}, *slow{head};

    while(fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if(fast == slow)
            break;
    }

    if(fast ==slow)
    {
        slow = head;

        while(slow != fast)
        {
            slow = slow->next;
            fast = fast->next;
        }

        return slow;
    }

    return nullptr;
}

ll_node* remove_loop(ll_node* head)
{
    if(head == nullptr)
    {
        std::cout << "***** head is empty *****\n";
        return head;
    }

    if(head->next == head)
    {
        head->next = nullptr;
        return head;
    }
    else if(head->next == nullptr)
        return head->next;

    ll_node* fast{head}, *slow{head};

    while(fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if(fast == slow)
            break;
    }

    if(fast == slow)
    {
        slow = head;
        ll_node* temp{nullptr};

        while(slow != fast)
        {
            slow = slow->next;
            temp = fast;
            fast = fast->next;
        }

        temp->next = nullptr;
        return head;
    }

    return nullptr;
}

ll_node* reverse_linked_list(ll_node* head)
{
    if(head == nullptr)
    {
        std::cout << "***** head is empty *****\n";
        return head;
    }

    ll_node* it{head}, *temp{head};

    while(it)
    {
        it = it->next;
        if(it)
        {
            temp->next = it->next;
            it->next = head;

            head = it;
            it = temp;
        }
    }

    return head;
}

ll_node* get_even_ll(int length)
{
    ll_node* head {nullptr};
    int val{};
    
    for(int i{}; i < length; i++)
    {
        ll_node* temp { new ll_node{val, nullptr} };
        temp->val = val;
        head = insert_at_end(head, temp);
        val += 2;
    }

    return head;
}

ll_node* get_odd_ll(int length)
{
    ll_node* head {nullptr};
    int val{1};
    
    for(int i{}; i < length; i++)
    {
        ll_node* temp { new ll_node{val, nullptr} };
        temp->val = val;
        head = insert_at_end(head, temp);
        val += 2;
    }

    return head;
}

ll_node* merge_two_ll(ll_node* even, ll_node* odd)
{
    if(even == nullptr || odd == nullptr)
    {
        std::cout << "***** even/odd list is empty *****\n";
        return nullptr;
    }

    ll_node* merged{nullptr};

    while(even && odd)
    {
        ll_node* temp {even};
        even = even->next;
        temp->next = nullptr;
        merged = insert_at_end(merged, temp);

        temp = odd;
        odd = odd->next;
        temp->next = nullptr;
        merged = insert_at_end(merged, temp); 
    }

    if(even)
        merged = insert_at_end(merged, even);

    if(odd)
        merged = insert_at_end(merged, odd);

    return merged;
}

bool intersect_ll(ll_node* list1, ll_node* list2, int position)
{
    for(; list2->next; list2 = list2->next);

    int i{};
    for(i; (i < position) && list1; i++, list1 = list1->next);

    if(i == position)
    {
        list2->next = list1;
        return true;
    }

    return false;
}

bool check_intersection_exists(ll_node* list1, ll_node* list2)
{
    ll_node* indicator_node{nullptr};

    while(true)
    {
        if((list1) && (list1->next == nullptr) && (indicator_node == nullptr))
        {
            indicator_node = list1;
            list1 = list1->next;
        }

        if((list2) && (list2->next == nullptr) && (indicator_node == nullptr))
        {
            indicator_node = list2;
            list2 = list2->next;
        }

        if( ( list1 && (list1 == indicator_node) ) || ( list2 && (list2 == indicator_node) ) )
            return true;

        if(!list1 && !list2)
            return false;

        if(list1)
            list1 = list1->next;
        if(list2)
            list2 = list2->next;
    }
}

bool is_palindrome(unsigned int num)
{
    unsigned int new_num{}, temp{num};

    for(int i{}; temp; i++)
    {
        new_num = (new_num *10) + (temp % 10);
        temp /= 10;
    }

    if(new_num == num)
        return true;
    else 
        return false;

}

ll_node* get_palindrome_ll(unsigned long int num)
{
    ll_node* res{nullptr};

//    if(is_palindrome(num))
    {
       unsigned long int temp{num};
       while(temp)
       {
            ll_node* tmp { new ll_node{static_cast<int>(temp % 10), nullptr} };
            //res = insert_at_end(res, tmp);
            res = insert_at_begining(res, tmp);
            temp /= 10;
       }
    }
//    else
    {
//        std::cout << "given number is NOT a palindrome\n";
    }

    return res;
}

bool check_palindrome_exists(ll_node* palindrome)
{
    if(palindrome == nullptr)
    {
        std::cout << "no palindrom linked list exits\n";
        return false;
    }
/*
    ll_node* fast{palindrome}, *slow{palindrome};

    while(fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
*/  
    ll_node* middle { get_middle_node(palindrome) };
    if(middle->next == nullptr)
    {
        if(palindrome->val == middle->val)
            return true;
        else 
            return false;
    }

    ll_node* head {middle->next};
    middle->next = nullptr;
    
    head = reverse_linked_list(head);
    middle->next = head;
    print_linked_list(palindrome);

    ll_node* it1{palindrome}, *it2{middle->next};

    while( (it1 != middle) && (it2) )
    {
        if(it1->val != it2->val)
            return false;

        it1 = it1->next;
        it2 = it2->next;
    }

    if(it2)
    {
        if(it1->val != it2->val)
            return false;
    }
/*
    ll_node* head {slow->next}, *it{slow->next}, *tmp{nullptr};
    while(it)
    {
        tmp = it;
        it = it->next;
        
        if(it)
        {
            tmp->next = it->next;
            it->next = head;
            head = it;
            it = tmp; 
        }
    }

    slow->next = head;
    print_linked_list(palindrome);

    it = palindrome;
    tmp = slow->next;

    while( (it != slow) && (tmp) )
    {
        if(it->val != tmp->val)
            return false;

        it = it->next;
        tmp = tmp->next;
    }
*/  
    return true;
}

int main(int argc, char* argv[])
{
    ll_node *head{ load_linked_list(std::stoi(std::string{argv[1]})) };
    ll_node* list1{nullptr}, *list2{nullptr};
    ll_node* palindrome{nullptr};
    int list1_length{}, list2_length{};

    if(head)
        print_linked_list(head);

    {
        bool exit{false};
        bool input_failed{false};
        while(!exit)
        {
            std::cout << "=====================================================\n";
            std::cout << " 0 - Print the list\n";
            std::cout << " 1 - Enter at begining\n";
            std::cout << " 2 - Enter at end\n";
            std::cout << " 3 - Enter at position\n";
            std::cout << " 4 - Delete at begining\n";
            std::cout << " 5 - Delete at end\n";
            std::cout << " 6 - Delete at position\n";
            std::cout << " 7 - Find middle node\n";
            std::cout << " 8 - Print reverse\n";
            std::cout << " 9 - Make a loop at given index\n";
            std::cout << " 10 - Check if loop exists\n";
            std::cout << " 11 - Loop starts at position\n";
            std::cout << " 12 - Loop starts at node\n";
            std::cout << " 13 - Remove loop\n";
            std::cout << " 14 - Reverse linked list\n";
            std::cout << " 15 - Merge two linked lists\n";
            std::cout << " 16 - Create two linked lists for intersection\n";
            std::cout << " 17 - Intersect two linked lists\n";
            std::cout << " 18 - Check if Intersection exists between the two linked lists\n";
            std::cout << " 19 - Generate palindrome linked list\n";
            std::cout << " 20 - Check the palindrome linked list is really palindrme or not\n";
            std::cout << " 100 - quit\n";
            std::cout << "=====================================================\n";

            int input{};
            std::cout << "Enter your choice : ";
            std::cin >> input;
            if(std::cin.fail())
            {
                input_failed = true;
                goto CLEAR;
            }

            switch(input)
            {
                case 100:
                    {
                        exit = true;
                        break;
                    }

                case 0:
                    {
                        print_linked_list(head);
                        break;
                    }

                case 1:
                    {
                        ll_node* temp { new ll_node{0, nullptr} };
                        std::cout << "Enter val : ";
                        std::cin >> temp->val;
                        if(std::cin)
                            head = insert_at_begining(head, temp);
                        else
                            input_failed = true;
                        break;
                    }

                case 2:
                    {
                        ll_node* temp { new ll_node{0, nullptr} };
                        std::cout << "Enter val : ";
                        std::cin >> temp->val;
                        if(std::cin)
                            head = insert_at_end(head, temp);
                        else
                            input_failed = true;
                        break;
                    }

                case 3:
                    {
                        int position{};
                        ll_node* temp { new ll_node{0, nullptr} };
                        std::cout << "Enter val : ";
                        std::cin >> temp->val;
                        if(std::cin)
                        {
                            std::cout << "Enter position : ";
                            std::cin >> position;
                            if(std::cin)
                            {
                                head = insert_at_position(head, temp, position);
                                print_linked_list(head);
                            }
                            else
                                input_failed = true;
                        }
                        else
                            input_failed = true;
                        break;
                    }
                
                case 4:
                    {
                        head = delete_at_begining(head);            
                        print_linked_list(head);
                        break;
                    }
                
                case 5:
                    {
                        head = delete_at_end(head);            
                        print_linked_list(head);
                        break;
                    }
                
                case 6:
                    {
                        int position{};
                        std::cout << "Enter position : ";
                        std::cin >> position;
                        if(std::cin)
                        {
                            head = delete_at_position(head, position);
                            print_linked_list(head);
                        }
                        else
                            input_failed = true;
                        break;
                    }
                
                case 7:
                    {
                        ll_node* middle_node { get_middle_node(head) };
                        if(middle_node)
                            std::cout << "middle node val : " << middle_node->val << "\n";
                        break;
                    }

                case 8:
                    {
                        print_reverse(head);
                        std::cout << "\n";
                        break;
                    }

                case 9:
                    {
                        int position{};
                        std::cout << "Enter position : ";
                        std::cin >> position;
                        if(std::cin)
                        {
                            ll_node* res {make_loop_at_position(head, position)};
                            if(res)
                                std::cout << "Done! loop has been made..!\n";
                            else
                                std::cout << "***** Invalid position *****\n";
                        }
                        else 
                        {
                            input_failed = true;
                            goto CLEAR;
                        }
                        break;
                    }
                
                case 10:
                    {
                        ll_node* res { check_loop_exists(head) };
                        if(res && res->next)
                            std::cout << "***** WARNING : loop detected *****\n";
                        else
                            std::cout << "***** NO LOOP DETECTED *****\n";
                        break;
                    }

                case 11:
                    {
                        int position { loop_starts_at_position(head) };
                        if(position == -1)
                            std::cout << "***** NO LOOP DETECTED *****\n";
                        else 
                            std::cout << "Loop is found at index " << position << "\n";

                        break;
                    }
                
                case 12:
                    {
                        ll_node* res { loop_starts_at_node(head) };
                        if(res)
                            std::cout << "loop is found at node val " << res->val << "\n";
                        else 
                            std::cout << "***** NO LOOP DETECTED *****\n";
                            
                        break;
                    }

                case 13:
                    {
                        ll_node* res { remove_loop(head) };
                        if(res)
                            std::cout << "loop has been removed.\n";
                        else
                            std::cout << "***** NO LOOP DETECTED *****\n";
                            
                        break;
                    }

                case 14:
                    {
                        head = reverse_linked_list(head); 
                        break;
                    }

                case 15:
                    {
                        ll_node* even{nullptr}, *odd{nullptr};
                        int even_length{}, odd_length{};

                        std::cout << "Enter length of even linked list : ";
                        std::cin >> even_length;

                        if(std::cin)
                        {
                            std::cout << "Enter length of odd linked list : ";
                            std::cin >> odd_length;

                            if(std::cin.fail())
                            {
                                input_failed = true;
                                goto CLEAR;
                            }
                        }
                        else 
                        {
                            input_failed = true;
                            goto CLEAR;
                        }

                        even = get_even_ll(even_length);
                        odd = get_odd_ll(odd_length);

                        std::cout << "\nEven ll : \n";
                        print_linked_list(even);

                        std::cout << "\nOdd ll : \n";
                        print_linked_list(odd);
                        
                        ll_node* merged { merge_two_ll(even, odd) };
                        std::cout << "\nMerged linked list : \n";
                        print_linked_list(merged);
                        
                        free_linked_list(merged);
                        break;
                    }

                case 16:
                    {
                        free_linked_list(list1);
                        free_linked_list(list2);

                        std::cout << "Enter list1 length : ";
                        std::cin >> list1_length;
                        std::cout << "Enter list2 length : ";
                        std::cin >> list2_length;

                        list1 = load_linked_list(list1_length);    
                        list2 = load_linked_list(list2_length);    

                        std::cout << "\nlist1 ll : \n";
                        print_linked_list(list1);

                        std::cout << "\nlist2 ll : \n";
                        print_linked_list(list2);

                        break;
                    }
                
                case 17:
                    {
                        if((list1 == nullptr) || (list2 == nullptr))
                        {
                            std::cout << "lists do not exist\n";
                        }
                        else 
                        {
                            int position{};
                            std::cout << "Enter the position of list1 at which list2 intersects : ";
                            std::cin >> position;
                        
                            while(position >= list1_length)
                            {
                                std::cout << "***** position should be less than list1 length *****\n";
                                std::cout << "Enter the position of list1 at which list2 intersects : ";
                                std::cin >> position;
                            }

                            if(intersect_ll(list1, list2, position))
                            {
                                std::cout << "\nlist2 has beed intersected to list1 at position " << position << "\n";
                                std::cout << "\nlist1 ll : \n";
                                print_linked_list(list1);

                                std::cout << "\nlist2 ll : \n";
                                print_linked_list(list2);
                            }
                            else 
                                std::cout << "\n***** intersection failed *****\n";
                        }

                        break;
                    }
                
                case 18:
                    {
                        if(check_intersection_exists(list1, list2))
                        {
                            std::cout << "INTERSECTION FOUND\n";
                        }
                        else 
                            std::cout << "***** NO INTERSECTION DETECTED *****\n";
                        
                        break;
                    }
                
                case 19:
                    {
                        unsigned long int palindrome_number{};
                        std::cout << "Enter palindrome number : ";
                        std::cin >> palindrome_number;

                        free_linked_list(palindrome);
                        palindrome = get_palindrome_ll(palindrome_number);

                        if(palindrome)
                        {
                            std::cout << "Palindrome linked list created\n";
                            print_linked_list(palindrome);
                        }
                        else 
                            std::cout << "***** Error in creating palindrome *****\n";

                        break;
                    }
                
                case 20:
                    {
                        if(check_palindrome_exists(palindrome))
                        {
                            std::cout << "Palindrome linked list is really Palindrome\n";
                        }
                        else
                            std::cout << "***** Palindrome NOT FOUND *****\n";

                        break;
                    }

                default:
                    std::cout << "Invalid choice\n";
                    break;
                    
            }
CLEAR:            
            if(input_failed)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }            
    }

    if(head)
    {
        free_linked_list(head);
        head = nullptr;
    }

    free_linked_list(list1);
    free_linked_list(list2);
    return 0;
}
