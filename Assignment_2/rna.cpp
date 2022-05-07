/// \headerfile iostream
/// \brief including iostream library.
#include <iostream>
/// \headerfile vector
/// \brief including vector library.
#include <vector>
/// \headerfile chrono
/// \brief including chrono library.
#include <chrono>
/// \headerfile fstream
/// \brief including fstream library.
#include <fstream>
/// \namespace std
/// \brief using standard namespace.
using namespace std;

/// \class RNA
/// \brief RNA defines a structure for RNA handling, processing, and output.
/// This class defines a length, RNA sequence, dp matrix, total pairs, time taken variables.
class RNA
{
public:
    /// \brief struct to contain the pairing data for back tracing the solution
    struct paired_data
    {
        char base1;
        char base2;
        int id1;
        int id2;
    };
    /// \brief struct paired_data paired_id_base stores the nucleotide pairs and their indices.
    struct paired_data paired_id_base[200];
    /// \brief int len stores the length of RNA sequence
    int len;
    /// \brief string base stores the RNA sequence
    string base;
    /// \brief vector<vector<int>> opt stores dp values of all subproblems
    vector<vector<int>> opt;
    /// \brief vector<int> partner stores whether the nucleotide is paired or not
    vector<int> partner;
    /// \brief int total_pairs stores the number of total pairs found in RNA sequence.
    int total_pairs;
    /// \brief int total_pairs_output stores the total number of pairs to show at output.
    int total_pairs_output;
    /// \brief double time_taken stores time taken to compute the pairs in sequence.
    double time_taken = 0;
    /// \brief Empty Constructor to create an RNA object.
    RNA()
    {
        len = 0;
        this->base = "";
        vector<vector<int>> opt;
    }
    /// \brief Constructor to create an RNA object with given RNA sequence string.
    RNA(string input)
    {
        this->base = input;
        this->len = base.length();
        this->opt = vector<vector<int>>(base.length() + 1, vector<int>(base.length() + 1));
        this->total_pairs = 0;
        this->total_pairs_output = 0;
    }
    /// \brief member function to begin the proessing of RNA sequence
    /// It initializes the dp matrix to 0, counts the time taken to execute and calls pairup function to process RNA sequence.
    void start_pairing()
    {
        for (int i = 0; i <= this->len; i++)
            for (int j = 0; j <= this->len; j++)
                this->opt[i][j] = 0;
        int no_of_pairs = 0;

        auto start1 = chrono::steady_clock::now();
        no_of_pairs = pairup(1, this->len);
        auto stop1 = chrono::steady_clock::now();

        this->time_taken = chrono::duration<double, milli>(stop1 - start1).count();

        this->total_pairs = no_of_pairs;

        display_result(no_of_pairs);
    }
    /// \brief member function to process and count the number of pairs in RNA sequence.
    /// It works by dynamic programming algorithm, following the given constraints.
    /// \param [in] int start the starting point of RNA sequence / subsequence
    /// \param [in] int end the ending point of RNA sequence / subsequence
    /// \returns int the total number of pairs found in RNA sequence, following the constraints.
    int pairup(int start, int end)
    {
        for (int k = 5; k < end; k++)
        {
            for (int i = start; i <= end - k; i++)
            {
                int j = i + k;
                int maxt = 0;
                bool paired = false;
                int t;
                for (t = i; t < j - 4; t++)
                {
                    if (satisfyConds(t, j))
                    {
                        int ans_paired = this->opt[i][t - 1] + this->opt[t + 1][j - 1];
                        if (maxt <= ans_paired)
                        {
                            maxt = ans_paired;
                            paired = true;
                        }
                    }
                }
                if (paired)
                {
                    this->opt[i][j] = max(this->opt[i][j - 1], 1 + maxt);
                }
                else
                    this->opt[i][j] = this->opt[i][j - 1];
            }
        }
        return this->opt[start][end];
    }
    /// \brief member function to check whether the given RNA conditions are satisfied (kinks and matching).
    /// \returns boolean value satisfied or not
    /// \retval true the nucletides can be paired.
    /// \retval false the nucletides can not be paired.
    bool satisfyConds(int i, int j)
    {
        if (i >= j - 4)
        {
            return false;
        }
        if (matches_base(this->base[i - 1], this->base[j - 1]))
        {
            return true;
        }
        return false;
    }
    /// \brief member function to check whether the given two nucleotides match.
    /// \returns boolean value matches or not
    /// \retval true the nucletides can be paired.
    /// \retval false the nucletides can not be paired.
    bool matches_base(char x, char y)
    {
        if ((x == 'A' && y == 'U') || (x == 'U' && y == 'A') || (x == 'G' && y == 'C') || (x == 'C' && y == 'G'))
            return true;
        else
            return false;
    }
    /// \brief member function to check whether the given bases are already present in output or not.
    /// \returns boolean value present in output or not
    /// \retval true the value present in output
    /// \retval false the value not present in output
    bool check_already_paired(int i)
    {
        for (int k = 0; k < this->total_pairs_output; k++)
        {
            if (i == paired_id_base[k].id1 || i == paired_id_base[k].id2)
                return true;
        }
        return false;
    }
    /// \brief member function to process the problem again and find the final pairs matched.
    /// the total number of pairs found in RNA sequence are stored ith indices and base names inside a struct.
    /// \param [in] int start the starting point of RNA sequence / subsequence
    /// \param [in] int end the ending point of RNA sequence / subsequence
    void find_matchings_backwards(int i, int j)
    {
        if (i < j - 4)
        {
            if (opt[i][j] == opt[i][j - 1])
                find_matchings_backwards(i, j - 1);
            else
            {
                for (int k = i; k < j - 4; k++)
                {
                    if (matches_base(this->base[k - 1], this->base[j - 1]))
                    {
                        if (opt[i][j] == opt[i][k - 1] + opt[k + 1][j - 1] + 1)
                        {
                            this->process_pairs_setting(k, j);
                            find_matchings_backwards(i, k - 1);
                            find_matchings_backwards(k + 1, j - 1);
                            break;
                        }
                    }
                }
            }
        }
    }

    /// \brief member function to set the struct values to matching pairs and their indices
    void process_pairs_setting(int k, int j)
    {
        if (!check_already_paired(k) && !check_already_paired(j))
        {
            paired_id_base[this->total_pairs_output].base1 = this->base[k - 1];
            paired_id_base[this->total_pairs_output].id1 = k;
            paired_id_base[this->total_pairs_output].base2 = this->base[j - 1];
            paired_id_base[this->total_pairs_output].id2 = j;
            this->total_pairs_output++;
        }
    }
    /// \brief member function to display the number of pairs result, and time taken to compute them.
    void display_result(int ans)
    {
        cout << "Total no. of pairs computed: " << ans << endl;
        cout << "\nThe pairs are as follows:" << endl;
        this->find_matchings_backwards(1, this->len);
        for (int i = 0; i < this->total_pairs_output; i++)
        {
            cout << "(" << paired_id_base[i].base1 << " , " << paired_id_base[i].base2 << ") at (" << paired_id_base[i].id1 << ", " << paired_id_base[i].id2 << ")\n";
        }
        cout << "\nTime taken to compute pairs for this RNA sequence: " << this->time_taken << "ms.\n";
    }
    /// \brief member function to display the dynamic programming matrix, showing all subproblem values.
    void show_matrix()
    {
        for (int i = 1; i <= this->len; i++)
        {
            for (int j = 1; j <= this->len; j++)
            {
                cout << " " << this->opt[i][j] << " ";
            }
            cout << "\n";
        }
    }
};