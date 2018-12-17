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

#include "inet/networklayer/spring/control/SRGBDatabase.h"

namespace inet {

namespace spring {

bool SRGBDatabase::addRange(SegmentId firstLabel,
        unsigned int rangeSize) {
    if (rangeSize == 0) {
        return false;
    }
    auto b = srgb.begin();
    while (b != srgb.end()) {
        //go to next range block
        if (b->first + b->size <= firstLabel) {
            b++;
            continue;
        }
        //range exactly the same as the range in the block b
        if (b->first == firstLabel && b->size == rangeSize) {
            return true;
        }

        //range intersects with range in block b
        if (b->first < firstLabel + rangeSize) {
            return false;
        }

        srgb.emplace(b, firstLabel, rangeSize);
        return true;
    }

    srgb.emplace_back(firstLabel, rangeSize);
    return true;
}

bool SRGBDatabase::indexInRange(unsigned int index) {
    SegmentId temp;
    return getByIndex(index, temp);
}

bool SRGBDatabase::getByIndex(unsigned int index, SegmentId &result) {
    unsigned int i = 0;

    while (index >= 0 && i < srgb.size()) {
        if (index >= srgb[i].size) {
            index -= srgb[i].size;
            i++;
        } else {
            result = srgb[i].first + index;
            return true;
        }
    }
    return false;
}

bool SRGBDatabase::getIndexOfSID(SegmentId sid, unsigned int &result) {
    unsigned int i = 0;

    unsigned int index = 0;

    while (i < srgb.size()) {
        if (sid < srgb[i].first) {
            index += srgb[i].size;
            i++;
        } else {
            if (sid < srgb[i].first + srgb[i].size) {
                result = index + (sid - srgb[i].first);
                return true;
            }
            return false;
        }
    }
    return false;
}

} //namespace spring

} //namespace inet



