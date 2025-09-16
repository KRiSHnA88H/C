// Project Euler Problem 6
// Find the difference between the sum of the squares of the first n natural
// numbers and the square of the sum of the first n natural numbers.
//
// Formula used:
// Sum of squares = n(n+1)(2n+1)/6
// Square of sum  = (n(n+1)/2)^2
//
// Example:
// Input : 100
// Output: 25164150

#include <stdio.h>

// Function to compute sum of squares using formula
long long sum_of_square(int n)
{
    return (long long)n * (n + 1) * (2 * n + 1) / 6;
}

// Function to compute square of sum using formula
long long square_of_sum(int n)
{
    long long sum = (long long)n * (n + 1) / 2;
    return sum * sum;
}

int main()
{
    int n;
    scanf("%d", &n);

    long long answer = square_of_sum(n) - sum_of_square(n);
    printf("%lld\n", answer);

    return 0;
}
//this calculates in o(1) ,enter the n till which u want to find. for our case (in question) its 100
