#ifndef SPRING_ERO_H_
#define SPRING_ERO_H_

#if STANDALONE_STREETE
#include "inet/networklayer/spring/standalone/IPv4Address.h"
#else
#include "inet/networklayer/contract/ipv4/IPv4Address.h"
#endif
#include "inet/networklayer/spring/SPRINGDefs.h"

namespace inet {

namespace spring {


class ERO {
public:
    IPv4Address srcIp;
    IPv4Address dstIp;
    SIDVector explicitRoute;
    friend std::ostream& operator<<(std::ostream& ostr, const ERO& ero);
};

typedef std::vector<ERO> EROs;
std::ostream& operator<<(std::ostream& ostr, const EROs& eros) ;

} //namespace spring

} //namespace inet


#endif /* SPRING_ERO_H_ */
