#include <ios>
#include <iostream>
#include <limits>
#include <random>
#include <string>

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
        head = add_at_begin(head, tmp);
        std::cout << value << " ";
    }

    std::cout << "\n";

    return head;
}

int main(int argc, char* argv[])
{
    bool exit {false}, input_failed{false};
    int length { std::stoi( std::string{ argv[1] } ) };
    Node *head { nullptr };
    //head = load_linked_list(length);

    if(head)
        print_linked_list(head);
    
    while(!exit)
    {
        std::cout << "==================================================================\n";
        std::cout << " 0 - print the list\n";
        std::cout << " 1 - add_at_begin\n";
        std::cout << " 2 - add_at_end\n";
        std::cout << " 3 - add_at_position\n";
        std::cout << " 7 - print_reverse\n";
        std::cout << "==================================================================\n";

        int choice{};
        std::cout << "Enter your choice : ";
        std::cin >> choice;
        if(std::cin.fail())
        {
            input_failed = true;
            goto CLEAR;
        }
        
        switch(choice)
        {
            case 0:
                {
                    print_linked_list(head);
                    break;
                }
            
            case 1:
                {
                    int value{};
                    while(true)
                    {
                        std::cout << "Enter value to be added : ";
                        std::cin >> value;
                        if(std::cin.fail())
                        {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            continue;
                        }
                        break;
                    }

                    Node *tmp { new Node{ value, nullptr, nullptr } };
                    head = add_at_begin(head, tmp);
                    break;
                }

            case 2:
                {
                    int value{};
                    while(true)
                    {
                        std::cout << "Enter value to be added : ";
                        std::cin >> value;
                        if(std::cin.fail())
                        {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            continue;
                        }
                        break;
                    }

                    Node *tmp { new Node{ value, nullptr, nullptr } };
                    head = add_at_end(head, tmp);
                    break;
                }

            default:
                std::cout << "Invalid Choice\n";
                break;
        }

CLEAR:
        if(input_failed)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    free_linked_list(head);

    return 0;
}
