#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>

// 包含你的circular queue實現
#include "circularQueue.cpp"

// 測試計數器
int tests_passed = 0;
int tests_total = 0;

// 測試輔助宏
#define TEST(name) \
    void test_##name(); \
    void run_test_##name() { \
        tests_total++; \
        std::cout << "Testing " #name "... "; \
        try { \
            test_##name(); \
            tests_passed++; \
            std::cout << "PASSED\n"; \
        } catch (const std::exception& e) { \
            std::cout << "FAILED: " << e.what() << "\n"; \
        } catch (...) { \
            std::cout << "FAILED: Unknown exception\n"; \
        } \
    } \
    void test_##name()

// 輔助函數：檢查circular queue內容
void verify_queue_content(const circularQueue<int>& q, const std::vector<int>& expected) {
    assert(q.size() == expected.size());
    
    if (expected.empty()) {
        assert(q.empty());
        return;
    }
    
    assert(!q.empty());
    assert(q.front() == expected.front());
    assert(q.back() == expected.back());
    
    // 檢查隊列是否有正確的容量關係
    //assert(q.size() <= q.capacity());
}

// ============= 基本功能測試 =============

TEST(constructor_and_destructor) {
    // 測試默認構造函數
    circularQueue<int> q(5);
    assert(q.empty());
    assert(q.size() == 0);
    assert(q.capacity() == 5);
    assert(!q.isFull());
}

TEST(constructor_with_default_value) {
    // 測試帶默認值的構造函數
    circularQueue<int> q(3, 42);
    assert(q.capacity() == 3);
    assert(q.empty());  // 雖然所有位置都有默認值，但count=0所以為空
    
    // 驗證可以正常enqueue
    q.enqueue(1);
    assert(q.size() == 1);
    assert(q.front() == 1);
}

TEST(enqueue_and_dequeue_basic) {
    circularQueue<int> q(3);
    
    // 測試enqueue
    q.enqueue(1);
    assert(!q.empty());
    assert(q.size() == 1);
    assert(q.front() == 1);
    assert(q.back() == 1);
    
    q.enqueue(2);
    assert(q.size() == 2);
    assert(q.front() == 1);
    assert(q.back() == 2);
    
    q.enqueue(3);
    assert(q.size() == 3);
    assert(q.isFull());
    assert(q.front() == 1);
    assert(q.back() == 3);
    
    // 測試dequeue
    q.dequeue();
    assert(q.size() == 2);
    assert(!q.isFull());
    assert(q.front() == 2);
    assert(q.back() == 3);
    
    q.dequeue();
    assert(q.size() == 1);
    assert(q.front() == 3);
    assert(q.back() == 3);
    
    q.dequeue();
    assert(q.empty());
    assert(q.size() == 0);
}

TEST(circular_behavior) {
    circularQueue<int> q(3);
    
    // 填滿隊列
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    assert(q.isFull());
    
    // 移除一個，再添加一個（測試循環行為）
    q.dequeue();  // 移除1
    q.enqueue(4); // 添加4
    
    assert(q.size() == 3);
    assert(q.isFull());
    assert(q.front() == 2);
    assert(q.back() == 4);
    
    // 繼續測試循環
    q.dequeue();  // 移除2
    q.enqueue(5); // 添加5
    
    assert(q.front() == 3);
    assert(q.back() == 5);
}

// ============= 異常處理測試 =============

TEST(overflow_exception) {
    circularQueue<int> q(2);
    
    // 填滿隊列
    q.enqueue(1);
    q.enqueue(2);
    assert(q.isFull());
    
    // 嘗試添加到已滿的隊列
    try {
        q.enqueue(3);
        assert(false);  // 不應該到達這裡
    } catch (const std::overflow_error&) {
        // 預期的異常
    }
    
    // 隊列狀態應該保持不變
    assert(q.isFull());
    assert(q.size() == 2);
    assert(q.front() == 1);
}

TEST(underflow_exception) {
    circularQueue<int> q(3);
    
    // 空隊列測試異常
    try {
        q.dequeue();
        assert(false);  // 不應該到達這裡
    } catch (const std::underflow_error&) {
        // 預期的異常
    }
    
    try {
        q.front();
        assert(false);
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    try {
        q.back();
        assert(false);
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    // 隊列應該仍然為空
    assert(q.empty());
    assert(q.size() == 0);
}

// ============= 拷貝和移動測試 =============

TEST(copy_constructor) {
    circularQueue<std::string> original(3);
    original.enqueue("hello");
    original.enqueue("world");
    original.enqueue("test");
    
    circularQueue<std::string> copy = original;  // 拷貝構造
    
    // 檢查拷貝的內容
    assert(copy.size() == 3);
    assert(copy.capacity() == 3);
    assert(copy.front() == "hello");
    assert(copy.back() == "test");
    
    // 修改原隊列不應影響拷貝
    original.dequeue();
    assert(original.size() == 2);
    assert(copy.size() == 3);
    
    // 修改拷貝不應影響原隊列
    copy.dequeue();
    assert(copy.size() == 2);
    assert(original.size() == 2);
    assert(copy.front() == "world");
}

TEST(copy_constructor_with_circular_state) {
    circularQueue<int> original(3);
    
    // 創建循環狀態
    original.enqueue(1);
    original.enqueue(2);
    original.enqueue(3);
    original.dequeue();  // frontIdx != 0
    original.enqueue(4);
    
    circularQueue<int> copy = original;
    
    // 檢查拷貝是否正確
    assert(copy.size() == 3);
    assert(copy.front() == 2);
    assert(copy.back() == 4);
    
    // 檢查拷貝的順序是否正確
    assert(copy.front() == 2); copy.dequeue();
    assert(copy.front() == 3); copy.dequeue();
    assert(copy.front() == 4);
}

TEST(move_constructor) {
    circularQueue<int> original(3);
    original.enqueue(1);
    original.enqueue(2);
    original.enqueue(3);
    
    circularQueue<int> moved = std::move(original);  // 移動構造
    
    // 檢查移動後的內容
    assert(moved.size() == 3);
    assert(moved.capacity() == 3);
    assert(moved.front() == 1);
    assert(moved.back() == 3);
    
    // 原隊列應該被清空
    assert(original.capacity() == 0);
    assert(original.size() == 0);
}

TEST(copy_assignment) {
    circularQueue<int> q1(2), q2(4);
    
    q1.enqueue(1);
    q1.enqueue(2);
    
    q2.enqueue(10);
    q2.enqueue(20);
    q2.enqueue(30);
    
    q2 = q1;  // 拷貝賦值
    
    // 檢查賦值結果
    assert(q2.size() == 2);
    assert(q2.capacity() == 2);
    assert(q2.front() == 1);
    assert(q2.back() == 2);
    
    // 原隊列應該保持不變
    assert(q1.size() == 2);
    assert(q1.front() == 1);
}

TEST(move_assignment) {
    circularQueue<int> q1(3), q2(2);
    
    q1.enqueue(1);
    q1.enqueue(2);
    q1.enqueue(3);
    
    q2.enqueue(10);
    
    q2 = std::move(q1);  // 移動賦值
    
    // 檢查移動結果
    assert(q2.size() == 3);
    assert(q2.capacity() == 3);
    assert(q2.front() == 1);
    assert(q2.back() == 3);
    
    // 原隊列應該被清空
    assert(q1.capacity() == 0);
    assert(q1.size() == 0);
}

TEST(self_assignment) {
    circularQueue<int> q(3);
    q.enqueue(1);
    q.enqueue(2);
    
    q = q;  // 自我賦值
    
    // 應該保持原樣
    assert(q.size() == 2);
    assert(q.front() == 1);
    assert(q.back() == 2);
}

// ============= 清理操作測試 =============

TEST(clear_operation) {
    circularQueue<std::string> q(3);
    
    q.enqueue("test1");
    q.enqueue("test2");
    q.enqueue("test3");
    
    assert(q.size() == 3);
    assert(q.isFull());
    
    q.clear();
    
    assert(q.empty());
    assert(q.size() == 0);
    assert(!q.isFull());
    assert(q.capacity() == 3);  // 容量應該保持不變
    
    // 清理後應該可以正常使用
    q.enqueue("after_clear");
    assert(q.size() == 1);
    assert(q.front() == "after_clear");
}

// ============= 容量和狀態測試 =============

TEST(capacity_and_state_checks) {
    circularQueue<int> q(4);
    
    // 初始狀態
    assert(q.empty());
    assert(!q.isFull());
    assert(q.size() == 0);
    assert(q.capacity() == 4);
    
    // 逐步填滿
    q.enqueue(1);
    assert(!q.empty());
    assert(!q.isFull());
    assert(q.size() == 1);
    
    q.enqueue(2);
    q.enqueue(3);
    assert(!q.isFull());
    assert(q.size() == 3);
    
    q.enqueue(4);
    assert(q.isFull());
    assert(q.size() == 4);
    assert(q.size() == q.capacity());
}

// ============= 複雜場景測試 =============

TEST(complex_circular_operations) {
    circularQueue<int> q(4);
    
    // 複雜的入隊出隊序列
    q.enqueue(1);
    q.enqueue(2);
    q.dequeue();  // 移除1
    q.enqueue(3);
    q.enqueue(4);
    q.enqueue(5);  // 現在應該是[2,3,4,5]
    
    assert(q.isFull());
    assert(q.front() == 2);
    assert(q.back() == 5);
    
    // 移除兩個，添加兩個
    q.dequeue();  // 移除2
    q.dequeue();  // 移除3
    q.enqueue(6);
    q.enqueue(7);  // 現在應該是[4,5,6,7]
    
    assert(q.isFull());
    assert(q.front() == 4);
    assert(q.back() == 7);
    
    // 驗證順序
    assert(q.front() == 4); q.dequeue();
    assert(q.front() == 5); q.dequeue();
    assert(q.front() == 6); q.dequeue();
    assert(q.front() == 7); q.dequeue();
    assert(q.empty());
}

// ============= 不同類型測試 =============

TEST(string_operations) {
    circularQueue<std::string> q(3);
    
    q.enqueue("first");
    q.enqueue("second");
    q.enqueue("third");
    
    assert(q.front() == "first");
    assert(q.back() == "third");
    
    q.dequeue();
    q.enqueue("fourth");
    
    assert(q.front() == "second");
    assert(q.back() == "fourth");
}

// ============= 大量數據測試 =============

TEST(large_scale_operations) {
    const size_t CAP = 100;
    circularQueue<int> q(CAP);
    
    // 填滿隊列
    for (int i = 0; i < CAP; i++) {
        q.enqueue(i);
    }
    assert(q.isFull());
    assert(q.size() == CAP);
    
    // 移除一半，再添加一半
    for (int i = 0; i < CAP/2; i++) {
        assert(q.front() == i);
        q.dequeue();
    }
    
    for (int i = CAP; i < CAP + CAP/2; i++) {
        q.enqueue(i);
    }
    
    assert(q.isFull());
    assert(q.front() == CAP/2);
    assert(q.back() == CAP + CAP/2 - 1);
}

// ============= 邊界條件測試 =============

TEST(boundary_conditions) {
    // 容量為1的隊列
    circularQueue<int> q1(1);
    assert(q1.capacity() == 1);
    
    q1.enqueue(42);
    assert(q1.isFull());
    assert(q1.front() == 42);
    assert(q1.back() == 42);
    
    q1.dequeue();
    assert(q1.empty());
    
    // 重複填滿和清空
    for (int i = 0; i < 10; i++) {
        q1.enqueue(i);
        assert(q1.isFull());
        q1.dequeue();
        assert(q1.empty());
    }
}

// ============= 主測試函數 =============

int main() {
    std::cout << "=== CircularQueue 測試套件 ===\n\n";
    
    // 運行所有測試
    run_test_constructor_and_destructor();
    run_test_constructor_with_default_value();
    run_test_enqueue_and_dequeue_basic();
    run_test_circular_behavior();
    run_test_overflow_exception();
    run_test_underflow_exception();
    run_test_copy_constructor();
    run_test_copy_constructor_with_circular_state();
    run_test_move_constructor();
    run_test_copy_assignment();
    run_test_move_assignment();
    run_test_self_assignment();
    run_test_clear_operation();
    run_test_capacity_and_state_checks();
    run_test_complex_circular_operations();
    run_test_string_operations();
    run_test_large_scale_operations();
    run_test_boundary_conditions();
    
    // 測試結果
    std::cout << "\n=== 測試結果 ===\n";
    std::cout << "通過: " << tests_passed << "/" << tests_total << " 測試\n";
    
    if (tests_passed == tests_total) {
        std::cout << "🎉 所有測試通過！\n";
        return 0;
    } else {
        std::cout << "❌ 有 " << (tests_total - tests_passed) << " 個測試失敗\n";
        return 1;
    }
}