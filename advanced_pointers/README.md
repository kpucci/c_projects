# COE 449 Lab 4: Advanced pointers

## Description
Pointers are super important when writing low-level programs! GET COMFORTABLE WITH EM!

In this lab, you’ll be writing a generic array-filtering function. This will make use of pointer arithmetic, function pointers, and pointer casting. This is an actually useful function! Filtering values out of an array is a very common operation.

## Predicates
“Predicate” is a common programming term which means “something that gives a yes-or-no answer.”

The filter function’s predicate must be a function which:

	* takes a const void* which points to a value from the array
	* returns an integer:
		* 0 for false (ignore the item)
		* nonzero for true (put the item in the output array)
			* (This is common in C, because it didn’t use to have bool.)

## Writing the predicate function
The less_than_50 function should interpret its parameter as a pointer to a float, and as the name implies, return a “true” (nonzero) value if it is less than 50.

Since the parameter is a const void*, you’ll have to cast the parameter to a different pointer type.

Have a look at how I wrote the comparison function in the qsort.c example to get an idea of how to write this.
