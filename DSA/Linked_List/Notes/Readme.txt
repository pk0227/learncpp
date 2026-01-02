╔══════════════════════════════════════════════════════════════╗
║                    L I N K E D   L I S T                    ║
║      COMPLETE C++ INTERVIEW PREPARATION (ALL TYPES)         ║
╚══════════════════════════════════════════════════════════════╝


══════════════════════════════════════════════════════════════
0) WHAT IS A LINKED LIST?
══════════════════════════════════════════════════════════════
• A Linked List is a linear data structure
• Elements (nodes) are NOT stored contiguously
• Each node contains:
  - Data
  - Pointer(s) to next (and/or previous) node

Basic Node Structure (Singly LL):

struct Node {
    int data;
    Node* next;
};

--------------------------------------------------------------
WHY LINKED LIST?
--------------------------------------------------------------
• Dynamic size
• Efficient insertions/deletions
• No shifting of elements like arrays

--------------------------------------------------------------
DRAWBACKS
--------------------------------------------------------------
• No random access
• Extra memory for pointers
• Cache-unfriendly


══════════════════════════════════════════════════════════════
1) TYPES OF LINKED LISTS (MUST KNOW)
══════════════════════════════════════════════════════════════
1. Singly Linked List
2. Doubly Linked List
3. Circular Linked List
4. Circular Doubly Linked List


══════════════════════════════════════════════════════════════
2) SINGLY LINKED LIST
══════════════════════════════════════════════════════════════
Node:
[data | next]

Operations Interviewers Ask:
• Insertion (beginning, end, position)
• Deletion (beginning, end, position)
• Traversal
• Length calculation
• Search element


══════════════════════════════════════════════════════════════
3) BASIC OPERATIONS (CODE PATTERN)
══════════════════════════════════════════════════════════════

INSERT AT BEGINNING:
newNode->next = head;
head = newNode;

INSERT AT END:
traverse till last
last->next = newNode;
newNode->next = nullptr;

DELETE FROM BEGINNING:
temp = head;
head = head->next;
delete temp;

DELETE FROM END:
traverse till second last
delete last node
secondLast->next = nullptr;


══════════════════════════════════════════════════════════════
4) MOST ASKED SINGLY LINKED LIST QUESTIONS
══════════════════════════════════════════════════════════════

--------------------------------------------------------------
1️⃣ Reverse a Linked List
--------------------------------------------------------------
Methods:
• Iterative (MOST IMPORTANT)
• Recursive

ITERATIVE LOGIC:
prev = nullptr
curr = head
while(curr):
    next = curr->next
    curr->next = prev
    prev = curr
    curr = next
head = prev


--------------------------------------------------------------
2️⃣ Find Middle of Linked List
--------------------------------------------------------------
Technique:
• Slow & Fast pointers

Logic:
slow moves 1 step
fast moves 2 steps
When fast reaches end → slow is middle


--------------------------------------------------------------
3️⃣ Detect Cycle (VERY COMMON)
--------------------------------------------------------------
Technique:
• Floyd’s Cycle Detection (Tortoise & Hare)

Logic:
slow = slow->next
fast = fast->next->next
If slow == fast → cycle exists


--------------------------------------------------------------
4️⃣ Find Start of Cycle
--------------------------------------------------------------
After detecting cycle:
• Move one pointer to head
• Move both one step at a time
• Meeting point = start of cycle


--------------------------------------------------------------
5️⃣ Remove Cycle
--------------------------------------------------------------
• Find start of cycle
• Traverse till last node of cycle
• Set its next = nullptr


--------------------------------------------------------------
6️⃣ Nth Node from End
--------------------------------------------------------------
Technique:
• Two pointers

Logic:
Move first pointer n steps
Move both pointers until first reaches end
Second pointer = nth from end


--------------------------------------------------------------
7️⃣ Merge Two Sorted Linked Lists
--------------------------------------------------------------
Technique:
• Dummy node approach
• Compare nodes and link smaller one

Used in:
• Merge Sort on Linked List


--------------------------------------------------------------
8️⃣ Check if Linked List is Palindrome
--------------------------------------------------------------
Techniques:
• Reverse second half
• Stack method (extra space)


--------------------------------------------------------------
9️⃣ Intersection Point of Two Linked Lists
--------------------------------------------------------------
Techniques:
• Length difference method
• Two-pointer switching method


--------------------------------------------------------------
🔟 Remove Duplicates
--------------------------------------------------------------
• Sorted list → simple traversal
• Unsorted list → hashing or nested loops


══════════════════════════════════════════════════════════════
5) DOUBLY LINKED LIST
══════════════════════════════════════════════════════════════
Node:
[prev | data | next]

Advantages:
• Bidirectional traversal
• Easier deletion

Extra Cost:
• More memory
• More pointer updates

Common Questions:
• Reverse DLL
• Delete given node
• Insert at position


══════════════════════════════════════════════════════════════
6) CIRCULAR LINKED LIST
══════════════════════════════════════════════════════════════
Property:
• Last node points back to first node

Types:
• Circular Singly LL
• Circular Doubly LL

Common Questions:
• Detect circular list
• Traverse circular list
• Insert & delete nodes


══════════════════════════════════════════════════════════════
7) MERGE SORT ON LINKED LIST (IMPORTANT)
══════════════════════════════════════════════════════════════
Why Merge Sort?
• No random access needed
• Efficient pointer manipulation

Steps:
1. Find middle
2. Split list
3. Recursively sort halves
4. Merge sorted lists

Time:
O(n log n)

Space:
O(log n) (recursion stack)


══════════════════════════════════════════════════════════════
8) TIME & SPACE COMPLEXITY SUMMARY
══════════════════════════════════════════════════════════════

+------------------------------+------------+
| Operation                    | Complexity |
+------------------------------+------------+
| Access element               | O(n)       |
| Insert/Delete at beginning   | O(1)       |
| Insert/Delete at end         | O(n)       |
| Search                       | O(n)       |
| Reverse                      | O(n)       |
| Cycle detection              | O(n)       |
+------------------------------+------------+


══════════════════════════════════════════════════════════════
9) COMPARISON: ARRAY vs LINKED LIST
══════════════════════════════════════════════════════════════

+-------------------+------------+--------------+
| Feature           | Array      | Linked List  |
+-------------------+------------+--------------+
| Memory            | Contiguous | Non-contig   |
| Access            | O(1)       | O(n)         |
| Insert/Delete     | O(n)       | O(1)*        |
| Cache friendly    | Yes        | No           |
+-------------------+------------+--------------+


══════════════════════════════════════════════════════════════
10) INTERVIEW PRIORITY (WHAT TO MASTER)
══════════════════════════════════════════════════════════════
MUST MASTER:
✓ Reverse linked list
✓ Cycle detection
✓ Middle node
✓ Merge two sorted lists
✓ Nth node from end
✓ Palindrome check

GOOD TO KNOW:
✓ Circular list
✓ Doubly linked list
✓ Merge sort on LL


══════════════════════════════════════════════════════════════
11) INTERVIEW ONE-LINER
══════════════════════════════════════════════════════════════
A linked list is a dynamic linear data structure where nodes
are connected via pointers. It supports efficient insertions
and deletions but does not allow random access.

══════════════════════════════════════════════════════════════
