#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cassert>

#include "EStore.h"
#include "TaskQueue.h"
#include "RequestGenerator.h" 

class Simulation {
    public:
    TaskQueue supplierTasks;
    TaskQueue customerTasks;
    EStore store;

    int maxTasks;
    int numSuppliers;
    int numCustomers;

    explicit Simulation(bool useFineMode) : store(useFineMode) { }
};

/*
 * ------------------------------------------------------------------
 * supplierGenerator --
 *
 *      The supplier generator thread. The argument is a pointer to
 *      the shared Simulation object.
 *
 *      Enqueue arg->maxTasks requests to the supplier queue, then
 *      stop all supplier threads by enqueuing arg->numSuppliers
 *      stop requests.
 *
 *      Use a SupplierRequestGenerator to generate and enqueue
 *      requests.
 *
 *      This thread should exit when done.
 *
 * Results:
 *      Does not return. Exit instead.
 *
 * ------------------------------------------------------------------
 */
static void*
supplierGenerator(void* arg)
{
    // TODO: Your code here.
    Simulation* simulation = static_cast<Simulation*>(arg);
    SupplierRequestGenerator generator(&(simulation->supplierTasks));
    generator.enqueueTasks(simulation->maxTasks, &(simulation->store));
    generator.enqueueStops(simulation->numSuppliers);
    return NULL;
}

/*
 * ------------------------------------------------------------------
 * customerGenerator --
 *
 *      The customer generator thread. The argument is a pointer to
 *      the shared Simulation object.
 *
 *      Enqueue arg->maxTasks requests to the customer queue, then
 *      stop all customer threads by enqueuing arg->numCustomers
 *      stop requests.
 *
 *      Use a CustomerRequestGenerator to generate and enqueue
 *      requests.  For the fineMode argument to the constructor
 *      of CustomerRequestGenerator, use the output of
 *      store.fineModeEnabled() method, where store is a field
 *      in the Simulation class.
 *
 *      This thread should exit when done.
 *
 * Results:
 *      Does not return. Exit instead.
 *
 * ------------------------------------------------------------------
 */
static void*
customerGenerator(void* arg)
{
    // TODO: Your code here.
    Simulation* simulation = static_cast<Simulation*>(arg);
    
    if(!simulation){
    std::cerr<<"object is NULL"<< std::endl;
    assert(false);
    return NULL;
    }
    CustomerRequestGenerator generator(&(simulation->customerTasks), simulation->store.fineModeEnabled());
    generator.enqueueTasks(simulation->maxTasks, &(simulation->store));
    generator.enqueueStops(simulation->numCustomers);
    return NULL; // Keep compiler happy.
}

/*
 * ------------------------------------------------------------------
 * supplier --
 *
 *      The main supplier thread. The argument is a pointer to the
 *      shared Simulation object.
 *
 *      Dequeue Tasks from the supplier queue and execute them.
 *
 * Results:
 *      Does not return.
 *
 * ------------------------------------------------------------------
 */
static void*
supplier(void* arg)
{
    // TODO: Your code here.
    Simulation* simulation = static_cast<Simulation*>(arg);
    while (true) {
        Task task = simulation->supplierTasks.dequeue();

        if (task.handler == NULL){
             break;
        }
        task.handler(task.arg);
        free(task.arg);
    }

    return NULL; // Keep compiler happy.
}

/*
 * ------------------------------------------------------------------
 * customer --
 *
 *      The main customer thread. The argument is a pointer to the
 *      shared Simulation object.
 *
 *      Dequeue Tasks from the customer queue and execute them.
 *
 * Results:
 *      Does not return.
 *
 * ------------------------------------------------------------------
 */
static void*
customer(void* arg)
{
    // TODO: Your code here.
    Simulation* simulation = static_cast<Simulation*>(arg);
    while (true) {
        Task task = simulation->customerTasks.dequeue();

        if (task.handler == NULL){
             break;
        }
        task.handler(task.arg);
        free(task.arg);
    }

    return NULL; // Keep compiler happy.
}

/*
 * ------------------------------------------------------------------
 * startSimulation --
 *      Create a new Simulation object. This object will serve as
 *      the shared state for the simulation. 
 *
 *      Create the following threads:
 *          - 1 supplier generator thread.
 *          - 1 customer generator thread.
 *          - numSuppliers supplier threads.
 *          - numCustomers customer threads.
 *
 *      After creating the worker threads, the main thread
 *      should wait until all of them exit, at which point it
 *      should return.
 *
 *      Hint: Use sthread_join.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
static void
startSimulation(int numSuppliers, int numCustomers, int maxTasks, bool useFineMode)
{
    // TODO: Your code here.
    Simulation simulation(useFineMode);
    simulation.maxTasks = maxTasks;
    simulation.numSuppliers = numSuppliers;
    simulation.numCustomers = numCustomers;

    std::vector<sthread_t> threads;

    sthread_t supplierGeneratorThread;
    sthread_create(&supplierGeneratorThread, supplierGenerator, &simulation);
    threads.push_back(supplierGeneratorThread);
    
    sthread_t customerGeneratorThread;
    sthread_create(&customerGeneratorThread, customerGenerator, &simulation);
    threads.push_back(customerGeneratorThread);

    for(int i = 0;i < numCustomers; i++){
        sthread_t customerThread;
        sthread_create(&customerThread, customer, &simulation);
        threads.push_back(customerThread);
    }
    for(int i = 0; i < numSuppliers; i++){
        sthread_t supplierThread;
        sthread_create(&supplierThread, supplier, &simulation);
        threads.push_back(supplierThread);
    }
    for(sthread_t thread : threads){
        sthread_join(thread);
    }
}

int main(int argc, char **argv)
{
    bool useFineMode = false;

    // Seed the random number generator.
    // You can remove this line or set it to some constant to get deterministic
    // results, but make sure you put it back before turning in.
    srand(time(NULL));

    if (argc > 1)
        useFineMode = strcmp(argv[1], "--fine") == 0;
    startSimulation(10, 10, 100, useFineMode);
    return 0;
}

