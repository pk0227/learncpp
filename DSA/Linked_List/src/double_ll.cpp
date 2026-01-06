#include <ctime>
#include <ios>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <string_view>

struct Node 
{
    int value{};
    Node *prev{}, *next{};
};

Node* add_at_begin(Node *head, Node *tmp)
{
    if(head == nullptr)
        return tmp;

    tmp->next = head;
    head->prev = tmp;
    head = tmp;

    return head;
}

Node* add_at_end(Node *head, Node *tmp)
{
    if(head == nullptr)
        return tmp;

    Node *it{head};
    for(; it->next; it = it->next);

    it->next = tmp;
    tmp->prev = it;

    return head;
}

void print_reverse(Node *head)
{
    for(; head->next; head = head->next);

    while(head)
    {
        if(head->prev)
            std::cout << head->value << " -> ";
        else 
            std::cout << head->value;

        head = head->prev;
    }
    std::cout << "\n";
}

void print_linked_list(Node *head)
{
    if(head)
    {
        while(head)
        {
            if(head->next)
                std::cout << head->value << " -> ";
            else 
                std::cout << head->value;

            head = head->next;
        }
        std::cout << "\n";
    }
    else 
    {
        std::cout << "***** List is empty *****\n";
    }
}

bool free_linked_list(Node *head)
{
    if(head)
    {
        while(head)
        {
            Node *tmp = head;
            head = head->next;
            if(head)
                head->prev = nullptr;

            delete tmp;
        }
        return true;
    }
    else 
        return false;
}

Node* load_linked_list(int length)
{
    Node *head{nullptr};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

    for(int i{}; i < length; i++)
    {
        int value { dist(gen) };
        Node *tmp { new Node{ value, nullptr, nullptr } };
        //head = add_at_begin(head, tmp);
        head = add_at_end(head, tmp);
        std::cout << value << " ";
    }

    std::cout << "\n";

    return head;
}

int get_valid_value(const std::string_view sv)
{
    int value{};

    while(true)
    {
        std::cout << sv << " : ";
        std::cin >> value;

        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        break;
    }

    return value;
}

bool is_list_existed(Node *head)
{
    if(head)
        return true;
    else 
    {
        std::cout << "List doesn't exist\n";
        return false;        
    }
}

Node* add_at_position(Node *head, Node *tmp, int position)
{
    if(!is_list_existed(head))
        return head;

    Node *it {head}, *prev{nullptr};
    int i{};

    for(i; (i < position) && it; i++)
    {
        prev = it;
        it = it->next;
    }

    if(!position && !prev)
    {
        tmp->next = head;
        head->prev = tmp;
        head = tmp;
    }
    else if(it)
    {
        prev->next = tmp;
        tmp->prev = prev;
        tmp->next = it;
        it->prev = tmp;
    }
    else if( (i == position) && !it )
    {
        prev->next = tmp;
        tmp->prev = prev;
    }
    else 
        std::cout << "\n\n***** Invalid Position *****\n\n";

    return head;
}

Node* delete_at_begin(Node *head)
{
    if(!is_list_existed(head))
        return head;

    Node *tmp {head};
    head = head->next;
    if(head)
        head->prev = nullptr;

    delete tmp;

    return head;
}

Node* delete_at_end(Node *head)
{
    if(!is_list_existed(head))
        return head;
    
    Node *it{head};
    for(; it->next; it = it->next);

    if(it->prev)
    {
        it->prev->next = nullptr;
        delete it;
    }
    else if(it->prev == nullptr)
    {
        head = it->next;
        delete it;
    }

    return head;
}

Node* delete_at_position(Node *head, int position)
{
    if(!is_list_existed(head))
        return head;

    Node *it{head};
    for(int i{}; (i < position) && it; i++)
    {
        it = it->next;
    }

    if(it == nullptr)
    {
        std::cout << "\n\n***** Invalid position to delete node *****\n\n";
    }
    else 
    {
        if(it->prev)
        {
            it->prev->next = it->next;
            if(it->next)
                it->next->prev = it->prev;
        }
        else 
            head = it->prev;

        delete it;
    }
        
    return head;
}

Node *find_middle_ll(Node *head)
{
    if(!is_list_existed(head) || (head->next == nullptr))
        return head;

    Node *fast{head}, *slow{nullptr};

    while(fast && fast->next)
    {
        if(slow)
            slow = slow->next;
        else 
            slow = fast;

        fast = fast->next->next;
    }

    if(fast == nullptr)
        return slow;
    else if(fast->next == nullptr)
        return slow->next;

    return nullptr;
}

Node* reverse_ll(Node *head)
{
    if(!is_list_existed(head))
        return head;

    Node *it{head};
    while(it)
    {
        Node *tmp {it};
        it = it->next;

        if(it)
        {
            tmp->next = it->next;
            if(it->next)
                it->next->prev = tmp;
                
            it->next = head;
            it->prev = nullptr;
            head->prev = it;
            head = it;

            it = tmp;
        }
    }

    return head;
}

bool make_loop_at_given_index(Node *head, int position)
{
    if(!is_list_existed(head))
        return false;

    Node *it{head};
    int i{};
    for(i; (i < position) && it; i++)
        it = it->next;

    if(it == nullptr)
    {
        std::cout << "\n\n***** Invalid position to make loop *****\n\n";
        return false;
    }
    else if(it && (i == position))
    {
        Node *tmp {it};

        for(; it->next; it = it->next);

        it->next = tmp;
    }

    return true;
}

int find_loop_at_index(Node *head)
{
    if(!is_list_existed(head) || (head->next == nullptr))
        return -1;

    if(head->next == head)
        return 0;
    
    Node *fast{head}, *slow{head};

    while(fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if(slow == fast)
            break;
    }

    if(slow && fast && (slow == fast))
    {
        slow = head;
        int position{};

        while(slow != fast)
        {
            slow = slow->next;
            fast = fast->next;
            position++;
        }

        return position;
    }

    return -1;
}

Node* find_loop_at_node(Node *head)
{
    if(!is_list_existed(head) || (head->next == nullptr) || (head->next == head))
        return head;

    Node *fast{head}, *slow{head};

    while(fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if(slow == fast)
            break;
    }

    if(slow && fast && (slow == fast))
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

Node* remove_loop_ll(Node *head)
{
    if(!is_list_existed(head) || (head->next == nullptr))
        return head;

    if(head->next == head)
    {
        head->next = nullptr;
        return head;
    }

    Node *fast{head}, *slow{head};
    while(fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if(slow == fast)
            break;
    }

    if(slow && fast && (slow == fast))
    {
        slow = head;
        Node *prev{nullptr};

        while(slow != fast)
        {
            slow = slow->next;
            prev = fast;
            fast = fast->next;
        }

        if(prev)
            prev->next = nullptr;
        else 
        {
            fast = head;
            fast = fast->next;
            for(; fast->next != head; fast = fast->next);

            fast->next = nullptr;
        }
    }

    return head;
}

int main(int argc, char* argv[])
{
    bool exit {false}, input_failed{false};
    int length { std::stoi( std::string{ argv[1] } ) };
    Node *head { nullptr };
    head = load_linked_list(length);

    if(head)
        print_linked_list(head);
    
    while(!exit)
    {
        std::cout << "==================================================================\n";
        std::cout << " 0 - print the list\n";
        std::cout << " 1 - add_at_begin\n";
        std::cout << " 2 - add_at_end\n";
        std::cout << " 3 - add_at_position\n";
        std::cout << " 4 - delete_at_begin\n";
        std::cout << " 5 - delete_at_end\n";
        std::cout << " 6 - delete_at_position\n";
        std::cout << " 7 - print_reverse\n";
        std::cout << " 8 - find_middle_ll\n";
        std::cout << " 9 - reverse_ll\n";
        std::cout << " 10 - make_loop_at_given_index\n";
        std::cout << " 11 - find_loop_at_index/node\n";
        std::cout << " 12 - remove_loop_ll\n";
        std::cout << " 100 - quit\n";
        std::cout << "==================================================================\n";

        int choice { get_valid_value("Enter your choice") };
        
        switch(choice)
        {
            case 0:
                {
                    print_linked_list(head);
                    break;
                }
            
            case 1:
                {
                    int value { get_valid_value("Enter value to be added") };

                    Node *tmp { new Node{ value, nullptr, nullptr } };
                    head = add_at_begin(head, tmp);
                    print_linked_list(head);
                    break;
                }

            case 2:
                {
                    int value { get_valid_value("Enter value to be added") };

                    Node *tmp { new Node{ value, nullptr, nullptr } };
                    head = add_at_end(head, tmp);
                    print_linked_list(head);
                    break;
                }
            case 3:
                {
                    int value { get_valid_value("Enter value to be added") };
                    Node *tmp { new Node{value, nullptr, nullptr} };

                    int position { get_valid_value("Enter position at which the new node to be added") };

                    head = add_at_position(head, tmp, position);
                    print_linked_list(head);
                    break;
                }
            case 4:
                {
                    head = delete_at_begin(head);
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
                    int position { get_valid_value("Enter position at which the new node to be deleted") };
                    head = delete_at_position(head, position);
                    print_linked_list(head);
                    break;
                }

            case 7:
                {
                    print_reverse(head);
                    break;
                }
            case 8:
                {
                    Node *middle { find_middle_ll(head) };
                    if(middle)
                        std::cout << "Middle Node : " << middle->value << "\n";
                    break;
                }

            case 9:
                {
                    head = reverse_ll(head);
                    print_linked_list(head);
                    break;
                }
            case 10:
                {

                    int position { get_valid_value("Enter position at which loop to be made") };
                    if(make_loop_at_given_index(head, position))
                        std::cout << "Loop has been made at " << position << "\n";
                    else 
                        std::cout << "***** Loop failed *****\n";

                    break;
                }

            case 11:
                {
                    int index { find_loop_at_index(head) };
                    Node *node { find_loop_at_node(head) };
                    if( (index == -1) || (node == nullptr) )
                        std::cout << "\n\n***** Loop doesn't exist *****\n\n";
                    else 
                        std::cout << "Loop has been found at " << index << " and value " << node->value << "\n";

                    break;
                }
            
            case 12:
                {
                    head = remove_loop_ll(head);
                    print_linked_list(head);
                    break;
                }

            case 100:
                exit = true;
                break;

            default:
                std::cout << "\n\nInvalid Choice\n\n";
                break;
        }

    }

    free_linked_list(head);

    return 0;
}
