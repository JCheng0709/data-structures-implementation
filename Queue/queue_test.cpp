#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <chrono>
#include <utility>
#include <limits>
#include <type_traits>

// 假設你的queue類別已經定義好
#include "queue.cpp"

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

// 輔助函數：檢查queue內容
template<typename T>
void verify_queue_content(const queue<T>& q, const std::vector<T>& expected) {
    assert(q.size() == expected.size());
    
    queue<T> temp = q;  // 拷貝以便測試
    for (size_t i = 0; i < expected.size(); i++) {
        assert(!temp.empty());
        assert(temp.front() == expected[i]);
        temp.dequeue();
    }
    assert(temp.empty());
}

// ============= 基本功能測試 =============

TEST(constructor_and_destructor) {
    // 測試構造函數
    queue<int> q1(5);
    assert(q1.empty());
    assert(q1.size() == 0);
    
    // 測試默認構造
    queue<std::string> q2(10);
    assert(q2.empty());
}

TEST(enqueue_and_dequeue) {
    queue<int> q(3);
    
    // 測試enqueue
    q.enqueue(1);
    assert(!q.empty());
    assert(q.size() == 1);
    assert(q.front() == 1);
    
    q.enqueue(2);
    q.enqueue(3);
    assert(q.size() == 3);
    assert(q.front() == 1);
    
    // 測試dequeue
    q.dequeue();
    assert(q.size() == 2);
    assert(q.front() == 2);
    
    q.dequeue();
    assert(q.size() == 1);
    assert(q.front() == 3);
    
    q.dequeue();
    assert(q.empty());
}

TEST(front_and_back_access) {
    queue<std::string> q(3);
    
    q.enqueue("first");
    assert(q.front() == "first");
    assert(q.back() == "first");
    
    q.enqueue("second");
    assert(q.front() == "first");
    assert(q.back() == "second");
    
    q.enqueue("third");
    assert(q.front() == "first");
    assert(q.back() == "third");
    
    // 注意：由於原始queue的front()和back()返回const引用，
    // 所以我們無法修改元素，只能測試讀取功能
    
    // 如果你的queue提供了非const版本的front()和back()，
    // 可以取消註釋下面的測試：
    
    // q.front() = "modified_first";
    // q.back() = "modified_third";
    // assert(q.front() == "modified_first");
    // assert(q.back() == "modified_third");
}

// ============= 動態擴容測試 =============

TEST(dynamic_resizing) {
    queue<int> q(2);  // 小容量
    
    // 超過初始容量
    for (int i = 1; i <= 10; i++) {
        q.enqueue(i);
    }
    
    assert(q.size() == 10);
    // 注意：原始queue沒有capacity()方法，所以我們只測試功能正確性
    
    // 驗證內容正確
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    verify_queue_content(q, expected);
}

TEST(resize_with_dequeue) {
    queue<int> q(4);
    
    // 填滿queue
    for (int i = 1; i <= 4; i++) {
        q.enqueue(i);
    }
    
    // dequeue一些元素
    q.dequeue();  // 移除1
    q.dequeue();  // 移除2
    
    // 繼續添加元素，觸發resize
    for (int i = 5; i <= 8; i++) {
        q.enqueue(i);
    }
    
    // 驗證內容：應該是[3, 4, 5, 6, 7, 8]
    std::vector<int> expected = {3, 4, 5, 6, 7, 8};
    verify_queue_content(q, expected);
}

// ============= 拷貝和移動測試 =============

TEST(copy_constructor) {
    queue<std::string> q1(3);
    q1.enqueue("a");
    q1.enqueue("b");
    q1.enqueue("c");
    
    queue<std::string> q2 = q1;  // 拷貝構造
    
    assert(q2.size() == 3);
    assert(q2.front() == "a");
    assert(q2.back() == "c");
    
    // 修改原queue不應影響拷貝
    q1.dequeue();
    assert(q1.size() == 2);
    assert(q2.size() == 3);
}

TEST(move_constructor) {
    queue<int> q1(3);
    q1.enqueue(1);
    q1.enqueue(2);
    q1.enqueue(3);
    
    queue<int> q2 = std::move(q1);  // 移動構造
    
    assert(q2.size() == 3);
    assert(q2.front() == 1);
    assert(q1.empty());  // q1應該被清空
}

TEST(assignment_operator) {
    queue<int> q1(3), q2(5);
    
    q1.enqueue(1);
    q1.enqueue(2);
    
    q2.enqueue(10);
    q2.enqueue(20);
    q2.enqueue(30);
    
    q2 = q1;  // 賦值
    
    assert(q2.size() == 2);
    assert(q2.front() == 1);
    assert(q2.back() == 2);
}

// ============= 異常處理測試 =============

TEST(empty_queue_exceptions) {
    queue<int> q(1);
    
    // 空queue訪問應該拋出異常
    try {
        q.front();
        assert(false);  // 不應該到達這裡
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    try {
        q.back();
        assert(false);
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    try {
        q.dequeue();
        assert(false);
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
}

// ============= 大量數據測試 =============

TEST(large_scale_operations) {
    queue<int> q(1);  // 從很小的容量開始
    const int N = 1000;
    
    // 添加大量元素
    for (int i = 0; i < N; i++) {
        q.enqueue(i);
    }
    assert(q.size() == N);
    
    // 移除一半
    for (int i = 0; i < N/2; i++) {
        assert(q.front() == i);
        q.dequeue();
    }
    assert(q.size() == N/2);
    
    // 再添加一些
    for (int i = N; i < N + 100; i++) {
        q.enqueue(i);
    }
    
    assert(q.size() == N/2 + 100);
    assert(q.front() == N/2);
}

// ============= 清理操作測試 =============

TEST(clear_operation) {
    queue<std::string> q(3);
    
    q.enqueue("test1");
    q.enqueue("test2");
    q.enqueue("test3");
    
    assert(q.size() == 3);
    
    q.clear();
    
    assert(q.empty());
    assert(q.size() == 0);
    
    // 清理後應該可以正常使用
    q.enqueue("after_clear");
    assert(q.size() == 1);
    assert(q.front() == "after_clear");
}

// ============= 複雜類型測試 =============

class TestObject {
public:
    int value;
    static int construct_count;
    static int destruct_count;
    
    TestObject(int v = 0) : value(v) { 
        construct_count++; 
        std::cout << "Construct " << value << " (total: " << construct_count << ")\n";
    }
    
    TestObject(const TestObject& other) : value(other.value) { 
        construct_count++; 
        std::cout << "Copy construct " << value << " (total: " << construct_count << ")\n";
    }
    
    TestObject(TestObject&& other) noexcept : value(other.value) { 
        construct_count++; 
        std::cout << "Move construct " << value << " (total: " << construct_count << ")\n";
    }
    
    ~TestObject() { 
        destruct_count++; 
        std::cout << "Destruct " << value << " (total: " << destruct_count << ")\n";
    }
    
    TestObject& operator=(const TestObject& other) {
        value = other.value;
        std::cout << "Copy assign " << value << "\n";
        return *this;
    }
    
    TestObject& operator=(TestObject&& other) noexcept {
        value = other.value;
        std::cout << "Move assign " << value << "\n";
        return *this;
    }
    
    bool operator==(const TestObject& other) const {
        return value == other.value;
    }
};

int TestObject::construct_count = 0;
int TestObject::destruct_count = 0;

TEST(complex_object_management) {
    // 重置計數器
    TestObject::construct_count = 0;
    TestObject::destruct_count = 0;
    
    std::cout << "\n=== 開始複雜物件管理測試 ===\n";
    
    {
        std::cout << "建立queue...\n";
        queue<TestObject> q(2);
        
        std::cout << "第一次enqueue...\n";
        q.enqueue(TestObject(1));  // 構造臨時物件 + 拷貝到queue + 析構臨時物件
        
        std::cout << "第二次enqueue...\n";
        q.enqueue(TestObject(2));  // 同上
        
        std::cout << "第三次enqueue (觸發resize)...\n";
        q.enqueue(TestObject(3));  // 觸發resize: 構造臨時物件 + resize過程 + 析構臨時物件
        
        assert(q.size() == 3);
        assert(q.front().value == 1);
        
        std::cout << "dequeue一個元素...\n";
        q.dequeue();  // 應該調用析構函數
        assert(q.front().value == 2);
        
        std::cout << "clear queue...\n";
        q.clear();  // 應該調用所有剩餘元素的析構函數
        assert(q.empty());
        
        std::cout << "離開作用域前 - 構造: " << TestObject::construct_count 
                  << ", 析構: " << TestObject::destruct_count << "\n";
    }  // queue析構 - 如果clear()正確工作，這裡不應該有額外的析構
    
    std::cout << "離開作用域後 - 構造: " << TestObject::construct_count 
              << ", 析構: " << TestObject::destruct_count << "\n";
    
    // 檢查是否有記憶體洩漏
    if (TestObject::construct_count != TestObject::destruct_count) {
        std::cout << "❌ 記憶體洩漏檢測失敗!\n";
        std::cout << "構造次數: " << TestObject::construct_count << "\n";
        std::cout << "析構次數: " << TestObject::destruct_count << "\n";
        std::cout << "差異: " << (TestObject::construct_count - TestObject::destruct_count) << "\n";
    }
    
    assert(TestObject::construct_count == TestObject::destruct_count);
    std::cout << "=== 複雜物件管理測試完成 ===\n\n";
}

// ============= 性能測試 =============

TEST(performance_test) {
    queue<int> q(1);
    const int N = 10000;
    
    auto start = std::chrono::steady_clock::now();
    
    // 混合操作
    for (int i = 0; i < N; i++) {
        q.enqueue(i);
        if (i % 3 == 0 && !q.empty()) {
            q.dequeue();
        }
    }
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Performance test completed in " << duration.count() << "ms\n";
    assert(duration.count() < 1000);  // 應該在1秒內完成
}

// ============= 主測試函數 =============

int main() {
    std::cout << "=== Queue 測試套件 ===\n\n";
    
    // 運行所有測試
    run_test_constructor_and_destructor();
    run_test_enqueue_and_dequeue();
    run_test_front_and_back_access();
    run_test_dynamic_resizing();
    run_test_resize_with_dequeue();
    run_test_copy_constructor();
    run_test_move_constructor();
    run_test_assignment_operator();
    run_test_empty_queue_exceptions();
    run_test_large_scale_operations();
    run_test_clear_operation();
    run_test_complex_object_management();
    run_test_performance_test();
    
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