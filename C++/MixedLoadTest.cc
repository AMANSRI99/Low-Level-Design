#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include "visitTrackerUsingAtomics.h"

void runMixedLoadTest() {
    const int NUM_PAGES = 100;           // Number of pages to test
    const int NUM_WRITER_THREADS = 50;    // Threads that increment counters
    const int NUM_READER_THREADS = 20;    // Threads that read counters
    const int OPERATIONS_PER_THREAD = 1000; // Operations each thread will perform
    
    Helper helper;
    visitTrackerUsingAtomics tracker;
    tracker.init(NUM_PAGES, &helper);

    // Random number generator for selecting random pages
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pageDist(0, NUM_PAGES - 1);

    // Writer function - increments counters
    auto writerFunc = [&]() {
        for(int i = 0; i < OPERATIONS_PER_THREAD; i++) {
            int pageIndex = pageDist(gen);  // Pick a random page
            tracker.incrementVisitCount(pageIndex);
            
            // Optional: Add small random delay to simulate real-world usage
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    };

    // Reader function - reads counter values
    auto readerFunc = [&]() {
        for(int i = 0; i < OPERATIONS_PER_THREAD; i++) {
            int pageIndex = pageDist(gen);  // Pick a random page
            int count = tracker.getVisitCount(pageIndex);
            
            // Optional: Add small random delay
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    };

    // Create and start writer threads
    std::vector<std::thread> writerThreads;
    for(int i = 0; i < NUM_WRITER_THREADS; i++) {
        writerThreads.emplace_back(writerFunc);
    }

    // Create and start reader threads
    std::vector<std::thread> readerThreads; 
    for(int i = 0; i < NUM_READER_THREADS; i++) {
        readerThreads.emplace_back(readerFunc);
    }

    // Wait for all threads to complete
    for(auto& thread : writerThreads) {
        thread.join();
    }
    for(auto& thread : readerThreads) {
        thread.join();
    }

    // Print final counts for verification
    int totalHits = 0;
    for(int i = 0; i < NUM_PAGES; i++) {
        int count = tracker.getVisitCount(i);
        totalHits += count;
        std::cout << "Page " << i << ": " << count << " hits\n";
    }

    std::cout << "Total hits across all pages: " << totalHits << "\n";
    std::cout << "Expected total hits: " << NUM_WRITER_THREADS * OPERATIONS_PER_THREAD << "\n";
}

int main() {
    std::cout << "Starting mixed load test...\n";
    auto start = std::chrono::high_resolution_clock::now();
    
    runMixedLoadTest();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Test completed in " << duration.count() << "ms\n";
    
    return 0;
}