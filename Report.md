# REPORT

## Finding Intersection points

**Group Members:**

| Name                  | ID               |
| --------------------- | ---------------- |
| **Dinank Vashistha**  | **2018B5A71055H**|
| **Rohit Kushwah**     | **2018B5A71062H**|
| **Soumit Mishra**     | **2018B5A70429H**|

---

Directions to run the file are provided in [README.md](./README.md)

<hr style="border:2px solid gray"> </hr>

### Introduction to the problem

This paper discusses solving the line segment intersection problem for a set of given lines.  
Computing all the intersection points between all the line segments and reporting them.  
This problem is a very well known geometric problem with applications in many fields which contain a network of line segments like Geographical Information systems, Integrated circuit design, Computer aided engineering and Design, Graphics Procseing in video games.

An efficient solution to this problem was given by Bentley and Ottman in 1979, and we use the same algorithm in our solution.

We solve this problem using a custom implementation of two data structures from scratch. To simulate a line sweeping across the x-y plane and checking for line segments intersecting with the sweep line in an ordered way.

Both of these DS are implemented as _self balancing binary search trees_.

<hr style="border:2px solid gray"> </hr>

### Discussion of the algorithm

First data structure named **StatusStructure** stores the information for the current status of _line segments intersecting the sweep line_, stored in an _ordered way_.  
It is also used to access the neighbours of a given segment, to test for intersections.  
Second data structure with name **EventQueue** stores the unique potential intersection points labeled as _event points_ stored in an _ordered way_. Along with the line segments starting at these points.  
<b> _Event Points_ </b> are the endpoints of the line segments, and the intersection points which are computed as we go.  
It can pop the first event in queue and return it (left points at the same y coordinate are considered first.)

We imagine a horizontal sweep line <b>_l_</b> moving downwards over the plane.  
The sweep line halts at event points.  
While the sweep line moves we maintain the ordered sequence of segments intersected by it.  
When the sweep line halts at an event point the sequence of segments changes and, depending on the type of event point, we have to take several actions to update the status of line segments and detect intersections.

When the event point is an upper endpoint of a segment, there is a new segment intersecting the sweep line.
This segment must be tested for intersection against its two neighbors along the sweep line.
Only intersection points below the sweep line are important, the ones above the sweep line have been detected already.

When the event point is an intersection, the two segments that intersect change their order.  
Each of them gets (at most) one new neighbor against which it is tested for intersection.  
Again, only intersections below the sweep line are still interesting.

When the event point is the lower endpoint of a segment, its two neighbors now become adjacent and must be tested for intersection. If they intersect below the sweep line, then their intersection point is an event point.

After we have swept the whole plane, meaning that after we have treated the last event point we have computed all intersection points.  
This is guaranteed by the following invariant, which holds at any time during the plane sweep: **all intersection points above the sweep line have been computed correctly.**

<hr style="border:2px solid gray"> </hr>

### Timing analysis

Let's find out the time complexity of this sweep line algorithm.
First the algorithm constructs the EventQueue as self balacing binary search tree, hence it takes **O(n\*logn)** time.  
Initializing the StatusStructure take constant time.

Then the line sweeps the plane and handles the event points, for handling event points we perform a few operations on the event queue.

1. Deleting the event from the queue. Takes **O(logn) time)**.
2. Search for new event and inserting it into queue (atmost 2 insertions). Takes **O(logn) time)**.
3. Then performing neighbour finding as well. Takes **O(logn) time)**.

Here to process all the intersection points, step 2 will be of the order of I (number of intersection points). Hence, the number of operations would be linear in number let's say they are <b>_m_</b>.  
Now if this <b>_m_</b> is the sum of all event points then the running time of the algorithm would be **O(m\*logn)**.

It is clear that m = n + I. Where n is the number of lines (or endpoints), and I is the number of intersection points in the output.

So, this is an output sensitive algorithm.
And the final complexity comes out to be:

#### O( n\*logn + I\*logn )

| N   | I     | Time Taken in ms |
| --- | ----- | ---------------- |
| 10  | 11    | 2.47 ms          |
| 10  | 25    | 5.53 ms          |
| 20  | 100   | 16.75 ms         |
| 200 | 10000 | 1963.44 ms       |
| 9   | 6     | 2.70 ms          |

**N** -> Number of lines  
**I** -> Number of intersections

<hr style="border:2px solid gray"> </hr>

### Visualizations

#### N = 10 and I = 11

#### Time taken : 2.47ms

<img src="11points.png" alt="drawing" width="1500"/>

#### Points Obtained

##### (2, 3) (2.5, 2.5) (1, 2) (2, 2) (3, 2) (1, 1.5) (1.25, 1.25) (1, 1) (1.5, 1) (2, 1) (0, 0)

---

#### N = 10 and I = 25

#### Time taken : 5.53ms

<img src="25points.png" alt="drawing" width="1500"/>

#### Points Obtained:

##### (1, 5) (2, 5) (3, 5) (4, 5) (5, 5) (1, 4) (2, 4) (3, 4) (4, 4) (5, 4) (1, 3) (2, 3) (3, 3) (4, 3) (5, 3) (1, 2) (2, 2) (3, 2) (4, 2) (5, 2) (1, 1) (2, 1) (3, 1) (4, 1) (5, 1)

---

#### N = 20 and I = 100

#### Time taken : 16.75ms

<img src="100points.png" alt="drawing" width="1500"/>

#### Points Obtained:

##### (1, 10) (2, 10) (3, 10) (4, 10) (5, 10) (6, 10) (7, 10) (8, 10) (9, 10) (10, 10) (1, 9) (2, 9) (3, 9) (4, 9) (5, 9) (6, 9) (7, 9) (8, 9) (9, 9) (10, 9) (1, 8) (2, 8) (3, 8) (4, 8) (5, 8) (6, 8) (7, 8) (8, 8) (9, 8) (10, 8) (1, 7) (2, 7) (3, 7) (4, 7) (5, 7) (6, 7) (7, 7) (8, 7) (9, 7) (10, 7) (1, 6) (2, 6) (3, 6) (4, 6) (5, 6) (6, 6) (7, 6) (8, 6) (9, 6) (10, 6) (1, 5) (2, 5) (3, 5) (4, 5) (5, 5) (6, 5) (7, 5) (8, 5) (9, 5) (10, 5) (1, 4) (2, 4) (3, 4) (4, 4) (5, 4) (6, 4) (7, 4) (8, 4) (9, 4) (10, 4) (1, 3) (2, 3) (3, 3) (4, 3) (5, 3) (6, 3) (7, 3) (8, 3) (9, 3) (10, 3) (1, 2) (2, 2) (3, 2) (4, 2) (5, 2) (6, 2) (7, 2) (8, 2) (9, 2) (10, 2) (1, 1) (2, 1) (3, 1) (4, 1) (5, 1) (6, 1) (7, 1) (8, 1) (9, 1) (10, 1)

---

#### N = 200 and I = 10,000

#### Time taken : 1963.44ms

<img src="10000points.png" alt="drawing" width="1500"/>

#### Points Obtained:

##### Omitting the points here because they number 10,000.

---

#### N = 10 and I = 11

#### Time taken : 2.70ms

<img src="randompoints.png" alt="drawing" width="1500"/>

#### Points Obtained:

##### (3.43537, 9.04172) (3.18462, 5.53846) (3.21216, 5.4696) (3.80183, 3.99543) (3.93939, 3.65152) (3.49107, 3.29286)

<hr style="border:2px solid gray"> </hr>

### Some issues faced and solved while coding

Intersection points were not getting picked up, because they were on the line but floating point precision messed up the equation testing for it.
Solved this issue by replacing the inequalities by a threshold interval.

A lot of debugging time was spent in figuring out how the null nodes behaved and to handle them. Because we can not set a node's value to null or nullptr.
Finally created a new empty constructor for each class to represnt null nodes. Along with custom function to check for null nodes.

<hr style="border:2px solid gray"> </hr>
