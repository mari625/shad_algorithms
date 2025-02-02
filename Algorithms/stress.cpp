std::vector<int> GenRandomArray(std::mt19937* gen, size_t count, int from,
                                int to) {
    std::uniform_int_distribution<int> dist(from, to);
    std::vector<int> data(count);
    for (int& cur : data) {
        cur = dist(*gen);
    }
    return data;
}

void StressTest() {
    std::mt19937 generator(72874);  // NOLINT
    const int kMaxValue = 100000;
    const int kMaxSize = 1000;
    for (int iter = 1; iter <= 100000; ++iter) {  // NOLINT
        std::cerr << "Test " << iter << "... ";
        auto data = GenRandomArray(&generator, kMaxSize, 1, kMaxValue);
        auto ok_answer = AnswerStupid(data);
        auto my_answer = Answer(kMaxSize, data);
        if (ok_answer == my_answer) {
            std::cerr << "OK\n";
        } else {
            std::cerr << "Fail\n";
            for (auto cur : data) {
                std::cerr << cur << " ";
            }
            std::cerr << "\n";
            std::cerr << "Ok ans " << ok_answer << ", my ans " << my_answer
                      << "\n";
            break;
        }
    }
}