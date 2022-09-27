//
// This file builds on the tictoc example from the omnetpp source code.
//

    #include <stdio.h>
    #include <string.h>
    #include <omnetpp.h>
    #include "pool_message_m.h"

    using namespace omnetpp;

    /**
     * Node in the simulation
     */
    class PoolNode : public cSimpleModule
    {
      protected:
        double forwardingProbability;
        int initialDelayMultiplier;
        int maxNumHops;
        int nextSequenceNumber = 0;
        virtual void sendCopyOf(PoolMessage *msg, int index);
        virtual void processMessage(PoolMessage *msg);
        virtual bool shouldForward(PoolMessage *msg, int index);
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void sendFirstMessage();
        virtual PoolMessage* generateNextMessage();
    };

    Define_Module(PoolNode);

    PoolMessage* PoolNode::generateNextMessage(){
        char msgname[20];
        auto sequenceNumber = nextSequenceNumber++;
        sprintf(msgname, "S%d: sNo: %d", getIndex(), sequenceNumber);
        PoolMessage *msg = new PoolMessage(msgname);
        msg->setSequenceNumber(sequenceNumber);
        return msg;
    }

    void PoolNode::sendFirstMessage(){
        // Boot the process scheduling the initial message as a self-message.
        PoolMessage *msg = generateNextMessage();
        scheduleAt(initialDelayMultiplier * getIndex(), msg);
    }

    void PoolNode::initialize()
    {
        forwardingProbability = par("forwardingProbability");
        initialDelayMultiplier = par("initialDelayMultiplier");
        maxNumHops = par("maxNumHops");
        if (getIndex() == 0) {
            sendFirstMessage();
        }
    }

    void PoolNode::handleMessage(cMessage *msg)
    {
        PoolMessage* pool_msg = check_and_cast<PoolMessage* >(msg);
        EV << "Message " << pool_msg << " arrived after " << pool_msg->getHopCount() << " hops.\n";
        processMessage(pool_msg);
    }

    void PoolNode::sendCopyOf(PoolMessage *msg, int index)
    {
        // Duplicate message and send the copy.
        PoolMessage *copy = msg->dup();
        copy->setHopCount(copy->getHopCount() + 1);
        send(copy, "gate$o", index);
    }

    bool PoolNode::shouldForward(PoolMessage *pool_msg, int index)
    {
        if (pool_msg->isSelfMessage())
                return true;
        auto received_from = pool_msg->getArrivalGate();
        double roll = uniform(0, 1);
        auto hop_count = pool_msg->getHopCount();
        return (gate("gate$i", index) != received_from &&
                hop_count <= maxNumHops - 1 &&
                (roll <= forwardingProbability));
    }

    void PoolNode::processMessage(PoolMessage *pool_msg)
    {
        // loop over all gates and send message out on each.
        auto num_gates = gateSize("gate$o") - 1;
        for (auto index = 0; index <= num_gates; index++){
            if (shouldForward(pool_msg, index)) {
                EV << "Forwarding message " << pool_msg << " on port channel" << index << "\n";
                sendCopyOf(pool_msg, index);
            }
        }
    }
