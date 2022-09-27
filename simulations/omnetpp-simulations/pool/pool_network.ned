
	//
	// This file is part of an OMNeT++/OMNEST simulation example.
	//
	// Copyright (C) 2003-2015 Andras Varga
	//
	// This file is distributed WITHOUT ANY WARRANTY. See the file
	// `license' for details on this and other legal matters.
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
	