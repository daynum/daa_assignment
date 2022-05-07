/// \headerfile rna.cpp
/// \brief including rna cpp file.
#include "rna.cpp"

/// \namespace std
/// \brief using standard namespace.
using namespace std;

/// \brief main function to drive the whole code.
/// Takes RNA sequence as string input, and sends it to processing.
/// Writes the result to a CSV file.
int main()
{
    int total_lines;
    cin >> total_lines;
    ofstream results("results.txt");
    for (int i = 0; i < total_lines; i++)
    {
        string base;
        cin >> base;
        cout << "RNA Sequence number " << i << ":\n";
		cout << base << "\n";
        cout << "Sequence Length: " << base.length() << "\n";
        RNA myRNA = RNA(base);
        myRNA.start_pairing();
        cout << "------------------------------------------------------------\n";
        results << myRNA.len << ", " << myRNA.base << ", " << myRNA.total_pairs << ", " << myRNA.time_taken << "\n";
    }
    cout << "Results exported to file named results.txt in CSV format.";
}