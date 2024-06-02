/*
 * ------------------------------------------------------------------
 * add_item_handler --
 *
 *      Handle an AddItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
add_item_handler(void *args)
{
    // TODO: Your code here.
    AddItemReq* req = static_cast<AddItemReq*>(args);

    std::cout << "Handling AddItemReq: item_id - " << req->item_id << ", quantity - " << req->quantity << ", price - $" << req->price << ", discount - " << req->discount << std::endl;
    req->store->addItem(req->item_id, req->quantity, req->price, req->discount);
    delete req;
}

/*
 * ------------------------------------------------------------------
 * remove_item_handler --
 *
 *      Handle a RemoveItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
remove_item_handler(void *args)
{
    // TODO: Your code here.
    RemoveItemReq* request = static_cast<RemoveItemReq*>(args);

    std::cout << "Handling RemoveItemReq: item_id - " << request->item_id << std::endl;
    globalStore->removeItem(request->item_id);
    delete request;
}

/*
 * ------------------------------------------------------------------
 * add_stock_handler --
 *
 *      Handle an AddStockReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
add_stock_handler(void *args)
{
    // TODO: Your code here.
    AddStockReq* request = static_cast<AddStockReq*>(args);
    
    std::cout << "Handling AddStockReq: item_id - " << request->item_id << ", count - " << request->count << std::endl;
    globalStore->addStock(request->item_id, request->count);
    delete request;
}

/*
 * ------------------------------------------------------------------
 * change_item_price_handler --
 *
 *      Handle a ChangeItemPriceReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
change_item_price_handler(void *args)
{
    // TODO: Your code here.
    ChangeItemPriceReq* request = static_cast<ChangeItemPriceReq*>(args);
    
    std::cout << "Handling ChangeItemPriceReq: item_id - " << request->item_id << ", new_price - $" << request->new_price << std::endl;
    globalStore->priceItem(request->item_id, request->new_price);
    delete request;
}

/*
 * ------------------------------------------------------------------
 * change_item_discount_handler --
 *
 *      Handle a ChangeItemDiscountReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
change_item_discount_handler(void *args)
{
    // TODO: Your code here.
    ChangeItemDiscountReq* request = static_cast<ChangeItemDiscountReq*>(args);

    std::cout << "Handling ChangeItemDiscountReq: item_id - " << request->item_id << ", new_discount - " << request->new_discount << std::endl;
    globalStore->discountItem(request->item_id, request->new_discount);
    delete request;

}

/*
 * ------------------------------------------------------------------
 * set_shipping_cost_handler --
 *
 *      Handle a SetShippingCostReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
set_shipping_cost_handler(void *args)
{
    // TODO: Your code here.
    SetShippingCostReq* request = static_cast<SetShippingCostReq*>(args);

    std::cout << "Handling SetShippingCostReq: cost - $" << request->shipping_cost << std::endl;
    globalStore->setShippingCost(request->shipping_cost);
    delete request;
}

/*
 * ------------------------------------------------------------------
 * set_store_discount_handler --
 *
 *      Handle a SetStoreDiscountReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
set_store_discount_handler(void *args)
{
    // TODO: Your code here.
     SetStoreDiscountReq* request = static_cast<SetStoreDiscountReq*>(args);

    std::cout << "Handling SetStoreDiscountReq: new_discount - " << request->new_discount << std::endl;
    globalStore->setStoreDiscount(request->new_discount);
    delete request;
}

/*
 * ------------------------------------------------------------------
 * buy_item_handler --
 *
 *      Handle a BuyItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
buy_item_handler(void *args)
{
    // TODO: Your code here.
    BuyItemReq* request = static_cast<BuyItemReq*>(args);

    std::cout << "Handling BuyItemReq: item_id - " << request->item_id << ", quantity - " << request->quantity << std::endl;
    globalStore->buyItem(request->item_id, request->quantity);
    delete request;
}

/*
 * ------------------------------------------------------------------
 * buy_many_items_handler --
 *
 *      Handle a BuyManyItemsReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
buy_many_items_handler(void *args)
{
    // TODO: Your code here.
    BuyManyItemsReq* request = static_cast<BuyManyItemsReq*>(args);

    std::cout << "Handling BuyManyItemsReq: items_count - " << request->items_count << std::endl;
    globalStore->buyManyItems(request->items, request->items_count);
    delete request;
}

/*
 * ------------------------------------------------------------------
 * stop_handler --
 *
 *      The thread should exit.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
stop_handler(void* args)
{
    // TODO: Your code here.
    std::terminate();
}

