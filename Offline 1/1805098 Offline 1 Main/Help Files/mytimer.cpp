    #include <ctime>   
    #include <iostream>  
      
      
    using namespace std;  
      
    int main()  
    {  
      
        time_t now = time(0); // get current dat/time with respect to system  
        tm* ltm = localtime(&now);  
        char* dt = ctime(&now); // convert it into string  
      
        cout << "The local date and time is: " << dt << endl; // print local date and time  
      
        tm* gmtm = gmtime(&now); // for getting time to UTC convert to struct  
        dt = asctime(gmtm);  
        cout << "The UTC date and time is:" << dt << endl; // print UTC date and time  
    }  