// Simplebench implementation

#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

class time_data
{
public:
    // Start time
    nanoseconds start_ns;
    // End time
    nanoseconds end_ns;
    // Duration
    nanoseconds duration_ns;

    // Convert nanoseconds to seconds
    inline static double ns_to_s(nanoseconds ns)
    {
        return ns.count() / 1000000000.0;
    }

    friend ostream &operator<<(ostream &os, const time_data &data)
    {
        os << "Start: " << data.start_ns.count() << "ns" << endl;
        os << "End: " << data.end_ns.count() << "ns" << endl;
        os << "Duration: " << data.duration_ns.count() << "ns" << endl;
        os << "Duration: " << ns_to_s(data.duration_ns) << "s" << endl;
        return os;
    }
};

// Simplebench implementation
class Simplebench
{
public:
    // Bench a function and return the time data
    template <typename F, typename... Args>
    inline static time_data bench(int iterations, F func, Args... args)
    {
        // Get the start time
        auto start = chrono::high_resolution_clock::now();
        // Run the function
        for (int i = 0; i < iterations; i++)
        {
            func(args...);
        }
        // Get the end time
        auto end = chrono::high_resolution_clock::now();
        // Calculate the duration
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
        // Print the duration
        cout << "Duration: " << duration.count() << "ns" << endl;
        // Return the time data
        time_data data;
        data.start_ns = start.time_since_epoch();
        data.end_ns = end.time_since_epoch();
        data.duration_ns = duration;

        sb_header();
        cout << data << endl;
        return data;
    }
    template <int N>
    inline static void compare(time_data data[])
    {
        sb_header();
        cout << "Compare" << endl;
        cout << "----------------------------------------" << endl;
        for (int i = 0; i < N; i++)
        {
            cout << "Run " << i << endl;
            cout << data[i] << endl;
        }
        // Find the fastest run
        int fastest = 0;
        for (int i = 0; i < N; i++)
        {
            if (data[i].duration_ns < data[fastest].duration_ns)
            {
                fastest = i;
            }
        }
        // Print the fastest run
        cout << "Fastest run: " << fastest << " at " << time_data::ns_to_s(data[fastest].duration_ns) << endl;
        cout << "----------------------------------------" << endl;
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
