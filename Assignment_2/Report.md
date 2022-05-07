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
This problem is presented in the book Algorithm design by Kleinberg and Tardos.  
It is a prominent problem in computational biology.  
Formally the problem is: Given an RNA sequence, find the number of nucleotide pairs that can be formed, given the constraints which lead to highest stability.  
The constraints on folding of RNA strand into secondary structures are: 
1. There must be no sharp turns in the secondary structure. Every pair must have atleast 4 nucleotide separation between them.
2. The elements of a pair can contain either A, U together or C, G together in any order.
3. No base appears in more than one pair.
4. No pairings should cross each other. If (i, j) and (k, l) are two pairs then, we cannot have i < k < j < l.

A dynamic programming solution to this problem is given in the text mentioned above and we use the same algorithm in our solution.

<hr style="border:2px solid gray"> </hr>

### Discussion of the algorithm

First we take the input of RNA as a string sequence.

Then we apply dynamic programming algorithm over intervals. These intervals are selected based on the subproblems given by splitting of original problem.  
The original problem is split when a matching pairing of 2 nucleotides is found undere the given constraints (no sharp turn and nucleotide pairing rule).  
If the match is found then the subproblems are to the left and right of the matching just found, hence this algorithm by design takes care of the 3rd constraint of noncrossing.  

For each subproblem we have to choices:
 - j is not involved in the pairing
 - j is involved with some t < j-4 in pairing.
 
Thus we get a recurrence relation iterating over t ranging from i to j.

A vector of vectors of integers data structure is used to depict the dynamic programming matrix which calculates our solution. Building over smaller subproblems.  
<hr style="border:2px solid gray"> </hr>

### Timing analysis

Let's find out the time complexity of this pairing algorithm.
There are O(n<sup>2</sup>) subproblems to solve, and each subproblem takes O(n) time to get proessed.

So the final complexity comes out to be:

#### O( n*n*n )

**n** -> Number of nucleotides in the sequence.

<hr style="border:2px solid gray"> </hr>

### Test Case Report Table

#### Given test cases
I scraped the test cases from the website given in the assignment document.
The data was cleaned and all sequences extracted to a text file.
This table shows the running time and number of pairs of all those sequences.

|Length|Sequence                                                                |Pairs|Time Taken|
|------|------------------------------------------------------------------------|-----|----------|
|23    | UUGCGUCGCGUCGACGAAGUCGC                                                | 6   | 0.0234   |
|13    | GGCGCUCGGCGCC                                                          | 4   | 0.0041   |
|26    | UCUCUGGUUAGGAAACUAACUAGGGA                                             | 9   | 0.0344   |
|18    | CAACUUGUUGGCGAGAUC                                                     | 4   | 0.0109   |
|11    | GCCUGGAGCGC                                                            | 3   | 0.0023   |
|24    | GGGGACUGCGUUCGCGCUUUCCCC                                               | 8   | 0.039    |
|19    | GGCGAAGAACCGGGGAGCC                                                    | 4   | 0.0149   |
|9     | CUGUGAAUG                                                              | 2   | 0.0011   |
|6     | AAUAAA                                                                 | 0   | 0.0002   |
|6     | UUCUGC                                                                 | 0   | 0.0003   |
|12    | GGCGAAGAACCG                                                           | 2   | 0.003    |
|20    | CAGCAGGCUAAGGCAUGAAA                                                   | 4   | 0.0168   |
|18    | GGCGAGAACCGGGGAGCC                                                     | 4   | 0.0119   |
|15    | GGCAACCGGGGAGCC                                                        | 4   | 0.0066   |
|8     | UCGUGCGA                                                               | 2   | 0.0007   |
|13    | CUUGUCUGACACG                                                          | 3   | 0.0043   |
|7     | UGGCUGC                                                                | 1   | 0.0005   |
|11    | GGACUCCGGCC                                                            | 3   | 0.0021   |
|12    | GGACUCGACUCC                                                           | 3   | 0.0029   |
|27    | UGCUCCUAGUACGAGAGGACCGGAGUG                                            | 7   | 0.0473   |
|15    | GAGAGGACCGGAGUG                                                        | 2   | 0.0062   |
|12    | AUCCAGGUGCAC                                                           | 2   | 0.0029   |
|18    | CAAAGAAAUCCAGGUGCA                                                     | 3   | 0.0099   |
|13    | CCACUGGAGCUUC                                                          | 2   | 0.0038   |
|13    | CCACUGGAGUUUC                                                          | 2   | 0.0038   |
|12    | CCACUGGAGUUC                                                           | 2   | 0.003    |
|24    | GCUGGGACAACUAGACAUACAGUG                                               | 6   | 0.0439   |
|24    | GGAAGCUCUGACCGACCCCCAGCC                                               | 6   | 0.0314   |
|11    | GGACUGAAUCC                                                            | 3   | 0.0021   |
|12    | GGGCCCGGACCC                                                           | 3   | 0.0026   |
|12    | GGGUCCGGUCCC                                                           | 3   | 0.0028   |
|12    | GGGCCCGGUCCC                                                           | 3   | 0.0029   |
|23    | UUGCGUCACGCCGGCGAAGUCGC                                                | 6   | 0.0296   |
|43    | GGGUGUAGAAAAGUAAGGGAAACUCAAACCCCUUUCUACACCC                            | 15  | 0.1462   |
|67    | CGGGCUGACCGACCCCCCGAGUUCGCUCGGGGACAACUAGACAUACAGUAUGAAAAUACUGGCCCGC    | 23  | 0.6056   |
|74    | GGUCUGCUAUUUCUAUUUACGUUUUAGUACUCUGGAAACAGAAUCUACUAAAACAAGGCAAAAUGCCGUGUUUC| 25  | 0.8332   |
|80    | GGUCUGCUAUUUCUAUUUACGUUUUAGUACUCUGGAAACAGAAUCUACUAAAACAAGGCAAAAUGCCGUGUUUAUCUCGU| 26  | 1.0498   |


#### Own test cases  
This table shows some test cases which I made myself to test the solution.

|Length|Sequence                                                                |Pairs|Time Taken|
|------|------------------------------------------------------------------------|-----|----------|
|4     | ACGU                                                                   | 0   | 0.0002   |
|1     | A                                                                      | 0   | 0.0001   |
|2     | AU                                                                     | 0   | 0.0001   |
|3     | ACG                                                                    | 0   | 0.0001   |
|11    | ACGUUUUUUGC                                                            | 1   | 0.0019   |
|13    | ACGUUUGCAUCGC                                                          | 3   | 0.0037   |
|26    | CAGAUCGGCGAUACGAGCAUACUGCA                                             | 7   | 0.0442   |
|20    | ACGUUUUUUUUUUGCAUCGC                                                   | 3   | 0.0174   |
|37    | CGCUUCAUAUAAUCCUAAUGAUAUGGUUUGGGAGUUU                                  | 10  | 0.1066   |
|39    | CGCUUCAUAUAAUCCUAAUGAUAUGGUUUGGGAGUUUCU                                | 11  | 0.1832   |
|17    | ACAUGGAUGGCUCAUGU                                                      | 6   | 0.0099   |
|40    | GGGUGUAGAAAAGUAAGGGAAACUCAAAGCGACCUGUACG                               | 10  | 0.123    |
|43    | GGGUGUAGAAAAGUAAGGGAAACUCAAACCCCUUUCUACACCC                            | 15  | 0.1466   |
|42    | GGGUGUAGAAAAGUAAGGGAAACUCAAAGCGACCUGUACGCC                             | 12  | 0.1456   |
|71    | CGCUUCAUAUAAUCCUAAUGAUAUGGUUUGGGAGUUUCUACCAAGAGCCUUAAACUCUUGAUUAUGAAGUG| 25  | 0.7323   |
|284   | === OMMITED BECAUSE TOO BIG TO FIT IN HERE === | 104 | 43.8224  |



<hr style="border:2px solid gray"> </hr>
