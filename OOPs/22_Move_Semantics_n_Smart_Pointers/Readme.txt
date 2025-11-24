1 — Introduction to smart pointers and move semantics
    -- The problem with raw-pointers is that Even if we remember to delete ptr at the end of the function, there are a myriad of ways that ptr may not be deleted if the function exits early.
    -- The raw pointers have no inherent mechanism to clean up after themselves.

    Smart pointer classes to the rescue
        -- classes contain destructors that automatically get executed when an object of the class goes out of scope. So if we allocate (or acquire) memory in your constructor, we can deallocate it in our destructor,
           and be guaranteed that the memory will be deallocated when the class object is destroyed.
        -- Consider a class whose sole job was to hold and “own” a pointer passed to it, and then deallocate that pointer when the class object went out of scope.
        -- A Smart pointer is a composition class that is designed to manage dynamically allocated memory and ensure that memory gets deleted when the smart pointer object goes out of scope.
        