//
    // This file is part of an OMNeT++/OMNEST simulation example.
    //
    // Copyright (C) 2003 Ahmet Sekercioglu
    // Copyright (C) 2003-2015 Andras Varga
    //
    // This file is distributed WITHOUT ANY WARRANTY. See the file
    // `license' for details on this and other legal matters.
    //

    #include <stdio.h>
    #include <string.h>
    #include <omnetpp.h>

    using namespace omnetpp;

    /**
     * Let's make it more interesting by using several (n) `tic' modules,
     * and connecting every module to every other. For now, let's keep it
     * simple what they do: module 0 generates a message, and the others
     * keep tossing it around in random directions until it arrives at
     * module 3.
     */
    class Txc1 : public cSimpleModule
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

    Define_Module(Txc1);

    void Txc1::initialize()
    {
        forwarding_probability = par("forwarding_probability");
        initial_delay_multiplier = par("initial_delay_multiplier");
        // Boot the process scheduling the initial message as a self-message.
        char msgname[20];
        sprintf(msgname, "tic-%d", getIndex());
        cMessage *msg = new cMessage(msgname);
        scheduleAt(initial_delay_multiplier * getIndex(), msg);
    }

    void Txc1::handleMessage(cMessage *msg)
    {
        forwardMessage(msg);
    }

    void Txc1::sendCopyOf(cMessage *msg, int index)
    {
        // Duplicate message and send the copy.
        cMessage *copy = (cMessage *)msg->dup();
        send(copy, "gate$o", index);
    }

    bool Txc1::shouldForward(cMessage *msg, int index)
    {
        if (msg->isSelfMessage())
                return true;
        auto received_from = msg->getArrivalGate();
        double roll = uniform(0, 1);
        return (gate("gate$i", index) != received_from &&
                (roll <= forwarding_probability));
    }

    void Txc1::forwardMessage(cMessage *msg)
    {
        // loop over all gates and send message out on each.
        // TODO - add probabilistic forwarding
        auto num_gates = gateSize("gate$o") - 1;
        for (auto index = 0; index <= num_gates; index++){
            if (shouldForward(msg, index)) {
                EV << "Forwarding message " << msg << " on port channel" << index << "\n";
                sendCopyOf(msg, index);
            }
        }
    }
