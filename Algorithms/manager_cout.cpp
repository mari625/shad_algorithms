#include <cstdint>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

struct Segment {
    int first_bit = 0;
    int last_bit = 0;

    bool operator==(Segment other) const {
        return (this->first_bit == other.first_bit) &&
               (this->last_bit == other.last_bit);
    }

    bool operator<(Segment other) const {
        if ((this->last_bit - this->first_bit) ==
            (other.last_bit - other.first_bit)) {
            return this->first_bit < other.first_bit; //?
        }
        return (this->last_bit - this->first_bit) <
               (other.last_bit - other.first_bit);
    }
};

struct MarkedSegment {
    Segment segment;
    bool mark = false;
    int bit = 0;
    int query_number = 0;

    bool operator==(MarkedSegment other) const {
        return (this->segment == other.segment) && (this->query_number == other.query_number);
    }

    bool operator<(MarkedSegment other) const {
        if ((this->segment == other.segment)) {
            if (!this->mark && other.mark) {
                return true;
            }
            if (!this->mark && other.mark) {
                return true;
            }
        }
        return this->segment < other.segment;
    }
};

struct PushQuery {
    Segment segment;
    int query_number = 0;
};

class HashFunction {
private:
    int64_t a_parameter_;
    int64_t b_parameter_;
    int64_t p_;
    int m_;

public:
    HashFunction(){};

    HashFunction(int64_t p_parameter, int m_parameter,
                 std::mt19937* generator) {
        p_ = p_parameter;
        m_ = m_parameter;
        a_parameter_ =
            std::uniform_int_distribution<int64_t>(1, p_ - 1)(*generator);
        b_parameter_ =
            std::uniform_int_distribution<int64_t>(0, p_ - 1)(*generator);
    }

    int operator()(int key) const {
        return static_cast<int>(
            (static_cast<int64_t>(key * a_parameter_ + b_parameter_) % p_) % m_);
    }
};

class MemoryManager {
private:
    std::priority_queue<MarkedSegment> heap_of_free_segments_;
    std::vector<std::vector<PushQuery>> hash_table_full_;
    std::vector<std::vector<MarkedSegment>> hash_table_empty_left_;
    std::vector<std::vector<MarkedSegment>> hash_table_empty_right_;
    HashFunction hash_function_full_;
    HashFunction hash_function_empty_;
    std::mt19937 generator_;
    int number_;

    /* static int NextTwoPower(size_t number) {
        number--;
        number = number | (number >> 1);
        number = number | (number >> 2);
        number = number | (number >> 4);
        number = number | (number >> 8);   // NOLINT
        number = number | (number >> 16);  // NOLINT
        return static_cast<int>(number + 1);
    } */

    static void HashEmptySegment(
        MarkedSegment& marked_segment, HashFunction& hash_function,
        std::vector<std::vector<MarkedSegment>>& hash_table, int number) {  //
        int address = hash_function(marked_segment.bit);
        hash_table[address].push_back(marked_segment);  // what if same

        for (int i = 0; i < number; ++i) {
            auto& bucket = hash_table[i];
            if (!bucket.empty()) {
                for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                    std::cout << "P " << it->bit << " ["
                              << it->segment.first_bit << ", "
                              << it->segment.last_bit << "] " << it->query_number<< " "<<it->mark
                              << "  ";
                }
                std::cout << '\n';
            }
        }
        std::cout << '\n';
    }

    static void DeleteTopEmptyLeftSegment(MarkedSegment& heap_top, HashFunction& hash_function,
        std::vector<std::vector<MarkedSegment>>& hash_table, int number, bool query_needed) {
        int left = heap_top.segment.first_bit;
        int address = hash_function(left);

        auto& bucket = hash_table[address];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->segment.first_bit == left) {
                if (query_needed && it->query_number == heap_top.query_number) {
                    bucket.erase(it);
                    break;
                }
                bucket.erase(it);
                break;
            }
        }

        for (int i = 0; i < number; ++i) {
            auto& bucket = hash_table[i];
            if (!bucket.empty()) {
                for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                    std::cout << "P " << it->bit << " ["
                              << it->segment.first_bit << ", "
                              << it->segment.last_bit << "] " <<it->query_number<<" "<< it->mark
                              << "   ";
                }
                std::cout << '\n';
            }
        }
        std::cout << '\n';
    }

    static void DeleteTopEmptyRightSegment(MarkedSegment& heap_top, HashFunction& hash_function,
        std::vector<std::vector<MarkedSegment>>& hash_table, int number, bool query_needed) {
        int right = heap_top.segment.last_bit;
        int address = hash_function(right);

        auto& bucket = hash_table[address];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->segment.last_bit == right) {
                if (query_needed && it->query_number == heap_top.query_number) {
                    bucket.erase(it);
                    break;
                }
                bucket.erase(it);
                break;
            }
        }

        for (int i = 0; i < number; ++i) {
            auto& bucket = hash_table[i];
            if (!bucket.empty()) {
                for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                    std::cout << "P " << it->bit << " ["
                              << it->segment.first_bit << ", "
                              << it->segment.last_bit << "] " <<it->query_number<<" "<< it->mark
                              << "  ";
                }
                std::cout << '\n';
            }
        }
        std::cout << '\n';
    }

    static void FindAndDeleteLeftEmptySegment(
        int point, HashFunction& hash_function,
        std::vector<std::vector<MarkedSegment>>& hash_table, Segment& segment,
        int number) {                           //
        int address = hash_function(point);  // search for empty neighbour
        if (hash_table[address].empty()) {
            return;
        }

        std::vector<MarkedSegment>& empty_bucket = hash_table[address];

        // get such segment (if exists)
        for (auto it = empty_bucket.begin(); it != empty_bucket.end(); ++it) {
            if (it->bit == point && !it->mark) {
                it->mark = true;
                segment.last_bit =
                    (it->segment).last_bit;  // increasing segment length
                break;
            }
        }

        for (int i = 0; i < number; ++i) {
            auto& bucket = hash_table[i];
            if (!bucket.empty()) {
                for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                    std::cout << "P " << it->bit << " ["
                              << it->segment.first_bit << ", "
                              << it->segment.last_bit << "] " << it->query_number<<" "<<it->mark
                              << "   ";
                }
                std::cout << '\n';
            }
        }
        std::cout << '\n';
    }

    static void FindAndDeleteRightEmptySegment(
        int point, HashFunction& hash_function,
        std::vector<std::vector<MarkedSegment>>& hash_table, Segment& segment,
        int number) {                           //
        int address = hash_function(point);  // search for empty neighbour
        if (hash_table[address].empty()) {
            return;
        }

        std::vector<MarkedSegment>& empty_bucket = hash_table[address];

        // get such segment (if exists)
        for (auto it = empty_bucket.begin(); it != empty_bucket.end(); ++it) {
            if (it->bit == point && !it->mark) {
                it->mark = true;
                segment.first_bit =
                    (it->segment).first_bit;  // increasing segment length
                break;
            }
        }

        for (int i = 0; i < number; ++i) {
            auto& bucket = hash_table[i];
            if (!bucket.empty()) {
                for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                    std::cout << "P " << it->bit << " ["
                              << it->segment.first_bit << ", "
                              << it->segment.last_bit << "] " << it->query_number<<" "<<it->mark
                              << "   ";
                }
                std::cout << '\n';
            }
        }
        std::cout << '\n';
    }

    static void ClearLazyDeletions(
        std::priority_queue<MarkedSegment>& heap_of_free_segments,
        HashFunction& hash_function,
        std::vector<std::vector<MarkedSegment>>& hash_table_empty_left,
        std::vector<std::vector<MarkedSegment>>& hash_table_empty_right,
        int number) {  //
        bool left_mark  = true;
        bool right_mark = true;
        if (!heap_of_free_segments.empty()) {
            std::cout << heap_of_free_segments.top().segment.first_bit
                      << " "
                      << heap_of_free_segments.top().segment.last_bit << " "<<heap_of_free_segments.top().query_number
                      << " top now" << '\n'
                      << '\n';
        }

        while (!heap_of_free_segments.empty() && (left_mark || right_mark)) {
            left_mark = false;
            right_mark = false;

            MarkedSegment top_segment = heap_of_free_segments.top();
            int left = top_segment.segment.first_bit;

            int left_address = hash_function(left);
            if (!hash_table_empty_left[left_address].empty()) {
                auto& left_bucket = hash_table_empty_left[left_address];
                for (auto it_left = left_bucket.begin(); it_left != left_bucket.end(); ++it_left) {
                    if (it_left->bit == left && it_left->query_number == top_segment.query_number && it_left->mark) {
                        left_mark = true;
                        left_bucket.erase(it_left);
                        std::cout<<"left "<<'\n';
                        DeleteTopEmptyRightSegment(top_segment, hash_function, hash_table_empty_right, number, false);
                        heap_of_free_segments.pop();
                        break;
                    }
                }
            }

            top_segment = heap_of_free_segments.top();
            int right = top_segment.segment.last_bit;

            int right_address = hash_function(right);
            if (!hash_table_empty_right[right_address].empty()) {
            auto& right_bucket = hash_table_empty_right[right_address];
                for (auto it_right = right_bucket.begin(); it_right != right_bucket.end(); ++it_right) {
                    if (it_right->bit == right && it_right->query_number == top_segment.query_number && it_right->mark) {
                        right_mark = true;
                        right_bucket.erase(it_right);
                        std::cout<<"right"<<'\n';
                        DeleteTopEmptyLeftSegment(top_segment, hash_function, hash_table_empty_left, number, false);
                        heap_of_free_segments.pop();
                        break;
                    }
                }
            }
        }
    }

public:
    MemoryManager(int memory_size, int number_of_queries) {
        Segment memory_stack = Segment{1, memory_size};

        const int64_t kP = 100003;
        hash_table_full_.resize(number_of_queries);
        hash_table_empty_left_ = {};
        hash_table_empty_left_.resize(number_of_queries);
        hash_table_empty_right_.resize(number_of_queries);

        number_ = number_of_queries;  //

        for (int i = 0; i < number_of_queries; ++i) {
            hash_table_full_[i] = {};
            hash_table_empty_left_[i] = {};
            hash_table_empty_right_[i] = {};
        }

        const int kSeed = 75837;
        generator_.seed(kSeed);

        int m_parameter = number_of_queries;

        hash_function_full_ = HashFunction(kP, m_parameter, &generator_);
        hash_function_empty_ = HashFunction(kP, m_parameter, &generator_);

        int left_address = hash_function_empty_(memory_stack.first_bit);
        hash_table_empty_left_[left_address].push_back(
            MarkedSegment{memory_stack, false, 0});

        int right_address = hash_function_empty_(memory_stack.last_bit);
        hash_table_empty_right_[right_address] = {
            MarkedSegment{memory_stack, false, memory_size}};

        heap_of_free_segments_.push(MarkedSegment{memory_stack});
    }

    int Push(int query, int number) {
        std::cout << query << " query" << '\n';
        if (heap_of_free_segments_.empty()) {
            return -1;
        }

        MarkedSegment marked_left = heap_of_free_segments_.top();
        MarkedSegment marked_right = heap_of_free_segments_.top();
        
        int length = marked_left.segment.last_bit -
                     marked_left.segment.first_bit + 1;
        if (length < query) {
            return -1;
        }

        Segment fragment =
            Segment{marked_left.segment.first_bit, marked_left.segment.first_bit + query - 1};

        int address = hash_function_full_(number);  // hashing full segments
        PushQuery element = PushQuery{fragment, number};
        hash_table_full_[address].push_back(element);  /// what if same - nothing

        std::cout << "full hash table" << '\n';
        for (int i = 0; i < number_; ++i) {
            auto& bucket = hash_table_full_[i];
            if (!bucket.empty()) {
                for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                    std::cout << "N " << it->query_number << " ["
                              << it->segment.first_bit << ", "
                              << it->segment.last_bit << "] " << i << "   ";
                }
                std::cout << '\n';
            }
        }
        std::cout << '\n';

        heap_of_free_segments_.pop();  // delete from heap  ????

        std::cout << "empty hash table left delete" << '\n';
        // delete top segment in left table
        DeleteTopEmptyLeftSegment(marked_left, hash_function_empty_, hash_table_empty_left_, number_, true);  //

        std::cout << "empty hash table right delete" << '\n';
        // delete top segment in right table
        DeleteTopEmptyRightSegment(marked_left, hash_function_empty_, hash_table_empty_right_, number_, true);  //

        marked_left.segment.first_bit += query;  // change top segment left
        marked_left.bit = marked_left.segment.first_bit;
        marked_left.query_number = number;
        marked_right.segment.first_bit += query;  // change top segment right
        marked_right.bit = marked_left.segment.last_bit;
        marked_right.query_number = number;

        if (!heap_of_free_segments_.empty()) {  //clear lazy deletions
            ClearLazyDeletions(heap_of_free_segments_, hash_function_empty_,
                               hash_table_empty_left_, hash_table_empty_right_,
                               number_);
        }

        if (length > query) {
            // push segment left side into hashtable
            std::cout << "empty hash table left" << '\n';
            HashEmptySegment(marked_left, hash_function_empty_,hash_table_empty_left_, number_);  //

            // push segment right side into hashtable
            std::cout << "empty hash table right" << '\n';
            HashEmptySegment(marked_right, hash_function_empty_, hash_table_empty_right_, number_);  //

            heap_of_free_segments_.push(MarkedSegment{marked_left.segment, false, 0, number});  // push segment into heap
        }
        if (!heap_of_free_segments_.empty()) {
            std::cout << heap_of_free_segments_.top().segment.first_bit
                      << " "
                      << heap_of_free_segments_.top().segment.last_bit<<" "<<heap_of_free_segments_.top().query_number
                      << " top now" << '\n'
                      << '\n';
        }
        // std::cout<<heap_of_free_segments_.top().left->segment.first_bit<<"
        // "<<heap_of_free_segments_.top().right->segment.last_bit<<" top
        // now"<<'\n'<<'\n';

        return fragment.first_bit;
    }

    void Delete(int query) {
        // std::cout<<heap_of_free_segments_.top().left->segment.first_bit<<"
        // "<<heap_of_free_segments_.top().right->segment.last_bit<<" top
        // now"<<'\n'<<'\n';

        std::cout << query << " query delete" << '\n';
        int address = hash_function_full_(
            query);  // check if such segment exists in full vector
        std::cout << address << '\n';
        if (hash_table_full_[address].empty()) {
            return;
        }
        // std::cout<<"aaaaaa"<<'\n';

        std::vector<PushQuery>& full_bucket = hash_table_full_[address];
        PushQuery full_segment_with_query;

        int segment_found = 0;  // get such segment (if exists)
        for (auto it = full_bucket.begin(); it != full_bucket.end(); ++it) {
            if (it->query_number == query) {
                full_segment_with_query.segment = it->segment;
                full_segment_with_query.query_number = query;
                full_bucket.erase(it);
                segment_found = 1;
                break;
            }
        }

        std::cout << segment_found << '\n';

        std::cout << "full hash table" << '\n';
        for (int i = 0; i < number_; ++i) {
            auto& bucket = hash_table_full_[i];
            if (!bucket.empty()) {
                for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                    std::cout << "N " << it->query_number << " ["
                              << it->segment.first_bit << ", "
                              << it->segment.last_bit << "]  ";
                }
                std::cout << '\n';
            }
        }
        std::cout << '\n';

        if (segment_found == 0) {
            return;
        }

        // std::cout<<full_segment.first_bit<<" "<<full_segment.last_bit<<"
        // found segment"<<'\n';

        std::cout << full_segment_with_query.segment.first_bit << " " << full_segment_with_query.segment.last_bit
                  << " found segment0" << '\n';
        if (full_segment_with_query.segment.first_bit != 1) {
            int right_border = full_segment_with_query.segment.first_bit - 1;
            std::cout << right_border << "left border" << '\n';
            std::cout << "empty hash table right delete" << '\n';
            FindAndDeleteRightEmptySegment(right_border, hash_function_empty_,
                                           hash_table_empty_right_,
                                           full_segment_with_query.segment, number_);  //
        }

        if (full_segment_with_query.segment.last_bit != number_) {
            int left_border = full_segment_with_query.segment.last_bit + 1;
            std::cout << left_border << "right border" << '\n';
            std::cout << "empty hash table left delete" << '\n';
            FindAndDeleteLeftEmptySegment(left_border, hash_function_empty_,
                                          hash_table_empty_left_, full_segment_with_query.segment,
                                          number_);
        }
        std::cout << full_segment_with_query.segment.first_bit << " " << full_segment_with_query.segment.last_bit
                  << " found segment" << '\n';

        // clear lazy deletions
        if (!heap_of_free_segments_.empty()) {
            ClearLazyDeletions(heap_of_free_segments_, hash_function_empty_,
                               hash_table_empty_left_, hash_table_empty_right_,
                               number_);
        }

        // push segment into heap (and hashtables)
        MarkedSegment marked_segment_left =
            MarkedSegment{full_segment_with_query.segment, false, full_segment_with_query.segment.first_bit, query};
        std::cout << "empty hash table left" << '\n';
        HashEmptySegment(marked_segment_left, hash_function_empty_,
                             hash_table_empty_left_, number_);  //

        MarkedSegment marked_segment_right =
            MarkedSegment{full_segment_with_query.segment, false, full_segment_with_query.segment.last_bit, query};
        std::cout << "empty hash table right" << '\n';
        HashEmptySegment(marked_segment_right, hash_function_empty_,
                             hash_table_empty_right_, number_);  //

        heap_of_free_segments_.push(MarkedSegment{full_segment_with_query.segment, false, 0, query});  // put full segment !!!!!!!!

        if (!heap_of_free_segments_.empty()) {
            std::cout << heap_of_free_segments_.top().segment.first_bit
                      << " "
                      << heap_of_free_segments_.top().segment.last_bit<<" "<<heap_of_free_segments_.top().query_number
                      << " top now" << '\n'
                      << '\n';
        }
    }
};

void AnswerQueries(int memory_size, int number_of_queries,
                   const std::vector<int>& queries) {
    MemoryManager manager = MemoryManager(memory_size, number_of_queries);

    for (int i = 0; i < number_of_queries; ++i) {
        int query = queries[i];
        if (query > 0) {
            std::cout << manager.Push(query, i + 1) << '\n';
        } else {
            manager.Delete(abs(query));
        }
    }
}

int main() {
    int memomry_size = 0;
    int number_of_queries = 0;

    std::cin >> memomry_size >> number_of_queries;

    std::vector<int> queries;

    for (int i = 0; i < number_of_queries; ++i) {
        int element = 0;
        std::cin >> element;
        queries.push_back(element);
    }

    AnswerQueries(memomry_size, number_of_queries, queries);
    return 0;
}