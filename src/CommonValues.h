

const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 768;

const float PI = 3.14;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}