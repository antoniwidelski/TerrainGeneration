

const unsigned int WINDOW_WIDTH = 2048;
const unsigned int WINDOW_HEIGHT = 1536;

const float PI = 3.14;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}