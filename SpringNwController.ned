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

package inet.networklayer.spring.control;

//
// TODO auto-generated type
//
simple SpringNwController like ISpringNwController
{
    parameters:
        @class(spring::NetworkController);
        string interfaceTableModule;   
        string routingTableModule;
        string springFecTableModule;
        string springFibTableModule;
        string springClassifierModule;
        
        int maxNodalSID = default(1024);
        int maxAdjacencySID = default(2048);
        int localPort = default(2000);

    gates:
        input udpIn;
        output udpOut;
}
