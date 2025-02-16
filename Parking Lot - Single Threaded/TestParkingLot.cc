#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include "SingleThreadedParkingLot.h"
#include <cassert>
#include <iostream>

class TestHelper : public Helper {
public:
    void logging(const std::string& msg) {
        std::cout << "Log: " << msg << std::endl;
    }
};

void testMultiFloorParking() {
    // Create a parking layout with 3 floors
    std::vector<std::vector<std::vector<int>>> parking = {
        {   // Floor 0
            {4, 4, 2, 2},  // 2 four-wheelers, 2 two-wheelers
            {2, 4, 2, 0},  // 1 four-wheeler, 2 two-wheelers
            {4, 4, 4, 0}   // 3 four-wheelers
        },
        {   // Floor 1 (more two-wheelers)
            {2, 2, 2, 2},  // 4 two-wheelers
            {2, 2, 2, 0},  // 3 two-wheelers
            {2, 2, 2, 0}   // 3 two-wheelers
        },
        {   // Floor 2 (more four-wheelers)
            {4, 4, 4, 4},  // 4 four-wheelers
            {4, 4, 4, 0},  // 3 four-wheelers
            {4, 4, 0, 0}   // 2 four-wheelers
        }
    };

    TestHelper helper;
    SingleThreadedParkingLot solution;
    solution.init(&helper, &parking);

    std::cout << "\nStarting Multi-Floor Parking Tests\n" << std::endl;

    // Test 1: Check initial free spot counts
    std::cout << "Test 1 - Initial free spot counts:" << std::endl;
    for(int floor = 0; floor < 3; floor++) {
        int twoWheelers = solution.getFreeSpotsCount(floor, 2);
        int fourWheelers = solution.getFreeSpotsCount(floor, 4);
        std::cout << "Floor " << floor << ": "
                  << twoWheelers << " two-wheelers, "
                  << fourWheelers << " four-wheelers" << std::endl;
    }

    // Test 2: Strategy 0 (lowest index) for two-wheeler
    std::string spotId1 = solution.park(2, "BIKE001", "TKT001", 0);
    std::cout << "\nTest 2 - Park 2-wheeler with strategy 0: " << spotId1 << std::endl;
    assert(spotId1.substr(0, 1) == "0");  // Should be on floor 0

    // Test 3: Strategy 1 (maximum free spots) for two-wheeler
    std::string spotId2 = solution.park(2, "BIKE002", "TKT002", 1);
    std::cout << "Test 3 - Park 2-wheeler with strategy 1: " << spotId2 << std::endl;
    assert(spotId2.substr(0, 1) == "1");  // Should be on floor 1 (most 2-wheeler spots)

    // Test 4: Strategy 1 for four-wheeler
    std::string spotId3 = solution.park(4, "CAR001", "TKT003", 1);
    std::cout << "Test 4 - Park 4-wheeler with strategy 1: " << spotId3 << std::endl;
    assert(spotId3.substr(0, 1) == "2");  // Should be on floor 2 (most 4-wheeler spots)

    // Test 5: Fill up all four-wheeler spots on a floor
    std::cout << "\nTest 5 - Fill up floor 2 four-wheeler spots:" << std::endl;
    for(int i = 2; i <= 10; i++) {
        std::string carNum = "CAR" + std::to_string(i);
        std::string tktNum = "TKT" + std::to_string(i + 2);
        std::string spotId = solution.park(4, carNum, tktNum, 1);
        std::cout << "Parked " << carNum << " at " << spotId << std::endl;
    }

    // Test 6: Verify spot distribution across floors
    std::cout << "\nTest 6 - Final free spot counts:" << std::endl;
    for(int floor = 0; floor < 3; floor++) {
        int twoWheelers = solution.getFreeSpotsCount(floor, 2);
        int fourWheelers = solution.getFreeSpotsCount(floor, 4);
        std::cout << "Floor " << floor << ": "
                  << twoWheelers << " two-wheelers, "
                  << fourWheelers << " four-wheelers" << std::endl;
    }

    // Test 7: Remove vehicles and verify counts
    std::cout << "\nTest 7 - Remove vehicles and verify:" << std::endl;
    assert(solution.removeVehicle(spotId1));
    assert(solution.removeVehicle(spotId2));
    assert(solution.removeVehicle(spotId3));
    
    for(int floor = 0; floor < 3; floor++) {
        int twoWheelers = solution.getFreeSpotsCount(floor, 2);
        int fourWheelers = solution.getFreeSpotsCount(floor, 4);
        std::cout << "Floor " << floor << ": "
                  << twoWheelers << " two-wheelers, "
                  << fourWheelers << " four-wheelers" << std::endl;
    }

    std::cout << "\nAll multi-floor tests passed successfully!" << std::endl;
}

int main() {
    try {
        testMultiFloorParking();
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}