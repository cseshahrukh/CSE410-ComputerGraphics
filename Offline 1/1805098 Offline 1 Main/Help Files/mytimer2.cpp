    #include <iostream>  
    #include <ctime>  
    using namespace std;  
      
    int main()  
    {  
        time_t now = time(0); // get current date and time  
      
        
      
        tm* ltm = localtime(&now);  
      
        // print various components of tm structure.  
        cout << "Year:" <<  1900 + ltm->tm_year << endl; // print the year  
        cout << "Month: " << 1 + ltm->tm_mon << endl; // print month number  
        cout << "Day: " << ltm->tm_mday << endl; // print the day  
        // Print time in hour:minute:second  
        int hour=0+ltm->tm_hour; 
        int minute=0 + ltm->tm_min; 
        int second=0+ltm->tm_sec;
        cout<<"Hour is "<<hour<<endl; 
        cout<<"Minute is "<<minute<<endl; 
        cout<<"Second is "<<second<<endl; 
    }  