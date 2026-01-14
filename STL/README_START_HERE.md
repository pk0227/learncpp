# 🎯 STL Interview Preparation - Getting Started

Welcome to your comprehensive STL interview preparation materials!

## 📖 Start Here

1. **[Master Index](file:///home/prashanth/learncpp_workspace/STL/00_Master_Index.md)** - Your navigation hub
2. **[Quick Reference](file:///home/prashanth/learncpp_workspace/STL/07_Quick_Reference.md)** - Cheat sheet for review

## 🚀 Quick Start (First Day)

### Morning (2-3 hours)
1. Read [STL Overview](file:///home/prashanth/learncpp_workspace/STL/01_STL_Overview.md) (30 min)
2. Read [Design Philosophy](file:///home/prashanth/learncpp_workspace/STL/02_Design_Philosophy.md) (45 min)
3. Skim [Container Selection Guide](file:///home/prashanth/learncpp_workspace/STL/04_Container_Selection_Guide.md) (30 min)

### Afternoon (2-3 hours)
1. Compile and run examples:
   ```bash
   cd /home/prashanth/learncpp_workspace/STL
   
   # Vector examples
   g++ -std=c++20 -Wall -Wextra -O2 examples/containers/vector_examples.cpp -o examples/containers/vector_examples
   ./examples/containers/vector_examples
   
   # LRU Cache
   g++ -std=c++20 -Wall -Wextra -O2 examples/interview_problems/problem_01_lru_cache.cpp -o examples/interview_problems/lru_cache
   ./examples/interview_problems/lru_cache
   ```

2. Try solving one problem from [Interview Problems](file:///home/prashanth/learncpp_workspace/STL/06_Interview_Problems.md)

## 🎯 Most Important Topics

1. **Container Selection** ⭐⭐⭐ (90% of interviews)
2. **Iterator Invalidation** ⭐⭐ (Common debugging questions)
3. **Complexity Analysis** ⭐⭐ (Every operation)
4. **Erase-Remove Idiom** ⭐ (Common gotcha)

## 📊 Material Overview

- **7 Core Documents** - Theory and concepts
- **4 Container Guides** - Detailed container documentation
- **2 Utility Guides** - Functors, lambdas, utility types
- **3 Code Examples** - Compilable, runnable code
- **10 Interview Problems** - Real questions with solutions
- **1 Quick Reference** - Cheat sheet

## 🎓 Interview Preparation Timeline

### 1 Week Before Interview
- [ ] Review Quick Reference daily
- [ ] Re-read Container Selection Guide
- [ ] Practice all Interview Problems
- [ ] Review Iterator Invalidation

### 2-3 Weeks Before Interview
- [ ] Read all core documentation
- [ ] Study all container guides
- [ ] Solve interview problems
- [ ] Run and understand all examples

### 4-5 Weeks Before Interview
- [ ] Complete reading (all documents)
- [ ] Practice coding examples
- [ ] Explain concepts out loud
- [ ] Create your own examples

## 💡 Study Tips

1. **Don't just read** - Explain concepts out loud
2. **Practice justifying** container choices
3. **Run the examples** - See code in action
4. **Solve problems** without looking at solutions first
5. **Review Quick Reference** before bed

## 🔥 Common Interview Questions

### Q: vector vs list?
**A:** vector for random access and cache efficiency. list only for frequent middle insert/delete on large containers.

### Q: map vs unordered_map?
**A:** map for ordering or range queries (O(log n)). unordered_map for pure lookup speed (O(1) average).

### Q: Why can't std::sort work on list?
**A:** std::sort needs random-access iterators. list has bidirectional. Use list::sort() instead (merge sort).

### Q: What's the erase-remove idiom?
**A:** `v.erase(std::remove(...), v.end())` - remove moves elements to end, erase actually removes them.

## 📚 Additional Resources

All materials are in: `/home/prashanth/learncpp_workspace/STL/`

- Core docs: Root directory
- Container guides: `containers/` folder
- Utilities: `utilities/` folder
- Examples: `examples/` folder

## ✅ Success Criteria

You're ready for interviews when you can:
- [ ] Explain container selection trade-offs
- [ ] Implement LRU Cache from memory
- [ ] Explain iterator invalidation rules
- [ ] Justify every container choice
- [ ] Solve interview problems in 20-30 minutes

---

**Good luck! You've got this! 🚀**

*For detailed walkthrough, see the artifacts directory.*
