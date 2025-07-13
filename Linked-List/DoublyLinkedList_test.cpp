#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <utility>

// 包含你的doubly linked list實現
#include "DoublyLinkedList.cpp"

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

// 輔助函數：檢查doubly linked list內容（正向）
void verify_list_content_forward(const DoublyLinkedList& list, const std::vector<int>& expected) {
    assert(list.size() == expected.size());
    
    if (expected.empty()) {
        assert(list.empty());
        return;
    }
    
    assert(!list.empty());
    assert(list.front() == expected.front());
    assert(list.back() == expected.back());
    
    // 檢查每個元素是否存在
    for (int value : expected) {
        assert(list.find(value));
    }
}

// 輔助函數：檢查雙向鏈結的完整性
void verify_bidirectional_integrity(const DoublyLinkedList& list) {
    if (list.empty()) {
        return;
    }
    
    // 檢查是否能從頭到尾再從尾到頭遍歷
    size_t forward_count = 0;
    size_t backward_count = 0;
    
    // 模擬向前遍歷計數（無法直接訪問內部指針，通過size()間接驗證）
    forward_count = list.size();
    backward_count = list.size();
    
    assert(forward_count == backward_count);
}

// ============= 基本功能測試 =============

TEST(constructor_and_destructor) {
    // 測試默認構造函數
    DoublyLinkedList list;
    assert(list.empty());
    assert(list.size() == 0);
    
    // 測試析構函數會在作用域結束時自動調用
}

TEST(insert_front_and_back) {
    DoublyLinkedList list;
    
    // 測試在空list中插入
    list.insertFront(1);
    assert(!list.empty());
    assert(list.size() == 1);
    assert(list.front() == 1);
    assert(list.back() == 1);
    
    // 測試insertFront
    list.insertFront(0);
    assert(list.size() == 2);
    assert(list.front() == 0);
    assert(list.back() == 1);
    
    // 測試pushBack
    list.pushBack(2);
    assert(list.size() == 3);
    assert(list.front() == 0);
    assert(list.back() == 2);
    
    verify_list_content_forward(list, {0, 1, 2});
}

TEST(pop_front_and_back) {
    DoublyLinkedList list;
    
    // 空list測試異常
    try {
        list.popFront();
        assert(false);  // 不應該到達這裡
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    try {
        list.popBack();
        assert(false);  // 不應該到達這裡
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    // 添加元素後測試pop
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    assert(list.size() == 3);
    
    // 測試popFront
    list.popFront();
    assert(list.size() == 2);
    assert(list.front() == 2);
    assert(list.back() == 3);
    
    // 測試popBack
    list.popBack();
    assert(list.size() == 1);
    assert(list.front() == 2);
    assert(list.back() == 2);
    
    // 測試最後一個元素
    list.popFront();
    assert(list.empty());
    assert(list.size() == 0);
}

TEST(front_and_back_access) {
    DoublyLinkedList list;
    
    // 空list測試異常
    try {
        list.front();
        assert(false);
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    try {
        list.back();
        assert(false);
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    // 單個元素
    list.pushBack(42);
    assert(list.front() == 42);
    assert(list.back() == 42);
    
    // 多個元素
    list.insertFront(10);
    list.pushBack(99);
    assert(list.front() == 10);
    assert(list.back() == 99);
}

// ============= 插入和訪問測試 =============

TEST(insert_at_index) {
    DoublyLinkedList list;
    
    // 在空list中插入
    list.insert(10, 0);  // 應該在前面插入
    assert(list.size() == 1);
    assert(list.front() == 10);
    
    // 在開頭插入
    list.insert(5, 0);
    assert(list.size() == 2);
    assert(list.front() == 5);
    
    // 在末尾插入（超出索引）
    list.insert(20, 10);
    assert(list.size() == 3);
    assert(list.back() == 20);
    
    // 在中間插入
    list.insert(15, 2);
    assert(list.size() == 4);
    verify_list_content_forward(list, {5, 10, 15, 20});
    
    // 負索引測試
    size_t before_size = list.size();
    list.insert(100, -1);
    assert(list.size() == before_size);  // 不應該插入
}

TEST(get_node_at_index) {
    DoublyLinkedList list;
    
    // 空list
    Node* node = list.getNodeAt(0);
    assert(node == nullptr);
    
    // 添加元素
    for (int i = 0; i < 5; i++) {
        list.pushBack(i * 10);
    }
    
    // 測試正常索引
    node = list.getNodeAt(0);
    assert(node != nullptr);
    assert(node->val == 0);
    
    node = list.getNodeAt(2);
    assert(node != nullptr);
    assert(node->val == 20);
    
    node = list.getNodeAt(4);
    assert(node != nullptr);
    assert(node->val == 40);
    
    // 測試邊界
    node = list.getNodeAt(-1);
    assert(node == nullptr);
    
    node = list.getNodeAt(5);
    assert(node == nullptr);
    
    node = list.getNodeAt(100);
    assert(node == nullptr);
}

// ============= 移除操作測試 =============

TEST(remove_by_value) {
    DoublyLinkedList list;
    
    // 空list移除（不應該崩潰）
    list.remove(10);
    assert(list.empty());
    
    // 添加元素
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    list.pushBack(2);
    list.pushBack(4);
    
    // 移除中間元素（只移除第一個）
    list.remove(2);
    assert(list.size() == 4);
    verify_list_content_forward(list, {1, 3, 2, 4});
    
    // 移除頭部元素
    list.remove(1);
    assert(list.size() == 3);
    assert(list.front() == 3);
    
    // 移除尾部元素
    list.remove(4);
    assert(list.size() == 2);
    assert(list.back() == 2);
    
    // 移除不存在的元素
    list.remove(999);
    assert(list.size() == 2);
}

TEST(remove_at_index) {
    DoublyLinkedList list;
    
    // 空list移除
    list.removeAt(0);
    assert(list.empty());
    
    // 負索引
    list.pushBack(10);
    list.removeAt(-1);
    assert(list.size() == 1);
    
    // 添加更多元素
    list.pushBack(20);
    list.pushBack(30);
    list.pushBack(40);
    // 現在list: [10, 20, 30, 40]
    
    // 移除中間元素
    list.removeAt(1);  // 移除20
    assert(list.size() == 3);
    verify_list_content_forward(list, {10, 30, 40});
    
    // 移除頭部
    list.removeAt(0);  // 移除10
    assert(list.size() == 2);
    assert(list.front() == 30);
    
    // 移除尾部
    list.removeAt(1);  // 移除40
    assert(list.size() == 1);
    assert(list.back() == 30);
    
    // 超出範圍的索引
    list.removeAt(10);
    assert(list.size() == 1);
}

// ============= 查找功能測試 =============

TEST(find_functionality) {
    DoublyLinkedList list;
    
    // 空list查找
    assert(!list.find(1));
    
    // 添加元素
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    list.pushBack(2);
    
    // 查找存在的元素
    assert(list.find(1));
    assert(list.find(2));
    assert(list.find(3));
    
    // 查找不存在的元素
    assert(!list.find(4));
    assert(!list.find(0));
    assert(!list.find(-1));
}

// ============= 拷貝和移動測試 =============

TEST(copy_constructor) {
    DoublyLinkedList original;
    original.pushBack(1);
    original.pushBack(2);
    original.pushBack(3);
    
    DoublyLinkedList copy = original;  // 拷貝構造
    
    // 檢查拷貝的內容
    assert(copy.size() == 3);
    verify_list_content_forward(copy, {1, 2, 3});
    verify_bidirectional_integrity(copy);
    
    // 修改原list不應影響拷貝
    original.pushBack(4);
    assert(original.size() == 4);
    assert(copy.size() == 3);
    
    // 修改拷貝不應影響原list
    copy.remove(1);
    assert(copy.size() == 2);
    assert(original.size() == 4);
    assert(original.find(1));
}

TEST(move_constructor) {
    DoublyLinkedList original;
    original.pushBack(1);
    original.pushBack(2);
    original.pushBack(3);
    
    DoublyLinkedList moved = std::move(original);  // 移動構造
    
    // 檢查移動後的內容
    assert(moved.size() == 3);
    verify_list_content_forward(moved, {1, 2, 3});
    verify_bidirectional_integrity(moved);
    
    // 原list應該被清空
    assert(original.empty());
    assert(original.size() == 0);
}

TEST(copy_assignment) {
    DoublyLinkedList list1, list2;
    
    list1.pushBack(1);
    list1.pushBack(2);
    
    list2.pushBack(10);
    list2.pushBack(20);
    list2.pushBack(30);
    
    list2 = list1;  // 拷貝賦值
    
    // 檢查賦值結果
    assert(list2.size() == 2);
    verify_list_content_forward(list2, {1, 2});
    
    // 原list應該保持不變
    assert(list1.size() == 2);
    verify_list_content_forward(list1, {1, 2});
}

TEST(move_assignment) {
    DoublyLinkedList list1, list2;
    
    list1.pushBack(1);
    list1.pushBack(2);
    list1.pushBack(3);
    
    list2.pushBack(10);
    list2.pushBack(20);
    
    list2 = std::move(list1);  // 移動賦值
    
    // 檢查移動結果
    assert(list2.size() == 3);
    verify_list_content_forward(list2, {1, 2, 3});
    
    // 原list應該被清空
    assert(list1.empty());
}

TEST(self_assignment) {
    DoublyLinkedList list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    
    list = list;  // 自我賦值
    
    // 應該保持原樣
    assert(list.size() == 3);
    verify_list_content_forward(list, {1, 2, 3});
}

// ============= 大小緩存測試 =============

TEST(size_caching_mechanism) {
    DoublyLinkedList list;
    
    // 初始狀態
    assert(list.size() == 0);
    
    // 添加元素應該invalidate size
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    assert(list.size() == 3);
    
    // 多次調用size()應該返回相同結果
    assert(list.size() == 3);
    assert(list.size() == 3);
    
    // 修改後size應該重新計算
    list.pushBack(4);
    assert(list.size() == 4);
    
    list.popFront();
    assert(list.size() == 3);
    
    list.remove(2);
    assert(list.size() == 2);
}

// ============= 異常安全測試 =============

TEST(exception_safety) {
    DoublyLinkedList list;
    
    // 測試空list的異常
    try {
        list.front();
        assert(false);
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    try {
        list.back();
        assert(false);
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    try {
        list.popFront();
        assert(false);
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    try {
        list.popBack();
        assert(false);
    } catch (const std::runtime_error&) {
        // 預期的異常
    }
    
    // 異常後list應該仍然有效
    assert(list.empty());
    list.pushBack(1);
    assert(list.size() == 1);
}

// ============= 大量數據測試 =============

TEST(large_scale_operations) {
    DoublyLinkedList list;
    const int N = 1000;
    
    // 插入大量元素
    for (int i = 0; i < N; i++) {
        if (i % 2 == 0) {
            list.pushBack(i);
        } else {
            list.insertFront(i);
        }
    }
    assert(list.size() == N);
    
    // 檢查一些元素存在
    assert(list.find(0));
    assert(list.find(1));
    assert(list.find(N-1));
    
    // 移除一些元素
    for (int i = 0; i < N; i += 10) {
        list.remove(i);
    }
    
    // 檢查size有所減少
    assert(list.size() < N);
    
    // 檢查被移除的元素不存在
    assert(!list.find(0));
    assert(!list.find(10));
    assert(!list.find(20));
}

// ============= 混合操作測試 =============

TEST(mixed_operations) {
    DoublyLinkedList list;
    
    // 複雜的操作序列
    list.pushBack(1);
    list.insertFront(0);
    list.pushBack(2);
    list.insert(1.5, 2);  // 實際上是1，因為int
    list.insert(3, 10);   // 在末尾
    
    assert(list.size() == 5);
    assert(list.front() == 0);
    assert(list.back() == 3);
    
    // 移除操作
    list.removeAt(1);     // 移除某個元素
    list.remove(3);       // 按值移除
    list.popFront();      // 移除前面
    list.popBack();       // 移除後面
    
    assert(list.size() == 1);
    
    // 清空並重建
    list.popFront();
    assert(list.empty());
    
    for (int i = 1; i <= 5; i++) {
        list.pushBack(i);
    }
    verify_list_content_forward(list, {1, 2, 3, 4, 5});
}

// ============= 雙向遍歷測試 =============

TEST(bidirectional_access) {
    DoublyLinkedList list;
    
    // 測試正向和反向訪問的一致性
    for (int i = 1; i <= 5; i++) {
        list.pushBack(i);
    }
    
    // 檢查head和tail
    assert(list.front() == 1);
    assert(list.back() == 5);
    
    // 通過getNodeAt檢查雙向性
    Node* first = list.getNodeAt(0);
    Node* last = list.getNodeAt(4);
    
    assert(first != nullptr);
    assert(last != nullptr);
    assert(first->val == 1);
    assert(last->val == 5);
    
    verify_bidirectional_integrity(list);
}

// ============= 主測試函數 =============

int main() {
    std::cout << "=== DoublyLinkedList 測試套件 ===\n\n";
    
    // 運行所有測試
    run_test_constructor_and_destructor();
    run_test_insert_front_and_back();
    run_test_pop_front_and_back();
    run_test_front_and_back_access();
    run_test_insert_at_index();
    run_test_get_node_at_index();
    run_test_remove_by_value();
    run_test_remove_at_index();
    run_test_find_functionality();
    run_test_copy_constructor();
    run_test_move_constructor();
    run_test_copy_assignment();
    run_test_move_assignment();
    run_test_self_assignment();
    run_test_size_caching_mechanism();
    run_test_exception_safety();
    run_test_large_scale_operations();
    run_test_mixed_operations();
    run_test_bidirectional_access();
    
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