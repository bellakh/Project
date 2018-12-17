/*
 * ERO.cpp
 *
 *  Created on: Mar 25, 2016
 *      Author: radu
 */

#include "inet/networklayer/spring/control/ERO.h"


namespace inet {

namespace spring {

std::ostream& operator<<(std::ostream& ostr, const ERO& ero) {
    ostr << "[" << ero.srcIp << " -> " << ero.dstIp << " : (";
    for (SegmentId id : ero.explicitRoute) {
        ostr << id << ",";
    }
    ostr << ")]";
    return ostr;
}

std::ostream& operator<<(std::ostream& ostr, const EROs& eros) {
    ostr << "{";
    for(auto &ero : eros ) {
        ostr << ero << ",";
    }
    ostr << "}";
    return ostr;
}


} //namespace spring

} //namespace inet

