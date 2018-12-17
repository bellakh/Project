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
#include "inet/common/INETDefs.h"
#include "inet/networklayer/spring/SPRINGDefs.h"
#include "inet/routing/ospfv2/opaque/OSPFOpaque.h"

}}



class noncobject IPv4Address;
class noncobject inet::ospf::OSPFOpaqueTLVs;
class noncobject inet::ospf::OSPFOpaqueLSA;

namespace inet::spring;

class noncobject SegmentId;

// following the ietf draft on the ospf extension for segment routing
//
// draft-ietf-ospf-segment-routing-extensions-03

//
//section-2.1
//
class SIDLabelValue 
{
    SegmentId sid;
}


//
//section-3.1
//
enum SRAlgorithmType
{
    IGP_METRIC_BASED_SPT = 0;
}
class SRAlgorithmValue 
{
    char algoritm[] @enum(SRAlgorithmType); 
}


//
//section-5
//
struct PrefixSIDFlags{
    bool N_NodeSID;
    bool NP_NoPHP;
    bool M_MappingServer;
    bool E_ExplicitNULL;
    bool V_ValueIndex;
    bool L_LocalGlobal;
    bool reserved1;
    bool reserved2;
}
class PrefixSIDValue 
{
	PrefixSIDFlags flags;
	char reserved;
	char multiTopoID;
	char algorithm;
	int labelOrIdx;
}

//
//section-6
//
enum SIDLabelBindSubTLVType
{
    SID_LABEL = 1;
    IPV4_ERO = 4;
    UNNUMBERED_INTERFACE_ERO = 5;
    IPV4_BACKUP_ERO = 6;
    UNNUMBERED_INTERFACE_BACKUP_ERO = 7;
    ERO_METRIC = 8;
}
struct SidLabelBindFlags{
    bool M_MirroringContext;
	bool reserved1;
	bool reserved2;
	bool reserved3;
	bool reserved4;
	bool reserved5;
	bool reserved6;
	bool reserved7;
}
class SIDLabelBindValue
{
	SidLabelBindFlags flags;
	char reserved;
	char multiTopoID;
	char weight;
	
	inet::ospf::OSPFOpaqueTLVs subTLVs;
}

//
//section-7
//
struct AdjSIDFlags{
    bool B_Backup; //true if adjacency protected
	bool V_ValueIndex; //true if absolute value
	bool L_LocalGlobal; //true if global
	bool S_Set; //true if set of adjacencies
	bool reserved1;
	bool reserved2;
	bool reserved3;
	bool reserved4;
}

class AdjSIDValue 
{
    AdjSIDFlags flags;
	char reserved;
	char multiTopoID;
	char weight;
	int labelOrIdx;
	
	double cost; //TODO: remove, not in specification
	double datarate;
}

