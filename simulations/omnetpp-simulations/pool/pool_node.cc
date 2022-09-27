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
        double forwarding_probability;
        int initial_delay_multiplier;
        int max_num_hops;
        virtual void sendCopyOf(PoolMessage *msg, int index);
        virtual void processMessage(PoolMessage *msg);
        virtual bool shouldForward(PoolMessage *msg, int index);
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void sendFirstMessage();
    };

    Define_Module(PoolNode);

    void PoolNode::sendFirstMessage(){
        // Boot the process scheduling the initial message as a self-message.
        char msgname[20];
        sprintf(msgname, "share-%d", getIndex());
        PoolMessage *msg = new PoolMessage(msgname);
        scheduleAt(initial_delay_multiplier * getIndex(), msg);
    }

    void PoolNode::initialize()
    {
        forwarding_probability = par("forwarding_probability");
        initial_delay_multiplier = par("initial_delay_multiplier");
        max_num_hops = par("max_num_hops");
        EV<< "max num hops " << max_num_hops << "\n";
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
                hop_count <= max_num_hops - 1 &&
                (roll <= forwarding_probability));
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
