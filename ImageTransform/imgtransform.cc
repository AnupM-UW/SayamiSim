#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;

// std::ostream& operator<<(std::ostream strm, int* values) {
//     cout<<"[ ";

//     cout<<" ]";
// }
template <typename T>
void print_array(T * arr, unsigned int size, bool displayHex = false) {
    cout<<"[ ";
    for (int i = 0; i < size & arr != nullptr; ++i) {
        if (!displayHex) {
            cout << arr[i] << " ";
        } else {
            printf("%x ",arr[i]);
        }
    }
    cout<<"]"<<endl;
}

unsigned int* ImageExpand(unsigned int* origValues, unsigned int size, double scaleFactor, unsigned int *newSz) {
    if (scaleFactor < 1) {
        throw std::range_error("scaleFactor needs to be > 1");
    }
    if (size < 1) {
        throw std::range_error("size needs to be >= 1");
    }

    unsigned int newSize = round(scaleFactor*size);
    unsigned int* retval = new unsigned int[newSize];
    std::memset(retval, 0, newSize * sizeof(unsigned int));
    double* scaled = new double[newSize];
    double rise = (double)1 / scaleFactor;
    for (int i = 0; i < newSize; ++i) {
        scaled[i] = 0 + i*rise;
    }
    print_array(scaled, newSize);
    // values in the scaled array will be as follows
    // e.g. for 5 values and scale by 1.4, the values will be
    // [ 0.00000  0.714286  1.42857  2.14286  2.85714  3.57143  4.28571 ]
    // so for 1st element, we will take px[0]*(1-0.00000) + px[1]*(0.00000)
    //    for 2nd element, we will take px[0]*(1-.714286) + px[1]*(0.714286)
    //    for 3rd element, we will take px[1]*(1-.42857) + px[2]*(.42857)
    //    for 7th element, we will take px[4] and effectively disregard the *(1-.28571) (see below)
    //        note that the px[5] does not exist in the original since it is only length 5
    //    in fact, if pixel from original at the right edge is non-existent, e.g. px[5], we will always take
    //    the min(right edge, left edge + 1)
    //    so the 7th element would be, left = 4, right = min(4,5), and therefore px[4]*(1-.28571) + px[4]*(0.28571), effectively px[4]*1
    for (int i = 0; i < newSize; ++i) {
        int leftIdx = floor(scaled[i]);
        int rightIdx = min((int)size-1, leftIdx + 1);
        double mult2 = scaled[i] - leftIdx;
        double mult1 = 1 - mult2;
        // now the real value will be origValues[leftIdx]*mult1 + origValues[rightIdx]*mult2;
        int 
            r1 = (origValues[leftIdx]  & 0x00FF0000) >> 16,
            r2 = (origValues[rightIdx] & 0x00FF0000) >> 16,
            g1 = (origValues[leftIdx]  & 0x0000FF00) >>  8,
            g2 = (origValues[rightIdx] & 0x0000FF00) >>  8,
            b1 = (origValues[leftIdx]  & 0x000000FF),
            b2 = (origValues[rightIdx] & 0x000000FF);
        int 
            r = (int)round(((double)r1)*mult1 + ((double)r2)*mult2),
            g = (int)round(((double)g1)*mult1 + ((double)g2)*mult2),
            b = (int)round(((double)b1)*mult1 + ((double)b2)*mult2);
        retval[i] = 0xFF000000 | (r << 16) | (g << 8) | b;
    }
    delete [] scaled;
    *newSz = newSize;
    return retval;
}

int main() {
    unsigned int foo[3] = {0xFFFF0000, 0xFF00FF00, 0xFF0000FF}; // red, green, blue
    unsigned int newSize;
    unsigned int* newFoo = ImageExpand(foo, 3, 15, &newSize);
    print_array(newFoo, newSize, true);
    delete [] newFoo;
    cout<<"Shutting Down"<<endl;
}
