//
///**
// * Division of var1 by var2, storing result in var1.
// * Uses a binary search on the dividend space.
// * Runs in O(n^2 log n), where n is the digit length of dividend.
// * This is slower than the naive algorithm
// *
// * @param var1 Dividend, also the output
// * @param var2 Divisor
// * @return Computation status
// */
//COMPUTE_STATUS do_div(longint_t *var1, const longint_t *var2)
//{
//    // check for dividing by zero
//    if (long_zero(var2))
//    {
//        return STATUS_ZERODIV;
//    }
//
//    // estimate lower and upper bounds
//    longint_t lower = {{INT_ONE, INT_ZERO}, 1}, upper = *var1;
//    int lower_digits = var1->length - var2->length;
//    do_mul10(&lower, MAX(0, lower_digits - 1));
//
//    longint_t difference, product, mid;
//    longint_t *upper_ptr = &upper, *lower_ptr = &lower;
//    COMPUTE_STATUS status;
//
//    for (;;)
//    {
//        // compute difference between upper and lower bound
//        difference = *upper_ptr;
//        do_sub_positive(&difference, lower_ptr);
//
//        // termination step when the difference in range [0..9]
//        if (difference.length <= 1)
//            break;
//
//        // compute steps by dividing the difference by 10
//        do_div10(&difference);
//
//        // then multiplying the step by 5, to reach a binary search difference
//        mid = *lower_ptr;
//        for (int j = 0; j < INT_BASE / 2; ++j)
//        {
//            status = do_plus(&mid, &difference);
//            if (status == STATUS_OVERFLOW)
//                goto FAIL_HIGH;
//        }
//
//        // compute mid * var2
//        product = mid;
//        status = do_mult(&product, var2);
//        if (status == STATUS_OVERFLOW)
//            goto FAIL_HIGH;
//
//        // if product equal or greater than var1, we've overdone it
//        if (do_order(&product, var1) >= 0)
//        {
//            FAIL_HIGH:;
//            *upper_ptr = mid;
//
//            // exact division early return
//            if (do_order(&product, var1) == 0)
//                break;
//        }
//        else // we're under
//        {
//            *lower_ptr = mid;
//        }
//    }
//
//    // linear search through [0..9]
//    product = *upper_ptr;
//    do_mult(&product, var2);
//    for (int i = 0; i < INT_BASE; ++i)
//    {
//        // return when we find a truncating result
//        if (do_order(&product, var1) <= 0)
//        {
//            break;
//        }
//
//        do_sub_positive(&product, var2);
//        do_dec(upper_ptr);
//    }
//
//    // upper is the solution
//    *var1 = *upper_ptr;
//    return STATUS_OK;
//}