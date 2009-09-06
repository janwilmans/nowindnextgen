#pragma once

class Component 
{
public:
    Component();

    // the prepare method should create all objects and register them with the broker
    virtual void prepare() = 0;

    // the initialize method should create relations to other objects 
    // it can assume that 'prepare' has been called on all existing objects in the broker.
    virtual void initialize() = 0;

    // should do any actions needed before the destructor 
    // can be savely called, such as: unregister objects from the broker,
    // stop threads...etc.
    virtual void prepare_shutdown() = 0;

    // the destructor should release any allocated resources (memory/filehandles etc.. during runtime 
    virtual ~Component() {}
};

