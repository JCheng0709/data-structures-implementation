#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <chrono>
#include <utility>
#include <limits>
#include <type_traits>

// 包含你的stack實現
#include "stack.cpp"

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

// 輔助函數：檢查stack內容
template<typename T>
void verify_stack_content(stack<T> s, const std::vector<T>& expected) {
    assert(s.size() == expected.size());
    
    // 從top到bottom檢查
    for (int i = expected.size() - 1; i >= 0; i--) {
        assert(!s.isEmpty());
        assert(s.top() == expected[i]);
        s.pop();
    }
    assert(s.isEmpty());
}

// ============= 基本功能測試 =============

TEST(constructor_and_destructor) {
    // 測試構造函數
    stack<int> s1(5);
    assert(s1.isEmpty());
    assert(s1.size() == 0);
    assert(!s1.isFull());
    
    // 測試不同類型
    stack<std::string> s2(10);
    assert(s2.isEmpty());
    assert(s2.size() == 0);
}

TEST(push_and_pop_basic) {
    stack<int> s(3);
    
    // 測試push
    s.push(1);
    assert(!s.isEmpty());
    assert(s.size() == 1);
    assert(s.top() == 1);
    
    s.push(2);
    assert(s.size() == 2);
    assert(s.top() == 2);
    
    s.push(3);
    assert(s.size() == 3);
    assert(s.top() == 3);
    assert(s.isFull());
    
    // 測試pop
    s.pop();
    assert(s.size() == 2);
    assert(s.top() == 2);
    assert(!s.isFull());
    
    s.pop();
    assert(s.size() == 1);
    assert(s.top() == 1);
    
    s.pop();
    assert(s.isEmpty());
    assert(s.size() == 0);
}

TEST(top_access_and_modification) {
    stack<std::string> s(3);
    
    s.push("first");
    assert(s.top() == "first");
    
    s.push("second");
    assert(s.top() == "second");
    
    // 測試修改top元素
    s.top() = "modified";
    assert(s.top() == "modified");
    assert(s.size() == 2);
    
    s.pop();
    assert(s.top() == "first");
}

TEST(lifo_behavior) {
    stack<int> s(5);
    
    // 驗證LIFO行為
    for (int i = 1; i <= 5; i++) {
        s.push(i);
    }
    
    for (int i = 5; i >= 1; i--) {
        assert(s.top() == i);
        s.pop();
    }
    
    assert(s.isEmpty());
}

// ============= 動態擴容測試 =============

TEST(dynamic_resizing) {
    stack<int> s(2);  // 小容量
    
    // 超過初始容量
    for (int i = 1; i <= 10; i++) {
        s.push(i);
    }
    
    assert(s.size() == 10);
    assert(s.top() == 10);
    
    // 驗證所有元素都存在且順序正確
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    verify_stack_content(s, expected);
}

TEST(multiple_resize_operations) {
    stack<int> s(1);  // 從很小的容量開始
    
    // 多次觸發resize
    for (int i = 0; i < 20; i++) {
        s.push(i);
    }
    
    assert(s.size() == 20);
    
    // 驗證內容正確性
    for (int i = 19; i >= 0; i--) {
        assert(s.top() == i);
        s.pop();
    }
    
    assert(s.isEmpty());
}

// ============= 拷貝和移動測試 =============

TEST(copy_constructor) {
    stack<std::string> s1(3);
    s1.push("a");
    s1.push("b");
    s1.push("c");
    
    stack<std::string> s2 = s1;  // 拷貝構造
    
    assert(s2.size() == 3);
    assert(s2.top() == "c");
    
    // 修改原stack不應影響拷貝
    s1.pop();
    assert(s1.size() == 2);
    assert(s2.size() == 3);
    
    // 驗證拷貝的內容
    assert(s2.top() == "c");
    s2.pop();
    assert(s2.top() == "b");
}

TEST(move_constructor) {
    stack<int> s1(3);
    s1.push(1);
    s1.push(2);
    s1.push(3);
    
    stack<int> s2 = std::move(s1);  // 移動構造
    
    assert(s2.size() == 3);
    assert(s2.top() == 3);
    assert(s1.isEmpty());  // s1應該被清空
}

TEST(assignment_operator) {
    stack<int> s1(3), s2(5);
    
    s1.push(1);
    s1.push(2);
    
    s2.push(10);
    s2.push(20);
    s2.push(30);
    
    s2 = s1;  // 賦值
    
    assert(s2.size() == 2);
    assert(s2.top() == 2);
    s2.pop();
    assert(s2.top() == 1);
}

TEST(self_assignment) {
    stack<int> s(3);
    s.push(1);
    s.push(2);
    
    s = s;  // 自我賦值
    
    assert(s.size() == 2);
    assert(s.top() == 2);
}

// ============= 異常處理測試 =============

TEST(empty_stack_exceptions) {
    stack<int> s(1);
    
    // 空stack訪問應該拋出異常
    try {
        s.top();
        assert(false);  // 不應該到達這裡
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    try {
        s.pop();
        assert(false);
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
}

TEST(exception_safety_during_resize) {
    // 這個測試驗證resize過程中的異常安全
    stack<std::string> s(2);
    
    s.push("test1");
    s.push("test2");
    
    // 觸發resize
    s.push("test3");
    
    assert(s.size() == 3);
    assert(s.top() == "test3");
}

// ============= 清理操作測試 =============

TEST(clear_operation) {
    stack<std::string> s(3);
    
    s.push("test1");
    s.push("test2");
    s.push("test3");
    
    assert(s.size() == 3);
    
    s.clear();
    
    assert(s.isEmpty());
    assert(s.size() == 0);
    
    // 清理後應該可以正常使用
    s.push("after_clear");
    assert(s.size() == 1);
    assert(s.top() == "after_clear");
}

// ============= 大量數據測試 =============

TEST(large_scale_operations) {
    stack<int> s(1);  // 從很小的容量開始
    const int N = 1000;
    
    // 添加大量元素
    for (int i = 0; i < N; i++) {
        s.push(i);
    }
    assert(s.size() == N);
    assert(s.top() == N - 1);
    
    // 移除一半
    for (int i = 0; i < N/2; i++) {
        assert(s.top() == N - 1 - i);
        s.pop();
    }
    assert(s.size() == N/2);
    
    // 再添加一些
    for (int i = N; i < N + 100; i++) {
        s.push(i);
    }
    
    assert(s.size() == N/2 + 100);
    assert(s.top() == N + 99);
}

// ============= 混合操作測試 =============

TEST(mixed_operations) {
    stack<int> s(2);
    
    // 混合push和pop操作
    s.push(1);
    s.push(2);
    assert(s.top() == 2);
    
    s.pop();
    assert(s.top() == 1);
    
    s.push(3);
    s.push(4);  // 觸發resize
    assert(s.size() == 3);
    assert(s.top() == 4);
    
    s.clear();
    assert(s.isEmpty());
    
    s.push(100);
    assert(s.top() == 100);
}

// ============= 複雜類型測試 =============

class TestObject {
public:
    int value;
    static int construct_count;
    static int destruct_count;
    
    TestObject(int v = 0) : value(v) { 
        construct_count++; 
    }
    
    TestObject(const TestObject& other) : value(other.value) { 
        construct_count++; 
    }
    
    TestObject(TestObject&& other) noexcept : value(other.value) { 
        construct_count++; 
    }
    
    ~TestObject() { 
        destruct_count++; 
    }
    
    TestObject& operator=(const TestObject& other) {
        value = other.value;
        return *this;
    }
    
    TestObject& operator=(TestObject&& other) noexcept {
        value = other.value;
        return *this;
    }
    
    bool operator==(const TestObject& other) const {
        return value == other.value;
    }
};

int TestObject::construct_count = 0;
int TestObject::destruct_count = 0;

TEST(complex_object_management) {
    TestObject::construct_count = 0;
    TestObject::destruct_count = 0;
    
    {
        stack<TestObject> s(2);
        
        s.push(TestObject(1));
        s.push(TestObject(2));
        s.push(TestObject(3));  // 觸發resize
        
        assert(s.size() == 3);
        assert(s.top().value == 3);
        
        s.pop();
        assert(s.top().value == 2);
        
        s.clear();
        assert(s.isEmpty());
        
    }  // stack析構
    
    // 由於使用相同的記憶體管理策略，應該沒有洩漏
    // (構造的物件數量應該等於析構的物件數量)
}

// ============= 性能測試 =============

TEST(performance_test) {
    stack<int> s(1);
    const int N = 10000;
    
    auto start = std::chrono::steady_clock::now();
    
    // 大量push操作
    for (int i = 0; i < N; i++) {
        s.push(i);
    }
    
    // 大量pop操作
    for (int i = 0; i < N/2; i++) {
        s.pop();
    }
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "\nPerformance test completed in " << duration.count() << "ms ";
    assert(duration.count() < 1000);  // 應該在1秒內完成
    assert(s.size() == N/2);
}

// ============= 邊界條件測試 =============

TEST(boundary_conditions) {
    // 測試容量為1的stack
    stack<int> s1(1);
    
    // 初始狀態：空的，但不滿
    assert(s1.isEmpty());
    assert(!s1.isFull());
    
    s1.push(42);
    assert(s1.isFull());    // 容量1，元素1 -> 滿了
    assert(s1.top() == 42);
    
    s1.push(43);            // 觸發resize，容量變成2
    assert(s1.size() == 2); // 現在有2個元素
    assert(s1.top() == 43); // top應該是最新push的元素
    
    // resize後：容量2，元素2 -> 又滿了！這是正確的行為
    assert(s1.isFull());    // 應該是滿的 (2/2)
    
    s1.push(44);            // 再次觸發resize，容量變成4
    assert(s1.size() == 3); // 現在有3個元素，容量4
    assert(!s1.isFull());   // 3個元素，容量4，不滿 (3/4)
    assert(s1.top() == 44);
    
    // 驗證所有元素都存在且順序正確
    assert(s1.top() == 44); s1.pop();
    assert(s1.top() == 43); s1.pop();
    assert(s1.top() == 42); s1.pop();
    assert(s1.isEmpty());
}

// ============= 主測試函數 =============

int main() {
    std::cout << "=== Stack 測試套件 ===\n\n";
    
    // 運行所有測試
    run_test_constructor_and_destructor();
    run_test_push_and_pop_basic();
    run_test_top_access_and_modification();
    run_test_lifo_behavior();
    run_test_dynamic_resizing();
    run_test_multiple_resize_operations();
    run_test_copy_constructor();
    run_test_move_constructor();
    run_test_assignment_operator();
    run_test_self_assignment();
    run_test_empty_stack_exceptions();
    run_test_exception_safety_during_resize();
    run_test_clear_operation();
    run_test_large_scale_operations();
    run_test_mixed_operations();
    run_test_complex_object_management();
    run_test_performance_test();
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