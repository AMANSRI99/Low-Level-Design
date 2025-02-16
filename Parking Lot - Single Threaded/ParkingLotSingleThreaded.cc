#include "../Helper.h"
class SingleThreadedParkingLot {
    public:
    int parkingLotSize;
    int availableSpaces;

    SingleThreadedParkingLot(int parkingLotSize) {
        this->parkingLotSize = parkingLotSize;
        this->availableSpaces = parkingLotSize;
    }
    void init() {

    }
}