//
//  Created on: Sep 27, 2022
//      Author: pool2win
//
	
	
	simple PoolNode
	{
	    parameters:
	        @display("i=block/routing");
	        double forwardingProbability = default(1.0);
	        int initialDelayMultiplier = default(1000);
	        int maxNumHops = default(2);
	    gates:
	        inout gate[];
	}
	
	network PoolNetwork
	{
	    types:
	        channel Channel extends ned.DelayChannel {
	            delay = 100ms;
	    }
	    submodules:
	        node[6]: PoolNode;
	    connections:
	        node[0].gate++ <--> Channel <--> node[1].gate++;
	        node[1].gate++ <--> Channel <--> node[2].gate++;
	        node[1].gate++ <--> Channel <--> node[4].gate++;
	        node[3].gate++ <--> Channel <--> node[4].gate++;	
	        node[4].gate++ <--> Channel <--> node[5].gate++;
	}
	