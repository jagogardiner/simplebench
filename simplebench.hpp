// Simplebench implementation

#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

template <int N>
class time_data
{
public:
    // Start time
    nanoseconds start_ns[N];
    // End time
    nanoseconds end_ns[N];
    // Duration
    nanoseconds duration_ns[N];

    // Convert nanoseconds to milliseconds
    auto ns_to_ms(nanoseconds nano)
    {
        return duration_cast<milliseconds>(abs(nano)).count();
    }

    // Convert nanoseconds to seconds
    auto ns_to_s(nanoseconds nano)
    {
        return duration_cast<seconds>(abs(nano)).count();
    }

    friend ostream &operator<<(ostream &os, const time_data &td)
    {
        for (int i = 0; i < N; i++)
        {
            os << "Start time [" << i << "]: " << td.start_ns[i].count() << "ns" << endl;
            os << "End time [" << i << "]: " << td.end_ns[i].count() << "ns" << endl;
            os << "Duration [" << i << "]: " << td.duration_ns[i].count() << "ns" << endl;
        }
        return os;
    }
};

// Simplebench implementation
class Simplebench
{
public:
    // Bench a function and return the time data
    template <typename F, typename... Args>
    inline static time_data<1> bench(F func, Args... args, int iterations)
    {
        // Get the start time
        auto start = chrono::high_resolution_clock::now().time_since_epoch();
        // Run the function
        for (int i = 0; i < iterations; i++)
        {
            func(args...);
        }
        // Get the end time
        auto end = chrono::high_resolution_clock::now().time_since_epoch();
        // Calculate the duration
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
        // Print the duration
        cout << "Duration: " << duration.count() << "ns" << endl;
        // Return the time data
        time_data<1> data;
        data.start_ns[0] = start;
        data.end_ns[0] = end;
        data.duration_ns[0] = duration;

        sb_header();
        cout << data << endl;
        return data;
    }

    // Compare two functions and return the time data
    template <typename F, typename... Args>
    inline static time_data<2> compare(F func1, F func2, Args... arg1, Args... arg2, int iterations)
    {
        // Time data
        time_data<2> data;
        // Get the start time
        time_data<1> td1 = bench(func1, arg1..., iterations);
        time_data<1> td2 = bench(func2, arg2..., iterations);

        // Copy the bench data to the time data struct
        data.start_ns[0] = td1.start_ns[0];
        data.start_ns[1] = td2.start_ns[0];
        data.end_ns[0] = td1.end_ns[0];
        data.end_ns[1] = td2.end_ns[0];
        data.duration_ns[0] = td1.duration_ns[0];
        data.duration_ns[1] = td2.duration_ns[0];

        sb_header();
        cout << "Comparison" << endl;
        cout << "Function 1:" << endl;
        cout << td1 << endl;
        cout << "Function 2:" << endl;
        cout << td2 << endl;
        if (data.duration_ns[0].count() < data.duration_ns[1].count())
        {
            cout << "(Function 1 is faster)" << endl;
        }
        else if (data.duration_ns[0].count() > data.duration_ns[1].count())
        {
            cout << "(Function 2 is faster)" << endl;
        }
        else
        {
            cout << "(Both functions are equally fast)" << endl;
        }
        cout << "----------------------------------------" << endl
             << endl;
        // Return the time data
        return data;
    }

private:
    static inline void sb_header()
    {
        cout << endl
             << "----------------------------------------" << endl;
        cout << "Simplebench" << endl;
        cout << "----------------------------------------" << endl;
    }
};
