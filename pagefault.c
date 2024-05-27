------------------------------------
FIFO
------------------------
#include <stdio.h>
#include <stdbool.h>

#define CAPACITY 4

// Function to find page faults using FIFO
int pageFaults(int pages[], int n) {
    // To represent set of current pages.
    // We use an array to quickly check if a page is present in set or not
    bool s[CAPACITY];

    // To store the pages in FIFO manner
    int indexes[CAPACITY];

    // Initialize all pages as not present in set
    for (int i = 0; i < CAPACITY; i++) {
        s[i] = false;
    }

    // Start from initial page
    int page_faults = 0;
    int front = 0, rear = 0;
    for (int i = 0; i < n; i++) {
        // Check if the set can hold more pages
        if (rear < CAPACITY) {
            // Insert it into set if not present already which represents page fault
            if (!s[pages[i]]) {
                // Insert the current page into the set
                s[pages[i]] = true;

                // Increment page fault
                page_faults++;

                // Push the current page into the queue
                indexes[rear++] = pages[i];
            }
        } else {
            // Check if current page is not already present in the set
            if (!s[pages[i]]) {
                // Store the first page in the queue to be used to find and
                // erase the page from the set
                int val = indexes[front];

                // Remove the first page from the queue
                front = (front + 1) % CAPACITY;

                // Remove the indexes page from the set
                s[val] = false;

                // Insert the current page in the set
                s[pages[i]] = true;

                // Push the current page into the queue
                indexes[rear] = pages[i];
                rear = (rear + 1) % CAPACITY;

                // Increment page faults
                page_faults++;
            }
        }
    }

    return page_faults;
}

// Driver code
int main() {
    int pages[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    int n = sizeof(pages) / sizeof(pages[0]);
    printf("%d", pageFaults(pages, n));
    return 0;
}
--------------------------------------
OPTIMAL
-----------------------------
#include <stdio.h>
#include <stdbool.h>

// Function to check whether a page exists in a frame or not
bool search(int key, int fr[], int fn) {
    for (int i = 0; i < fn; i++) {
        if (fr[i] == key) {
            return true;
        }
    }
    return false;
}

// Function to find the frame that will not be used recently in future after given index in pg[0..pn-1]
int predict(int pg[], int fr[], int pn, int fn, int index) {
    // Store the index of pages which are going to be used recently in future
    int res = -1, farthest = index;
    for (int i = 0; i < fn; i++) {
        int j;
        for (j = index; j < pn; j++) {
            if (fr[i] == pg[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }

        // If a page is never referenced in future, return it.
        if (j == pn) {
            return i;
        }
    }

    // If all of the frames were not in future, return any of them, we return 0. Otherwise we return res.
    return (res == -1) ? 0 : res;
}

void optimalPage(int pg[], int pn, int fn) {
    // Create an array for given number of frames and initialize it as empty.
    int fr[fn];
    int fr_size = 0;

    // Traverse through page reference array and check for miss and hit.
    int hit = 0;
    for (int i = 0; i < pn; i++) {
        // Page found in a frame: HIT
        if (search(pg[i], fr, fr_size)) {
            hit++;
            continue;
        }

        // Page not found in a frame: MISS

        // If there is space available in frames.
        if (fr_size < fn) {
            fr[fr_size++] = pg[i];
        } else {
            int j = predict(pg, fr, pn, fn, i + 1);
            fr[j] = pg[i];
        }
    }
    printf("No. of hits = %d\n", hit);
    printf("No. of misses = %d\n", pn - hit);
}

// Driver Function
int main() {
    int pg[] = { 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2 };
    int pn = sizeof(pg) / sizeof(pg[0]);
    int fn = 4;
    optimalPage(pg, pn, fn);
    return 0;
}
-----------------------------
LRU
------------------------
#include <stdio.h>
#include <limits.h>

// Function to find page faults using indexes
int pageFaults(int pages[], int n, int capacity)
{
    // To represent set of current pages. We use
    // an array to simulate set-like behavior
    int s[capacity];
    // To store least recently used indexes
    // of pages.
    int indexes[capacity];

    // Start from initial page
    int page_faults = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        int j;
        // Check if the set can hold more pages
        if (i < capacity)
        {
            // Insert it into set if not present
            // already which represents page fault
            int found = 0;
            for (j = 0; j < i; j++)
            {
                if (s[j] == pages[i])
                {
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                s[i] = pages[i];

                // increment page fault
                page_faults++;
            }

            // Store the recently used index of
            // each page
            indexes[i] = i;
        }

        // If the set is full then need to perform lru
        // i.e. remove the least recently used page
        // and insert the current page
        else
        {
            // Check if current page is not already
            // present in the set
            int found = 0;
            for (j = 0; j < capacity; j++)
            {
                if (s[j] == pages[i])
                {
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                // Find the least recently used pages
                // that is present in the set
                int lru = INT_MAX, val;
                for (j = 0; j < capacity; j++)
                {
                    if (indexes[j] < lru)
                    {
                        lru = indexes[j];
                        val = s[j];
                    }
                }

                // Remove the indexes page
                for (j = 0; j < capacity; j++)
                {
                    if (s[j] == val)
                    {
                        s[j] = pages[i];
                        break;
                    }
                }

                // Increment page faults
                page_faults++;
            }

            // Update the current page index
            for (j = 0; j < capacity; j++)
            {
                if (s[j] == pages[i])
                {
                    indexes[j] = i;
                    break;
                }
            }
        }
    }

    return page_faults;
}

// Driver code
int main()
{
    int pages[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    int n = sizeof(pages) / sizeof(pages[0]);
    int capacity = 4;
    printf("%d", pageFaults(pages, n, capacity));
    return 0;
}
