/*
 * PoolMessageComparator.h
 *
 *  Created on: Sep 27, 2022
 *      Author: kulpreet
 */

#ifndef POOLMESSAGECOMPARATOR_H_
#define POOLMESSAGECOMPARATOR_H_

class PoolMessageComparator {
public:
    bool operator()(PoolMessage* a, PoolMessage* b){
        return a->getSequenceNumber() < b->getSequenceNumber();
    }
};

#endif /* POOLMESSAGECOMPARATOR_H_ */
