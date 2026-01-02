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
            std::cout << " 1 - Enter at begining\n";
            std::cout << " 2 - Enter at end\n";
            std::cout << " 3 - Enter at position\n";
            std::cout << " 4 - Delete at begining\n";
            std::cout << " 5 - Delete at end\n";
            std::cout << " 6 - Delete at position\n";
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
