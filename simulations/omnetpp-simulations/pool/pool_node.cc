//
// This file builds on the tictoc example from the omnetpp source code.
//

    #include <stdio.h>
    #include <string.h>
    #include <omnetpp.h>

    using namespace omnetpp;

    /**
     * Node in the simulation
     */
    class PoolNode : public cSimpleModule
    {
      protected:
        double forwarding_probability;
        int initial_delay_multiplier;
        virtual void sendCopyOf(cMessage *msg, int index);
        virtual void forwardMessage(cMessage *msg);
        virtual bool shouldForward(cMessage *msg, int index);
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
    };

    Define_Module(PoolNode);

    void PoolNode::initialize()
    {
        forwarding_probability = par("forwarding_probability");
        initial_delay_multiplier = par("initial_delay_multiplier");
        // Boot the process scheduling the initial message as a self-message.
        char msgname[20];
        sprintf(msgname, "share-%d", getIndex());
        cMessage *msg = new cMessage(msgname);
        scheduleAt(initial_delay_multiplier * getIndex(), msg);
    }

    void PoolNode::handleMessage(cMessage *msg)
    {
        forwardMessage(msg);
    }

    void PoolNode::sendCopyOf(cMessage *msg, int index)
    {
        // Duplicate message and send the copy.
        cMessage *copy = (cMessage *)msg->dup();
        send(copy, "gate$o", index);
    }

    bool PoolNode::shouldForward(cMessage *msg, int index)
    {
        if (msg->isSelfMessage())
                return true;
        auto received_from = msg->getArrivalGate();
        double roll = uniform(0, 1);
        return (gate("gate$i", index) != received_from &&
                (roll <= forwarding_probability));
    }

    void PoolNode::forwardMessage(cMessage *msg)
    {
        // loop over all gates and send message out on each.
        auto num_gates = gateSize("gate$o") - 1;
        for (auto index = 0; index <= num_gates; index++){
            if (shouldForward(msg, index)) {
                EV << "Forwarding message " << msg << " on port channel" << index << "\n";
                sendCopyOf(msg, index);
            }
        }
    }
