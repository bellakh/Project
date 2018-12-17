//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef SPRINGSRGBDATABASE_H_
#define SPRINGSRGBDATABASE_H_


#include "inet/networklayer/spring/SPRINGDefs.h"


namespace inet {

namespace spring {


class SRGBDatabase: public cObject {
private:
    class Block{
    public:
        SegmentId first;
        unsigned int size;

        Block():first(0),size(0){}
        Block(SegmentId _first, unsigned int _size):first(_first),size(_size){}
    };
public:
    std::vector<Block> srgb;
    bool addRange(SegmentId firstLabel, unsigned int rangeSize);
    bool indexInRange(unsigned int index) ;
    bool getByIndex(unsigned int index, SegmentId &result) ;
    bool getIndexOfSID(SegmentId sid, unsigned int &result) ;

};

} //namespace spring

} //namespace inet


#endif /* SPRINGSRGBDATABASE_H_ */
