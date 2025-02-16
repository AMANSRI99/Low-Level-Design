#include "../HelperClasses/Helper.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <string>
#include "ParkingLot.h"

#pragma once

class SingleThreadedParkingLot {
    private:

    ParkingLot parkingLot;
    Helper* helper;

    std::string getSpotId(int floor, int row, int column) {
        return std::to_string(floor) + "-" + std::to_string(row) + "-" + std::to_string(column);
    }
    bool parseSpotId(const std::string& spotId, int& floor, int& row, int& col) {
        int firstDash = spotId.find('-');
        int secondDash = spotId.find('-', firstDash + 1);
        
        if (firstDash == std::string::npos || secondDash == std::string::npos) return false;
        
        floor = stoi(spotId.substr(0, firstDash));
        row = stoi(spotId.substr(firstDash + 1, secondDash - firstDash - 1));
        col = stoi(spotId.substr(secondDash + 1));
        
        return true;
    }


    public:
    SingleThreadedParkingLot() : parkingLot(1, 1, 1), helper(nullptr) {}

    //parking strategy 0.
    bool getFirstAvailableSpot(int vehicleType, int &floor, int &row, int &column) {
        for(int i=0;i<parkingLot.floors;i++) {
            for(int j=0;j<parkingLot.rows;j++) {
                for(int k=0;k<parkingLot.columns;k++) {
                    auto spot = parkingLot.getSpot(i,j,k);
                    if(spot && spot->type == vehicleType && spot->isOccupied == false) {
                        floor = i;
                        row = j;
                        column = k;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    //parking strategy 1.
    bool getSpotOnFloorWithMaximumFreeSpots(int vehicleType, int &floor, int &row, int &column) {
        int maxFreeSpots = 0;
        int selectedFloor = -1;

        for (int i=0;i<parkingLot.floors;i++) {
            int freeSpots = 0;
            for(int j=0;j<parkingLot.rows;j++) {
                for(int k=0;k<parkingLot.columns;k++) {
                    auto spot = parkingLot.getSpot(i,j,k);
                    if(spot && spot->type == vehicleType && spot->isOccupied == false) {
                        freeSpots++;
                    }
                }
            }
            if(freeSpots > maxFreeSpots) {
                maxFreeSpots = freeSpots;
                selectedFloor = i;
            }
        }
        if(selectedFloor == -1) {
            return false;
        }

        //get free spot from the selected floor.
        for(int j=0;j<parkingLot.rows;j++) {
            for(int k=0;k<parkingLot.columns;k++) {
                auto spot = parkingLot.getSpot(selectedFloor,j,k);
                if(spot && spot->type == vehicleType && spot->isOccupied == false) {
                    floor = selectedFloor;
                    row = j;
                    column = k;
                    return true;
                }
            }
        }
        return false;
    }

    void init(Helper* helper, std::vector<std::vector<std::vector<int>>>* parking) {
        this->helper = helper;
        int floors = parking->size();
        int rows = (*parking)[0].size();
        int cols = (*parking)[0][0].size();
        parkingLot = ParkingLot(floors, rows, cols);

        for(int i=0;i<floors;i++) {
            for(int j=0;j<rows;j++) {
                for(int k=0;k<cols;k++) {
                    parkingLot.addSpotType(i, j, k, (*parking)[i][j][k]);
                }
            }
        }
    }

    std::string park(int vehicleType, std::string vehicleNumber, std::string ticketId, int parkingStrategy){
        int floor = -1, row = -1, column = -1;
        if(parkingStrategy == 0) {
            if(!getFirstAvailableSpot(vehicleType, floor, row, column)) {
                return "No spot available";
            }

        } else if(parkingStrategy == 1) {
            if(!getSpotOnFloorWithMaximumFreeSpots(vehicleType, floor, row, column)) {
                return "No spot available";
            }
        } else {
            return "Invalid parking strategy";
        }
        
        ParkingLot::Spot* spot = parkingLot.getSpot(floor, row, column);
        if(!spot) {
            return "Invalid spot";
        }
        spot->isOccupied=true;
        spot->ticketId = ticketId;
        spot->vehicleNumber = vehicleNumber;
        std::string spotId = getSpotId(floor, row, column);
        helper->logging("Parking vehicle " + vehicleNumber + " at spot " + spotId);
        parkingLot.addVehicle(spotId, vehicleNumber, ticketId);
        return spotId;
    }

    bool removeVehicle(std::string spotId) {
        int floor, row, col;
        if(!parseSpotId(spotId, floor, row, col)) return false;

        try {
            ParkingLot::Spot* spot = parkingLot.getSpot(floor, row, col);
            if(!spot->isOccupied) return false;

            parkingLot.removeVehicle(spot->vehicleNumber, spot->ticketId);
            spot->isOccupied = false;
            spot->vehicleNumber = "";
            spot->ticketId = "";
            return true;
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    std::string searchVehicle(const std::string& query) {
        return parkingLot.findVehicle(query);
    }
    int getFreeSpotsCount(int floor, int vehicleType) {
        return parkingLot.countFreeSpots(floor, vehicleType);
    }
};