#include <cassert>

#include "EStore.h"

using namespace std;


Item::
Item() : valid(false)
{ }

Item::
~Item()
{ }


EStore::
EStore(bool enableFineMode)
    : fineMode(enableFineMode)
{
    // TODO: Your code here.
    setShippingCost(3.0);
    setStoreDiscount(0.0);

    for (int i = 0; i < INVENTORY_SIZE; ++i) {
        inventory[i].valid = false; 
    }
}

EStore::
~EStore()
{
    // TODO: Your code here.
}

/*
 * ------------------------------------------------------------------
 * buyItem --
 *
 *      Attempt to buy the item from the store.
 *
 *      An item can be bought if:
 *          - The store carries it.
 *          - The item is in stock.
 *          - The cost of the item plus the cost of shipping is no
 *            more than the budget.
 *
 *      If the store *does not* carry this item, simply return and
 *      do nothing. Do not attempt to buy the item.
 *
 *      If the store *does* carry the item, but it is not in stock
 *      or its cost is over budget, block until both conditions are
 *      met (at which point the item should be bought) or the store
 *      removes the item from sale (at which point this method
 *      returns).
 *
 *      The overall cost of a purchase for a single item is defined
 *      as the current cost of the item times 1 - the store
 *      discount, plus the flat overall store shipping fee.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
buyItem(int item_id, double budget)
{
    assert(!fineModeEnabled());


    // TODO: Your code here.
    std::unique_lock<mutex> lock(storeMutex);
    if (item_id < 0 || item_id >= INVENTORY_SIZE || !inventory[item_id].valid) {
        return;
    }

    itemCondition.wait(lock, [this, item_id, budget]() {
        Item& item = inventory[item_id];
        double totalCost = item.price * (1 - item.discount) + shippingCost;
        return !item.valid || (item.quantity > 0 && totalCost <= budget);
    });

    Item& item = inventory[item_id];
    double totalCost = item.price * (1 - item.discount) + shippingCost;
    if (item.valid && item.quantity > 0 && totalCost <= budget) {
        item.quantity -= 1;
    }
}

/*
 * ------------------------------------------------------------------
 * buyManyItem --
 *
 *      Attempt to buy all of the specified items at once. If the
 *      order cannot be bought, give up and return without buying
 *      anything. Otherwise buy the entire order at once.
 *
 *      The entire order can be bought if:
 *          - The store carries all items.
 *          - All items are in stock.
 *          - The cost of the the entire order (cost of items plus
 *            shipping for each item) is no more than the budget.
 *
 *      If multiple customers are attempting to buy at the same
 *      time and their orders are mutually exclusive (i.e., the
 *      two customers are not trying to buy any of the same items),
 *      then their orders must be processed at the same time.
 *
 *      For the purposes of this lab, it is OK for the store
 *      discount and shipping cost to change while an order is being
 *      processed.
 *
 *      The cost of a purchase of many items is the sum of the
 *      costs of purchasing each item individually. The purchase
 *      cost of an individual item is covered above in the
 *      description of buyItem.
 *
 *      Challenge: For bonus points, implement a version of this
 *      method that will wait until the order can be fulfilled
 *      instead of giving up. The implementation should be efficient
 *      in that it should not wake up threads unecessarily. For
 *      instance, if an item decreases in price, only threads that
 *      are waiting to buy an order that includes that item should be
 *      signaled (though all such threads should be signaled).
 *
 *      Challenge: For bonus points, ensure that the shipping cost
 *      and store discount does not change while processing an
 *      order.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
buyManyItems(vector<int>* item_ids, double budget)
{
    assert(fineModeEnabled());

    // TODO: Your code here.
    std::unique_lock<std::mutex> lock(storeMutex);

    double totalCost = 0;
    bool allItemsValid = true;
    
    for (int item_id : *item_ids) {
        if (item_id < 0 || item_id >= INVENTORY_SIZE || !inventory[item_id].valid || inventory[item_id].quantity <= 0) {
            allItemsValid = false;
            break;
        }
        totalCost += (inventory[item_id].price * (1 - inventory[item_id].discount) + shippingCost);
    }
    
    if (!allItemsValid || totalCost > budget) {
        return;
    }

    for (int item_id : *item_ids) {
        inventory[item_id].quantity -= 1;
    }
}

/*
 * ------------------------------------------------------------------
 * addItem --
 *
 *      Add the item to the store with the specified quantity,
 *      price, and discount. If the store already carries an item
 *      with the specified id, do nothing.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
addItem(int item_id, int quantity, double price, double discount)
{
    // TODO: Your code here.
    std::unique_lock<std::mutex> lock(storeMutex);
    
    if (item_id < 0 || item_id >= INVENTORY_SIZE) {
        return;
    }
    
    if (!inventory[item_id].valid) {
        inventory[item_id].valid = true;
        inventory[item_id].quantity = quantity;
        inventory[item_id].price = price;
        inventory[item_id].discount = discount;
    }
}

/*
 * ------------------------------------------------------------------
 * removeItem --
 *
 *      Remove the item from the store. The store no longer carries
 *      this item. If the store is not carrying this item, do
 *      nothing.
 *
 *      Wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
removeItem(int item_id)
{
    // TODO: Your code here.
    std::unique_lock<std::mutex> lock(storeMutex);
    
    if (item_id < 0 || item_id >= INVENTORY_SIZE) {
        return;
    }
    
    if (inventory[item_id].valid) {
        inventory[item_id].valid = false;
        inventory[item_id].quantity = 0;
        inventory[item_id].price = 0;
        inventory[item_id].discount = 0;
        itemCondition.notify_all();
    }
}

/*
 * ------------------------------------------------------------------
 * addStock --
 *
 *      Increase the stock of the specified item by count. If the
 *      store does not carry the item, do nothing. Wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
addStock(int item_id, int count)
{
    // TODO: Your code here.
    std::unique_lock<std::mutex> lock(storeMutex); 

    if (item_id >= 0 && item_id < INVENTORY_SIZE && inventory[item_id].valid) {
        inventory[item_id].quantity += count;
        itemCondition.notify_all();
    }
}

/*
 * ------------------------------------------------------------------
 * priceItem --
 *
 *      Change the price on the item. If the store does not carry
 *      the item, do nothing.
 *
 *      If the item price decreased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
priceItem(int item_id, double price)
{
    // TODO: Your code here.
    std::unique_lock<std::mutex> lock(storeMutex);
    
    if (item_id >= 0 && item_id < INVENTORY_SIZE && inventory[item_id].valid) {
        bool priceDecreased = price < inventory[item_id].price;
        inventory[item_id].price = price;
        if (priceDecreased) {
            itemCondition.notify_all();
        }
    }  
}

/*
 * ------------------------------------------------------------------
 * discountItem --
 *
 *      Change the discount on the item. If the store does not carry
 *      the item, do nothing.
 *
 *      If the item discount increased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
discountItem(int item_id, double discount)
{
    // TODO: Your code here.
    std::unique_lock<std::mutex> lock(storeMutex);
    if (item_id >= 0 && item_id < INVENTORY_SIZE && inventory[item_id].valid) {
        bool discountIncreased = discount > inventory[item_id].discount;
        inventory[item_id].discount = discount;
        if (discountIncreased) {
            itemCondition.notify_all();
        }
    }
    
}

/*
 * ------------------------------------------------------------------
 * setShippingCost --
 *
 *      Set the per-item shipping cost. If the shipping cost
 *      decreased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
setShippingCost(double cost)
{
    // TODO: Your code here.
    std::unique_lock<std::mutex> lock(storeMutex); 
    if(cost == shippingCost)
        return;
    if(cost < shippingCost)
        itemCondition.notify_all();

}


/*
 * ------------------------------------------------------------------
 * setStoreDiscount --
 *
 *      Set the store discount. If the discount increased, wake any
 *      waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
setStoreDiscount(double discount)
{
    std::unique_lock<std::mutex> lock(storeMutex);
    if (storeDiscount ==  discount){
        return;
    }
    lock.unlock();
    if(discount > storeDiscount){
        itemCondition.notify_all();
    }
        

}


