#include <iostream>
using namespace std;

int main()
{

    double average = 0;
    average = 67;
    cout << "enter number\n";
    cin >> average;

    cout << "\n\nThe Average of the  is : " << average << endl;
    if (average < 90 && average > 50)
     return 0;
    else 
	    return -1;
}

