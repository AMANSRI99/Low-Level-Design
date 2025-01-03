#include <iostream>
#include"visitTracker.h"
int main() {
    Helper helper;
    VisitTracker tracker;
    
    // Initialize with 5 pages
    tracker.init(5, &helper);
    
    // Increment some visits
    tracker.incrementVisitCount(0);  // Will print: [LOG] Page 0 visits: 1
    tracker.incrementVisitCount(1);  // Will print: [LOG] Page 1 visits: 1
    tracker.incrementVisitCount(1);  // Will print: [LOG] Page 1 visits: 2
    
    // Get visit count
    int visits = tracker.getVisitCount(1);  // Will print: [LOG] Retrieved count for page 1: 2
    std::cout << "Visits for page 1: " << visits << std::endl;  // Will print: Visits for page 1: 2
}