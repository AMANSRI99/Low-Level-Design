#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#pragma once
class ParkingLot {
    public:

    int floors, rows, columns;
    struct Spot {
        int type;
        std::string vehicleNumber;
        std::string ticketId;
        bool isOccupied;

        Spot() {
            this->type = 0;
            this->isOccupied = false;
        }
    };
    

    ParkingLot(int floors, int row, int column) {
        this->floors = floors;
        this->rows = row;
        this->columns = column;
        parkingLot.resize(floors);
        for(int i=0;i<floors;i++) {
            parkingLot[i].resize(row);
            for(int j=0;j<row;j++) {
                parkingLot[i][j].resize(column);
            }
        }
    }

    // Add a spot type at specific location
    bool addSpotType(int floor, int row, int column, int type) {
        if (!isValidLocation(floor, row, column)) return false;
        if (type != 0 && type != 2 && type != 4) return false;
        
        parkingLot[floor][row][column].type = type;
        return true;
    }

    // Check if location is valid
    bool isValidLocation(int floor, int row, int col) const {
        return floor >= 0 && floor < floors &&
               row >= 0 && row < rows &&
               col >= 0 && col < columns;
    }

    Spot* getSpot(int floor, int row, int column) {
        if (!isValidLocation(floor, row, column)) return nullptr;
        return &parkingLot[floor][row][column];
    }

    void addVehicle(const std::string& spotId, const std::string& vehicleNumber, const std::string& ticketId) {
        vehicleToSpotMap[vehicleNumber] = spotId;
        vehicleToSpotMap[ticketId] = spotId;
    }

    void removeVehicle(const std::string& vehicleNumber, const std::string& ticketId) {
        if( vehicleToSpotMap.find(vehicleNumber) == vehicleToSpotMap.end()) {
            return;
        }
        vehicleToSpotMap.erase(vehicleNumber);
        vehicleToSpotMap.erase(ticketId);
    }

    std::string findVehicle(const std::string& query) {
        if(vehicleToSpotMap.find(query) == vehicleToSpotMap.end()) {
            return "Vehicle Not Found";
        }
        return vehicleToSpotMap[query];
    }
    int countFreeSpots(int floor, int vehicleType) const {
        if(!isValidLocation(floor, 0, 0)) return 0;
        
        int count = 0;
        for(const auto& row : parkingLot[floor]) {
            for(const auto& spot : row) {
                if(spot.type == vehicleType && !spot.isOccupied) {
                    count++;
                }
            }
        }
        return count;
    }
    private:
    std::unordered_map<std::string, std::string> vehicleToSpotMap;
    std::vector<std::vector<std::vector<Spot>>> parkingLot;
};
