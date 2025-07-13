#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <utility>

// 包含你的linked list實現
#include "linked_list.cpp"

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

// 輔助函數：檢查linked list內容
void verify_list_content(const LinkedList& list, const std::vector<int>& expected) {
    assert(list.size() == expected.size());
    
    // 如果是空list
    if (expected.empty()) {
        assert(list.empty());
        return;
    }
    
    // 檢查每個元素是否存在
    for (int value : expected) {
        assert(list.find(value));
    }
    
    assert(!list.empty());
}

// ============= 基本功能測試 =============

TEST(constructor_and_destructor) {
    // 測試默認構造函數
    LinkedList list;
    assert(list.empty());
    assert(list.size() == 0);
    
    // 測試析構函數會在作用域結束時自動調用
}

TEST(insert_and_size) {
    LinkedList list;
    
    // 測試插入第一個元素
    list.insert(1);
    assert(!list.empty());
    assert(list.size() == 1);
    assert(list.find(1));
    
    // 測試插入多個元素
    list.insert(2);
    list.insert(3);
    assert(list.size() == 3);
    assert(list.find(2));
    assert(list.find(3));
}

TEST(insert_front) {
    LinkedList list;
    
    // 測試在空list前面插入
    list.insertFront(1);
    assert(list.size() == 1);
    assert(list.find(1));
    
    // 測試在非空list前面插入
    list.insertFront(2);
    list.insertFront(3);
    assert(list.size() == 3);
    
    // 驗證順序：insertFront應該讓新元素成為第一個
    verify_list_content(list, {3, 2, 1});
}

TEST(find_functionality) {
    LinkedList list;
    
    // 空list中查找
    assert(!list.find(1));
    
    // 添加元素後查找
    list.insert(1);
    list.insert(2);
    list.insert(3);
    
    assert(list.find(1));
    assert(list.find(2));
    assert(list.find(3));
    assert(!list.find(4));  // 不存在的元素
    assert(!list.find(0));  // 不存在的元素
}

TEST(remove_functionality) {
    LinkedList list;
    
    // 從空list中移除（不應該崩潰）
    list.remove(1);
    assert(list.empty());
    
    // 添加元素後測試移除
    list.insert(1);
    list.insert(2);
    list.insert(3);
    assert(list.size() == 3);
    
    // 移除中間元素
    list.remove(2);
    assert(list.size() == 2);
    assert(!list.find(2));
    assert(list.find(1));
    assert(list.find(3));
    
    // 移除頭部元素
    list.remove(1);
    assert(list.size() == 1);
    assert(!list.find(1));
    assert(list.find(3));
    
    // 移除最後一個元素
    list.remove(3);
    assert(list.empty());
    assert(!list.find(3));
}

TEST(remove_edge_cases) {
    LinkedList list;
    
    // 只有一個元素的情況
    list.insert(42);
    list.remove(42);
    assert(list.empty());
    
    // 移除頭部元素
    list.insert(1);
    list.insert(2);
    list.insert(3);
    list.remove(1);  // 移除頭部
    assert(list.size() == 2);
    assert(!list.find(1));
    
    // 移除尾部元素
    list.remove(3);  // 移除尾部
    assert(list.size() == 1);
    assert(!list.find(3));
    assert(list.find(2));
    
    // 移除不存在的元素
    list.remove(999);
    assert(list.size() == 1);
    assert(list.find(2));
}

// ============= 拷貝和移動測試 =============

TEST(copy_constructor) {
    LinkedList original;
    original.insert(1);
    original.insert(2);
    original.insert(3);
    
    LinkedList copy = original;  // 拷貝構造
    
    // 檢查拷貝的內容
    assert(copy.size() == 3);
    verify_list_content(copy, {1, 2, 3});
    
    // 修改原list不應影響拷貝
    original.insert(4);
    assert(original.size() == 4);
    assert(copy.size() == 3);
    
    // 修改拷貝不應影響原list
    copy.remove(1);
    assert(copy.size() == 2);
    assert(original.size() == 4);
    assert(original.find(1));
}

TEST(move_constructor) {
    LinkedList original;
    original.insert(1);
    original.insert(2);
    original.insert(3);
    
    LinkedList moved = std::move(original);  // 移動構造
    
    // 檢查移動後的內容
    assert(moved.size() == 3);
    verify_list_content(moved, {1, 2, 3});
    
    // 原list應該被清空
    assert(original.empty());
    assert(original.size() == 0);
}

TEST(copy_assignment) {
    LinkedList list1, list2;
    
    list1.insert(1);
    list1.insert(2);
    
    list2.insert(10);
    list2.insert(20);
    list2.insert(30);
    
    list2 = list1;  // 拷貝賦值
    
    // 檢查賦值結果
    assert(list2.size() == 2);
    verify_list_content(list2, {1, 2});
    
    // 原list應該保持不變
    assert(list1.size() == 2);
    verify_list_content(list1, {1, 2});
}

TEST(move_assignment) {
    LinkedList list1, list2;
    
    list1.insert(1);
    list1.insert(2);
    list1.insert(3);
    
    list2.insert(10);
    list2.insert(20);
    
    list2 = std::move(list1);  // 移動賦值
    
    // 檢查移動結果
    assert(list2.size() == 3);
    verify_list_content(list2, {1, 2, 3});
    
    // 原list應該被清空
    assert(list1.empty());
}

TEST(self_assignment) {
    LinkedList list;
    list.insert(1);
    list.insert(2);
    list.insert(3);
    
    list = list;  // 自我賦值
    
    // 應該保持原樣
    assert(list.size() == 3);
    verify_list_content(list, {1, 2, 3});
}

// ============= 清理操作測試 =============

TEST(delete_all_functionality) {
    LinkedList list;
    
    // 空list調用deleteAll
    list.deleteAll();
    assert(list.empty());
    
    // 非空list調用deleteAll
    list.insert(1);
    list.insert(2);
    list.insert(3);
    assert(list.size() == 3);
    
    list.deleteAll();
    assert(list.empty());
    assert(list.size() == 0);
    
    // deleteAll後應該可以正常使用
    list.insert(100);
    assert(list.size() == 1);
    assert(list.find(100));
}

// ============= 混合操作測試 =============

TEST(mixed_operations) {
    LinkedList list;
    
    // 混合insert和insertFront
    list.insert(2);
    list.insertFront(1);
    list.insert(3);
    list.insertFront(0);
    
    assert(list.size() == 4);
    verify_list_content(list, {0, 1, 2, 3});
    
    // 混合remove操作
    list.remove(1);
    list.remove(3);
    assert(list.size() == 2);
    verify_list_content(list, {0, 2});
    
    // 清空後重新使用
    list.deleteAll();
    assert(list.empty());
    
    list.insertFront(100);
    list.insert(200);
    assert(list.size() == 2);
    verify_list_content(list, {100, 200});
}

// ============= 大量數據測試 =============

TEST(large_scale_operations) {
    LinkedList list;
    const int N = 1000;
    
    // 插入大量元素
    for (int i = 0; i < N; i++) {
        list.insert(i);
    }
    assert(list.size() == N);
    
    // 檢查所有元素都存在
    for (int i = 0; i < N; i++) {
        assert(list.find(i));
    }
    
    // 移除一半元素
    for (int i = 0; i < N; i += 2) {
        list.remove(i);
    }
    assert(list.size() == N/2);
    
    // 檢查剩餘元素
    for (int i = 1; i < N; i += 2) {
        assert(list.find(i));
    }
    for (int i = 0; i < N; i += 2) {
        assert(!list.find(i));
    }
}

// ============= 邊界條件測試 =============

TEST(boundary_conditions) {
    LinkedList list;
    
    // 重複插入相同值
    list.insert(1);
    list.insert(1);
    list.insert(1);
    assert(list.size() == 3);
    
    // 移除一個1（應該只移除第一個）
    list.remove(1);
    assert(list.size() == 2);
    assert(list.find(1));  // 還有其他的1
    
    // 負數測試
    list.insert(-1);
    list.insert(-100);
    assert(list.find(-1));
    assert(list.find(-100));
    
    // 大數測試
    list.insert(1000000);
    assert(list.find(1000000));
}

// ============= 記憶體管理測試 =============

TEST(memory_management) {
    // 測試多次創建和銷毀
    for (int i = 0; i < 100; i++) {
        LinkedList list;
        list.insert(i);
        list.insert(i + 1);
        list.insert(i + 2);
        
        assert(list.size() == 3);
        
        list.remove(i + 1);
        assert(list.size() == 2);
        
        // list會在作用域結束時自動銷毀
    }
    
    // 測試大量拷貝
    LinkedList original;
    for (int i = 0; i < 50; i++) {
        original.insert(i);
    }
    
    for (int i = 0; i < 10; i++) {
        LinkedList copy = original;
        assert(copy.size() == 50);
        // copy會在作用域結束時自動銷毀
    }
}

// ============= 鏈結操作測試 =============

TEST(chain_operations) {
    LinkedList list;
    
    // 測試連續操作後的狀態
    list.insert(1);
    list.insertFront(0);
    list.insert(2);
    list.remove(1);
    list.insertFront(-1);
    list.remove(0);
    
    assert(list.size() == 2);
    verify_list_content(list, {-1, 2});
    
    // 測試清空後重建
    list.deleteAll();
    for (int i = 5; i >= 1; i--) {
        list.insertFront(i);
    }
    
    assert(list.size() == 5);
    verify_list_content(list, {1, 2, 3, 4, 5});
}

// ============= 主測試函數 =============

int main() {
    std::cout << "=== LinkedList 測試套件 ===\n\n";
    
    // 運行所有測試
    run_test_constructor_and_destructor();
    run_test_insert_and_size();
    run_test_insert_front();
    run_test_find_functionality();
    run_test_remove_functionality();
    run_test_remove_edge_cases();
    run_test_copy_constructor();
    run_test_move_constructor();
    run_test_copy_assignment();
    run_test_move_assignment();
    run_test_self_assignment();
    run_test_delete_all_functionality();
    run_test_mixed_operations();
    run_test_large_scale_operations();
    run_test_boundary_conditions();
    run_test_memory_management();
    run_test_chain_operations();
    
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