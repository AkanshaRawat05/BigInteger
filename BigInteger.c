#include "BigInteger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node *createNode(int data)
{
  struct node *newNode = (struct node *)malloc(sizeof(struct node));
  if (newNode)
  {
    newNode->data = data;
    newNode->next = NULL;
  }
  return newNode;
}

struct BigInteger initialize(char *str)
{
  struct BigInteger bigInt;
  bigInt.length = 0;
  bigInt.list = NULL;

  int len = strlen(str);
  int i = 0;

  // Add digits to the linked list in reverse order
  for (; i < len; i++)
  {
    if (str[i] < '0' || str[i] > '9')
    {
      printf("Error: Invalid character in the input string.\n");
      exit(1);
    }
    int digit = str[i] - '0';
    struct node *newNode = createNode(digit);
    if (!newNode)
    {
      printf("Error: Memory allocation failed.\n");
      exit(1);
    }
    newNode->next = bigInt.list;
    bigInt.list = newNode;
    bigInt.length++;
  }

  // Handle the case of an empty list (input was "0")
  if (bigInt.length == 0)
  {
    bigInt.length = 1;
    bigInt.list = createNode(0);
  }

  return bigInt;
}

// Function to add two BigIntegers
struct BigInteger add(struct BigInteger num1, struct BigInteger num2)
{
  struct BigInteger result;
  result.length = 0;
  result.list = NULL;

  int carry = 0, sum = 0;
  struct node *ptr1 = num1.list;
  struct node *ptr2 = num2.list;
  while (ptr1 || ptr2 || carry)
  {
    sum = carry;
    if (ptr1)
    {
      sum += ptr1->data;
      ptr1 = ptr1->next;
    }
    if (ptr2)
    {
      sum += ptr2->data;
      ptr2 = ptr2->next;
    }
    carry = sum / 10;
    struct node *newNode = createNode(sum % 10);
    if (result.list == NULL)
    {
      result.list = newNode;
    }
    else
    {
      struct node *current = result.list;
      while (current->next)
      {
        current = current->next;
      }
      current->next = newNode;
    }

    result.length++;
  }
  return result;
}

struct BigInteger sub(struct BigInteger num1, struct BigInteger num2)
{
  struct BigInteger result;
  result.length = 0;
  result.list = NULL;

  int borrow = 0, diff = 0;
  struct node *ptr1 = num1.list;
  struct node *ptr2 = num2.list;
  while (ptr1 || ptr2)
  {
    diff = borrow;

    if (ptr1)
    {
      diff += ptr1->data;
      ptr1 = ptr1->next;
    }
    if (ptr2)
    {
      diff -= ptr2->data;
      ptr2 = ptr2->next;
    }
    if (diff < 0)
    {
      diff += 10;
      borrow = -1;
    }
    else
      borrow = 0;

    struct node *newNode = createNode(diff);
    if (result.list == NULL)
    {
      result.list = newNode;
    }
    else
    {
      struct node *current = result.list;
      while (current->next)
      {
        current = current->next;
      }
      current->next = newNode;
    }

    result.length++;
  }
  struct node *current = result.list;
  struct node *lastNonZero = NULL;
  while (current)
    {
      if (current->data != 0)
	{
	  lastNonZero = current;
	}
      current = current->next;
    }

  if (lastNonZero)
    {
      lastNonZero->next = NULL;
    }
  else
    {
      // If no non-zero digits are left, add a single zero.
      result.list = createNode (0);
    }
  return result;
}

// Function to multiply two BigIntegers
struct BigInteger
mul(struct BigInteger num1, struct BigInteger num2)
{
  struct BigInteger result;
  result.length = 1;
  result.list = createNode(0);

  struct node *ptr2 = num2.list;
  int position = 0; // Track the position to append zeros

  while (ptr2)
  {
    int value = ptr2->data;
    struct BigInteger product;
    product.length = 0;
    product.list = NULL;

    // Multiply num1 by a single digit from num2
    struct node *ptr1 = num1.list;
    int carry = 0;

    while (ptr1)
    {
      int total = (ptr1->data * value) + carry;
      carry = total / 10;
      struct node *newNode = createNode(total % 10);

      if (product.list == NULL)
      {
        product.list = newNode;
      }
      else
      {
        struct node *current = product.list;
        while (current->next)
        {
          current = current->next;
        }
        current->next = newNode;
      }

      ptr1 = ptr1->next;
    }

    if (carry > 0)
    {
      struct node *newNode = createNode(carry);
      if (product.list == NULL)
      {
        product.list = newNode;
      }
      else
      {
        struct node *current = product.list;
        while (current->next)
        {
          current = current->next;
        }
        current->next = newNode;
      }
    }

    // Append zeros based on position
    for (int i = 0; i < position; i++)
    {
      struct node *zeroNode = createNode(0);
      zeroNode->next = product.list;
      product.list = zeroNode;
    }

    // Add the product to the result
    result = add(result, product);

    ptr2 = ptr2->next;
    position++;
  }

  return result;
}

void freeList(struct node *head)
{
  while (head != NULL)
  {
    struct node *temp = head;
    head = head->next;
    free(temp); // Deallocate the memory for the node
  }
}

struct node *reverseList(struct node *head)
{
  struct node *prev = NULL;
  struct node *current = head;
  struct node *next = NULL;
  while (current != NULL)
  {
    next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }
  return prev;
  // Return the new head of the reversed list
}

struct node *copyList(struct node *head)
{
  struct node *newHead = NULL;
  struct node *current = head;
  struct node *tail = NULL;

  while (current != NULL)
  {
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = current->data;
    newNode->next = NULL;

    if (tail == NULL)
    {
      newHead = newNode;
      tail = newNode;
    }
    else
    {
      tail->next = newNode;
      tail = newNode;
    }

    current = current->next;
  }

  return newHead;
}

int compare(struct BigInteger num1, struct BigInteger num2)
{
  struct node *ptr1 = num1.list;
  struct node *ptr2 = num2.list;
  int length1 = 0;
  int length2 = 0;

  while (ptr1 != NULL)
  {
    length1++;
    ptr1 = ptr1->next;
  }

  while (ptr2 != NULL)
  {
    length2++;
    ptr2 = ptr2->next;
  }

  if (length1 < length2)
  {
    return -1;
  }
  else if (length1 > length2)
  {
    return 1;
  }
  else
  {

    struct node *reversedCopy1 = copyList(num1.list);
    ptr1 = reverseList(reversedCopy1);
    

    struct node *reversedCopy2 = copyList(num2.list);
    ptr2 = reverseList(reversedCopy2); // Reverse the copied list temporarily
    while (ptr1 != NULL)
    {
      if (ptr1->data < ptr2->data)
      {

        freeList(reversedCopy1); // Free the copied list
        freeList(reversedCopy2); // Free the copied list
        return -1;
      }
      else if (ptr1->data > ptr2->data)
      {
        freeList(reversedCopy1); // Free the copied list
        freeList(reversedCopy2); // Free the copied list
        return 1;
      }
      ptr1 = ptr1->next;
      ptr2 = ptr2->next;
    }
    freeList(reversedCopy1);
    freeList(reversedCopy2);
    // Free the copied list
    // Free the copied list
    return 0; // Both lists are equal
  }
}

// Function to divide two BigIntegers (integer division)
struct BigInteger
mod(struct BigInteger dividend, struct BigInteger divisor)
{
  struct BigInteger zero;
  zero.length = 1;
  zero.list = createNode(0);

  if (compare(divisor, zero) == 0)
  {
    printf("Error: Division by zero.\n");
    exit(1);
  }

  struct BigInteger remainder = dividend;

  while (compare(remainder, divisor) >= 0)
  {
    remainder = sub(remainder, divisor);
  }

  return remainder;
}

// Function to divide two BigIntegers
struct BigInteger div1(struct BigInteger dividend, struct BigInteger divisor)
{
  struct BigInteger quotient;
  quotient.length = 1;
  quotient.list = createNode(0);

  struct BigInteger one;
  one.length = 1;
  one.list = createNode(1);

  struct BigInteger zero;
  zero.length = 1;
  zero.list = createNode(0);

  if (compare(divisor, zero) == 0)
  {
    printf("Error: Division by zero.\n");
    exit(1);
  }

  if (compare(dividend, zero) == 0)
  {
    return zero;
  }

  struct BigInteger rem = dividend;

  while (compare(rem, divisor) >= 0)
  {
    rem = sub(rem, divisor);

    quotient = add(quotient, one);
  }

  return quotient;
}

// Recursive function to display a BigInteger in reverse order
void displayReverse(struct node *current)
{
  if (current == NULL)
  {
    return;
  }
  displayReverse(current->next);
  printf("%d", current->data);
}

// Function to display a BigInteger in reverse order
void display(struct BigInteger bigInt)
{
  if (bigInt.length == 0)
  {
    printf("0\n");
    return;
  }

  displayReverse(bigInt.list); // Start the recursive display
  printf("\n");
}