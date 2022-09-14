#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
typedef map<int, bool> temp1;
typedef map<int, vector<int>> temp2;

/**
 * @brief MEMOIZATION RECIPE
 * 1. Make is work correctly.
 *  -   visualize problem as a tree
 *  -   implement the tree using recursion
 *      -   leaves as base case
 *  -   test it (SHOULD GIVE CORRECT ANSWER EVEN IF SLOW)
 * 2. Make it efficient. 
 *  -   add a memo object (hash map, unordered/ordered set, tuple, etc)
 *  -   add a base case to return the memoized values
 *  -   store return values into the memo
 */

// turns vector into string for printing
template<typename T>
string printVect(vector<T> vect){
    string result = "";
    for(int i = 0; i < vect.size()-1; i++){
        result += to_string(vect[i]) + ", ";
    }
    result += to_string(vect[vect.size()-1]);
    return result; 
}

/* 
 *  Dynamic Programming Part 1: Memoization
 *  Problem 1:
 *      Write a function `fib(n)` that takes in a number as an argument.
 *      The function should return the n-th number of the Fibonacci sequence. 
 *  Ex:
 *      -   1st and 2nd numbers of the sequence are 1
 *      -   to generate the next number, we sum the previous 2
 *      -   fib(n): 1, 1, 2, 3, 5, 8, 13, 21, 34, . . .
 *      -   n:      1, 2, 3, 4, 5, 6, 7,  8,  9
 */
int fib(int n, std::map<int, int>& memo){
    //  recurrsive implmentation:
    //      issues: -   not efficient for larger values of n
    //              -   This will build out a tree of branching possibilities which then repeats calculations for subsequent values of n repeatedly
    //              -   time: O(2^n) space: O(n)

    // if(n <= 2) return 1;
    // return fib(n-1) + fib(n-2); 

    /*  optimized recursize implementation using memoization (memorizing previous cases):
     *      -   using a hashmap, store the values of subsequent results needed to calculate n
     *      -   if current val of n exists in map, just return the value and skip branching
     *      -   time: O(n) space: O(n)
     */
    if(n <= 2) return 1;
    if(memo.find(n) != memo.end()) return memo[n];
    int sum = fib(n-1, memo) + fib(n-2, memo);
    memo[n] = sum;
    return sum; 
}

/**
 * @brief Problem 1.2: grid traveler
 * Say that you are a traveler on a 2D grid. You being in the top-left corner and your goal is to travel to the bottom-right corner.
 * How many different ways can you travel to the goal on a grid with dimensions m * n? 
 * @cond You may only move down or right
 * Inputs: int m, int n
 * @return int 
 */
uint32_t gridTraveler(int m, int n, map<vector<int>, uint32_t>& memo){
    // WITHOUT MEMOIZATION: time -> O(2^n+m), space -> O(max(m+n))
    // int sum = 0;
    // if(m == 0 || n == 0) return 0;
    // if(m == 1 || n == 1) return 1;
    // sum += gridTraveler(m-1, n) + gridTraveler(m, n-1);
    // return sum; 

    // WITH MEMOIZATION: time -> O(m * n), space -> O(m + n)
    uint32_t sum = 0;
    if(m == 0 || n == 0) return 0;
    if(m == 1 || n == 1) return 1;
    if(memo.find({m, n}) != memo.end()) return memo[{m,n}];
    else if(memo.find({n,m}) != memo.end()) return memo[{n,m}];
    sum += gridTraveler(m-1, n, memo) + gridTraveler(m, n-1, memo);
    memo[{m, n}] = sum; 
    memo[{n, m}] = sum; 
    return sum; 
}

/**
 * @brief Problem 1.3: can Sum
 * Write a function `canSum(targetSum, numbers)` that takes in a targetSum and an array of numbers as arguments.
 * The function should return a boolean indicating whether or not it is possible to generate the targetSum using numbers from the array.
 * You may use an element of the array as many times as needed.
 * You may assume that all input nubers are nonnegative. 
 * 
 * @return bool
 */
bool canSum(int targetSum, vector<int> numbers, temp1 memo = temp1()){
    // BRUTE FORCE: time -> O(n^m), space -> O(m) where n = size of numbers, and m = targetSum 
    // if(targetSum == 0) return true; 
    // for(int i = 0; i < numbers.size(); i++){
    //     int newTarget = targetSum - numbers[i];
    //     if(newTarget >= 0 && canSum(newTarget, numbers) == true) return true; 
    // }
    // return false; 

    // MEMOIZED SOLUTION: time -> O(m*n), space -> O(m) where m = target sum, n = array length
    if(memo.find(targetSum) != memo.end()) return memo[targetSum];
    if(targetSum == 0) return true;
    if(targetSum < 0) return false;

    for(auto num : numbers){
        int newTarget = targetSum - num;
        if(newTarget >= 0 && canSum(newTarget, numbers, memo) == true){
            memo[targetSum] = true;
            return true; 
        }
    }
    memo[targetSum] = false; 
    return false; 
}

/**
 * Write a function `howSum(targetSum, numbers)` that takes in a targetSum and an array of numbers as arguments. 
 * 
 * The function should return an array containing any combinations of elements that add up to exactly the targetSum. 
 * If there is no combination, then return null. 
 * 
 * If there are multiple combinations possible, you may return any single one. 
 * 
 * @return vector<int>
 */
vector<int> howSum(int targetSum, vector<int> nums, temp2 memo = temp2()){
    // BRUTE FORCE: m = target sum, n = nums.size() -> time: O(n^m) -> space: O(m)
    // if (targetSum == 0) return {};
    // if (targetSum < 0) return {-1};


    // for (int i = 0; i < nums.size(); i++){
    //     int leftover = targetSum - nums[i];
    //     vector<int> result = howSum(leftover, nums);
    //     if(result.empty() || result[0] != -1){
    //         result.push_back(nums[i]); 
    //         return result; 
    //     }
    // }
    // return {-1}; 

    // MEMOIZATION: m = target sum, n = nums.size() -> time: O(n * m) -> space: O(m^m)
    if (memo.find(targetSum) != memo.end()) return memo[targetSum]; 
    if (targetSum == 0) return {};
    if (targetSum < 0) return {-1};


    for (int i = 0; i < nums.size(); i++){
        int leftover = targetSum - nums[i];
        vector<int> result = howSum(leftover, nums, memo);
        if(result.empty() || result[0] != -1){
            result.push_back(nums[i]); 
            memo[targetSum] = result; 
            return memo[targetSum]; 
        }
    }
    memo[targetSum] = {-1}; 
    return {-1}; 
}

/**
 * Write a function `bestSum(targetSum, numbers)` that takes in a targetSum and an array of numbers as arguments. 
 * 
 * The function should return an array containing any smallest combination of elements that add up to exactly the targetSum. 
 * If there is no combination, then return null. 
 * 
 * If there are multiple smallest combinations possible, you may return any single one. 
 * 
 * @return vector<int>
 */
vector<int> bestSum(int targetSum, vector<int> nums, map<int, vector<int>>& memo){
    if (memo.find(targetSum) != memo.end()) return memo[targetSum]; 
    if (targetSum == 0) return {};
    if (targetSum < 0) return {-1};

    vector<int> shortest = {-1};

    for (int i = 0; i < nums.size(); i++){
        int leftover = targetSum - nums[i];
        vector<int> result = bestSum(leftover, nums, memo);
        if(result.empty() || result[0] != -1){
            result.push_back(nums[i]); 
            if (shortest[0] == -1 || result.size() < shortest.size()){
                shortest = result; 
            }
        }
    }
    memo[targetSum] = shortest; 
    return shortest; 
}
vector<int> bestSum(int targetSum, vector<int> nums){
    // BRUTE FORCE: m = target sum, n = nums.size() -> time: O(n^m) -> space: O(m^2)


    // MEMOIZATION: m = target sum, n = nums.size() -> time: O(n*m) -> space: O(m^2)
    map<int, vector<int>> mp; 
    return bestSum(targetSum, nums, mp); 
}

/**
 * Write a function called canConstruct(target, wordBank) that accepts a target string and an array of strings
 * 
 * The function should return a boolean indicating whtether or not the target can be constructed by concatenating elements of the wordbank
 * 
 * You may resuse elements of the word bank as many times as needed
 * 
 * Brute: time -> O(n^m * m), space -> O(m^2)
 * 
 * Memo: time -> O(n*m^2), space -> O(m^2)
 * 
 * @return bool
 */
bool canConstruct(string target, vector<string> wordBank, map<string, bool>& memo){
    if(memo.find(target) != memo.end()) return memo[target]; 
    if(target == "") return true;

    for(int i = 0; i < wordBank.size(); i++){
        if(target.find(wordBank[i]) == 0){
            string suffix = target;
            suffix.erase(0, wordBank[i].size());
            if (canConstruct(suffix, wordBank, memo)){
                memo[suffix] = true;
                return true;
            }
            // memo[suffix] = false; 
        }
    }
    memo[target] = false; 
    return false; 
}
bool canConstruct(string target, vector<string> wordBank){
    map<string, bool> mp;
    return canConstruct(target, wordBank, mp); 
}

/**
 * Write a function countConstruct(target, wordbank) that accepts a target string and an array of strings
 * 
 * The function should return the number of ways that the target can be constructed by concatenating elements
 * of the wordbank array.
 * 
 * you may reuse elements of the word bank as many times as needed
 * 
 * @return int 
 */
int countConstruct(string target, vector<string> wordBank, map<string, int>& memo){
    if(memo.find(target) != memo.end()) return memo[target];
    if(target == "") return 1;

    int tot = 0;

    for(int i = 0; i < wordBank.size(); i++){
        if(target.find(wordBank[i]) == 0){
            int count = countConstruct(target.erase(0, wordBank[i].size()), wordBank, memo); 
            tot += count; 
            if(count > 0){
                memo[target] = count + 1;
                return count;
            }
        }
    }
    memo[target] = 0;
    return 0; 
}
int countConstruct(string target, vector<string> wordBank){
    map<string, int> mp;
    return countConstruct(target, wordBank, mp); 
}

int main(){
    if (__cplusplus == 201703L) std::cout << "C++17\n";
    else if (__cplusplus == 201402L) std::cout << "C++14\n";
    else if (__cplusplus == 201103L) std::cout << "C++11\n";
    else if (__cplusplus == 199711L) std::cout << "C++98\n";
    else std::cout << "pre-standard C++\n";

    //  P1.1: Fibonacci Sequence
    // map<int,int> temp;
    // cout << "P1.1: Fibonacci Sequence Reurrsive\n";
    // cout << "\tEx1: n = 6, fib(n) = " << fib(6, temp) << endl;
    // cout << "\tEx2: n = 7, fib(n) = " << fib(7, temp) << endl; 
    // cout << "\tEx3: n = 8, fib(n) = " << fib(8, temp) << endl; 
    // cout << "\tEx4: n = 50, fib(n) = " << fib(50, temp) << endl; 

    //  P1.2: 2D Grid Traveler
    // map<vector<int>, uint32_t> temp; 
    // cout << "Grid Traveler (0,0): " << gridTraveler(0,0, temp) << endl; 
    // cout << "Grid Traveler (1,1): " << gridTraveler(1,1, temp) << endl; 
    // cout << "Grid Traveler (2,2): " << gridTraveler(2,2, temp) << endl; 
    // cout << "Grid Traveler (3,2): " << gridTraveler(3,2, temp) << endl; 
    // cout << "Grid Traveler (3,3): " << gridTraveler(3,3, temp) << endl;
    // cout << "Grid Traveler (18,18): " << gridTraveler(18,18, temp) << endl;

    //  P1.3: canSum
    // map<int, bool> temp; 
    // cout << "CanSum: t = 7, nums = [5, 3, 4, 7]:    " << boolalpha << canSum(7, {5, 3, 4, 7}) << endl; 
    // cout << "CanSum: t = 7, nums = [2, 4]:          " << boolalpha << canSum(7, {2, 4}) << endl; 
    // cout << "CanSum: t = 8, nums = [2, 3, 5]:       " << boolalpha << canSum(8, {2, 3, 5}) << endl; 
    // cout << "CanSum: t = 300, nums = [7, 14]:       " << boolalpha << canSum(300, {7, 14}) << endl; 
    // cout << "CanSum: t = 1400, nums = [7, 14]:      " << boolalpha << canSum(1400, {7, 14}) << endl;

    // P1.4: howSum
    // cout << "howSum: t = 7, nums = [2, 3]:            " << printVect(howSum(7, {2, 3})) << endl; 
    // cout << "howSum: t = 7, nums = [5, 3, 4, 7]:      " << printVect(howSum(7, {5, 3, 4, 7})) << endl; 
    // cout << "howSum: t = 7, nums = [2, 4]:            " << printVect(howSum(7, {2, 4})) << endl; 
    // cout << "howSum: t = 8, nums = [2, ,3, 5]:        " << printVect(howSum(8, {2, 3, 5})) << endl;
    // cout << "howSum: t = 300, nums = [7, 14]:         " << printVect(howSum(300, {7, 14})) << endl;
    
    // P1.5 bestSum
    // cout << "bestSum: t = 7, nums = [5, 3, 4, 7]:      " << printVect(bestSum(7, {5, 3, 4, 7})) << endl; 
    // cout << "bestSum: t = 8, nums = [2, 3, 5]:         " << printVect(bestSum(8, {2, 3, 5})) << endl; 
    // cout << "bestSum: t = 8, nums = [1, 4, 5]:         " << printVect(bestSum(8, {1, 4, 5})) << endl;
    // cout << "bestSum: t = 100, nums = [1, 2, 5, 25]:   " << printVect(bestSum(100, {1, 2, 5, 25})) << endl;

    // P1.6 canConstruct
    // cout << "canConstruct 1:    " << boolalpha << canConstruct("abcdef", {"ab", "abc", "cd", "def", "abcd"}) << endl; //  true
    // cout << "canConstruct 2:    " << boolalpha << canConstruct("skateboard", {"bo", "rd", "ate", "t", "ska", "sk", "boar"}) << endl; // false
    // cout << "canConstruct 3:    " << boolalpha << canConstruct("enterapotentpot", {"a", "p", "ent", "enter", "ot", "o", "t"}) << endl; // true
    // cout << "canConstruct 4:    " << canConstruct("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeef", {"e", "ee", "eee", "eeee", "eeeee"}) << endl; // false

    // P1.7 countconstruct
    cout << "countConstruct 1:    " << countConstruct("abcdef", {"ab", "abc", "cd", "def", "abcd"}) << endl; //  2
    cout << "countConstruct 2:    " << countConstruct("skateboard", {"bo", "rd", "ate", "t", "ska", "sk", "boar"}) << endl; // 1
    cout << "countConstruct 3:    " << countConstruct("enterapotentpot", {"a", "p", "ent", "enter", "ot", "o", "t"}) << endl; // 0
    cout << "countConstruct 4:    " << countConstruct("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeef", {"e", "ee", "eee", "eeee", "eeeee"}) << endl; // 4

    return 0;
}