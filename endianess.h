void SwapBytes(void *pv, size_t n);
#define SWAP(x) SwapBytes(&x, sizeof(x));
void checkEndianess(int *endianess);
