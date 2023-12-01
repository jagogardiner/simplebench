// Simplebench implementation

#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

#define DIV 1000000000.0

class time_data
{
public:
    // Start time
    nanoseconds start_ns;
    // End time
    nanoseconds end_ns;
    // Duration
    nanoseconds duration_ns;
    // Average call time
    nanoseconds avg_call_time_ns;

    // Test name
    string name;

    // ns to s
    inline static double ns_to_s(nanoseconds ns)
    {
        return ns.count() / DIV;
    }

    // Overload the << operator
    friend ostream &operator<<(ostream &os, const time_data &data)
    {
        os << "Task name: " << data.name << endl;
        os << "Start: " << data.start_ns.count() << "ns" << endl;
        os << "End: " << data.end_ns.count() << "ns" << endl;
        os << "Duration (ns): " << data.duration_ns.count() << endl;
        os << "Duration (s): " << time_data::ns_to_s(data.duration_ns) << endl;
        os << "Average function call time (ns): " << data.avg_call_time_ns.count() << endl;
        return os;
    }
};

// Simplebench implementation
class Simplebench
{
public:
    // Bench a function and return the time data
    template <typename F, typename... Args>
    inline static time_data bench(size_t iterations, string run_name, F func, Args... args)
    {
        // Get the start time
        auto start = chrono::steady_clock::now();

        // Array for the call times
        nanoseconds total_call_time = 0ns;

        // Run the function
        for (size_t i = 0; i < iterations; i++)
        {
            auto func_call = chrono::steady_clock::now();
            func(args...);
            auto func_end = chrono::steady_clock::now();
            auto func_duration = chrono::duration_cast<chrono::nanoseconds>(func_end - func_call);
            total_call_time += func_duration;
        }
        total_call_time /= iterations;
        // Get the end time
        auto end = chrono::steady_clock::now();
        // Calculate the duration
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
        // Print the duration
        cout << "Duration: " << duration.count() << "ns" << endl;
        // Return the time data
        time_data data;
        data.start_ns = start.time_since_epoch();
        data.end_ns = end.time_since_epoch();
        data.duration_ns = duration;
        data.name = run_name;
        data.avg_call_time_ns = total_call_time;

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
            cout << "Run " << i + 1 << endl;
            cout << data[i] << endl;
        }
        // Find the fastest run
        int fastest = 0;
        int fastest_call = 0;
        for (int i = 0; i < N; i++)
        {
            if (data[i].duration_ns < data[fastest].duration_ns)
            {
                fastest = i;
            }
            if (data[i].avg_call_time_ns < data[fastest_call].avg_call_time_ns)
            {
                fastest_call = i;
            }
        }
        // Print the fastest run
        cout << "Fastest run: " << fastest + 1 << " at " << time_data::ns_to_s(data[fastest].duration_ns) << endl;
        cout << "Fastest function call: " << fastest_call + 1 << " at " << time_data::ns_to_s(data[fastest_call].avg_call_time_ns) << endl;
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
