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

int main(int argc, char* argv[])
{
    ll_node *head{ load_linked_list(std::stoi(std::string{argv[1]})) };

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

    return 0;
}
