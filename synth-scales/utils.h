
int wrapi(int num, int const left, int const right) {
    int range_size = right - left;
    if (num < left)
        num += range_size * ((left - num) / range_size + 1);
    return left + (num - left) % range_size;
}
