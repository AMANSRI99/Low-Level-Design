#ifndef VISIT_TRACKER_H
#define VISIT_TRACKER_H

#include <vector>
#include <atomic>
#include "Helper.h"
#include <memory>

class VisitTracker {
    private:
        std::vector<std::unique_ptr<std::atomic<int>>> counters;//vector of pointers to atomic integers.
       // The reason std::vector<std::atomic<int>> counters; is not used directly is because std::atomic types are not copyable or moveable by design. This is a fundamental restriction of atomic types in C++. When a vector needs to grow (reallocate), it needs to move/copy its elements to the new memory location. Since std::atomic can't be copied/moved, operations like:
        Helper* helper;
    public:
        VisitTracker() = default;

        void init(int totalPages, Helper *helper) {
            this->helper = helper;

            if(totalPages>1000){
                helper->logging("totalPages should be in the range 1 to 1000");
                throw std::invalid_argument("Invalid number of pages");
            }


            // counters.resize(totalPages); doesn't work with atomics as it involves copying.
            // So, we need to use the following method to resize the vector.
            counters.clear();
            for(int i=0;i<totalPages;i++){
                counters.push_back(std::make_unique<std::atomic<int>>(0));
            }

            helper->logging("Initialised with pagr count ="+totalPages);
        }

        void incrementVisitCount(int pageIndex) 
        {
            if(pageIndex<0 || pageIndex>=counters.size()){
                helper->logging("Invalid page index");
            }
            counters[pageIndex]->fetch_add(1);
            helper->logging("Page index "+std::to_string(pageIndex)+" incremented");
        }

        int getVisitCount(int pageIndex){
            if(pageIndex<0 || pageIndex>=counters.size()){
                helper->logging("Invalid page index");
            }
            return counters[pageIndex]->load();
        }
};

#endif