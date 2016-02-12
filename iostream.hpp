
#include <iostream>
#include <vector>

// not that generic, to make compiler not that confuse.
template<typename T>
inline std::ostream& operator<<(std::ostream& stream, const std::vector<T>& series)
{
    typedef std::vector<T> Series;
    stream << '[';
    for (typename Series::size_type i = 0, last = series.size() - 1; i != series.size(); ++i) {
        if (stream.good()) {
            stream << series[i];
            if (i < last) {
                stream << ',' << ' ';
            }
        }
    }
    stream << ']';
    return stream;
}
