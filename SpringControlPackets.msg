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

cplusplus {{
#include <vector>
#include <utility>
#include "inet/networklayer/spring/SPRINGDefs.h"
#include "inet/networklayer/spring/control/ERO.h"
#include "inet/networklayer/spring/control/NetGraph.h"
}}

namespace inet::spring;

cplusplus {{
typedef std::vector<std::pair<SegmentId, double>> StatsVector;
typedef std::vector<std::pair<unsigned int, SegmentId>> OffLinks;
}}

class noncobject SegmentId;
class noncobject StatsVector;
class noncobject EROs;
class noncobject OffLinks;


packet UDPStats
{
    SegmentId origSid;
    StatsVector stats;
}
packet UDPEros
{
    EROs eros;
}

class OSPFOpaqueLinksOff 
{
    OffLinks offLinks;
}