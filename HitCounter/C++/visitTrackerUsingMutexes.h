#ifndef VISIT_TRACKER_MUTEX_H
#define VISIT_TRACKER_MUTEX_H
#include "../../HelperClasses/Helper.h"
#include<vector>
#include<mutex>
#include<memory>

class visitTrackerUsingMutexes {
    private:
        std::vector<int>counters;
        std::mutex mutex;
        Helper* helper;

    public:
        visitTrackerUsingMutexes() = default;

        void init(int totalPages, Helper* helper) {
            this->helper = helper;
            if(totalPages>1000){
                helper->logging("totalPages should be in the range 1 to 1000");
                throw std::invalid_argument("Invalid number of pages");
            }
            counters.resize(totalPages,0);
            helper->logging("Initialised with page count = "+std::to_string(totalPages));
        }

        void incrementVisitCount(int pageIndex) {
            std::lock_guard<std::mutex>Lock(mutex);
            if(pageIndex<0 || pageIndex>=1000){
                helper->logging("Invalid page index");
                return;
            }
            counters[pageIndex]++;
            helper->logging("Page index "+std::to_string(pageIndex)+" incremented");
        }

        int getVisitCount(int pageIndex) {
            std::lock_guard<std::mutex>Lock(mutex);
            if(pageIndex<0 || pageIndex>=1000){
                helper->logging("Invalid page index");
                return -1;
            }
            return counters[pageIndex];
        }
};
#endif